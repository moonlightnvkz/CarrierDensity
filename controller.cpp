#include <numeric> // std::iota
#include <sstream>
#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <cmath>
#include "controller.h"
#include "formulas.h"

Controller &Controller::GetInstance()
{
    static Controller instance;
    return instance;
}

void Controller::Init()
{
    Controller &c = Controller::GetInstance();
    if (c.inited) {
        return;
    }

    c.model.T.resize(1000);
    std::iota(c.model.T.begin(), c.model.T.end(), 1);
    c.Recalculate();
}

void Controller::LoadSettings()
{
    QString settingsPath = QApplication::applicationDirPath() + SettingsFile;
    QSettings settings(settingsPath, QSettings::IniFormat);

    QVariant value;
    QStringList presetNames = settings.childGroups();

    bool ok = presetNames.size() > 0;
    if (!ok) {
        qDebug() << "size " << presetNames.size();
        return LoadSettingsDefault();
    }

    std::map<QString, Model::Preset> presets;
    for (const QString &name : presetNames) {
        Model::Preset preset;
        value = settings.value(name + "/Eg", QVariant(0.0));
        preset.Eg = value.toDouble(&ok);
        if (!ok) {
            return LoadSettingsDefault();
        }
        value = settings.value(name + "/me", QVariant(0.0));
        preset.me = value.toDouble(&ok);
        if (!ok) {
            return LoadSettingsDefault();
        }
        value = settings.value(name + "/mh", QVariant(0.0));
        preset.mh = value.toDouble(&ok);
        if (!ok) {
            return LoadSettingsDefault();
        }
        value = settings.value(name + "/ae", QVariant(0.0));
        preset.ae = value.toDouble(&ok);
        if (!ok) {
            return LoadSettingsDefault();
        }
        value = settings.value(name + "/be", QVariant(0.0));
        preset.be = value.toDouble(&ok);
        if (!ok) {
            return LoadSettingsDefault();
        }
        value = settings.value(name + "/ah", QVariant(0.0));
        preset.ah = value.toDouble(&ok);
        if (!ok) {
            return LoadSettingsDefault();
        }
        value = settings.value(name + "/bh", QVariant(0.0));
        preset.bh = value.toDouble(&ok);
        if (!ok) {
            return LoadSettingsDefault();
        }

        presets[name] = preset;
    }

    model.presets = presets;
    model.currentPreset = presetNames[0];
}

void Controller::SaveSettings()
{
    QString settingsPath = QApplication::applicationDirPath() + SettingsFile;
    QSettings settings(settingsPath, QSettings::IniFormat);
    qDebug() << settings.status();
    qDebug() << settings.fileName();

    for (const auto &kv : model.presets) {
        settings.setValue(kv.first+ "/Eg", kv.second.Eg);
        settings.setValue(kv.first+ "/me", kv.second.me);
        settings.setValue(kv.first+ "/mh", kv.second.mh);
        settings.setValue(kv.first+ "/ae", kv.second.ae);
        settings.setValue(kv.first+ "/be", kv.second.be);
        settings.setValue(kv.first+ "/ah", kv.second.ah);
        settings.setValue(kv.first+ "/bh", kv.second.bh);
    }
}

bool Controller::LoadFromASCII(QTextStream &is)
{
    return model.Deserialize(is);
}

bool Controller::SaveToASCII(QTextStream &os)
{
    return model.Serialize(os);
}

static double GetMu(double a, double b, double T, double NdPlus, double NaMinus) {
    double Tp = pow(T, 1.5);
    return a / (Tp + b * (NdPlus + NaMinus) / Tp);
}

// FIXME: LoadMobility: model.muh
bool Controller::LoadMobility()
{
    model.mue.resize(model.T.size());
    model.muh.resize(model.T.size());

    Model::Preset preset = model.presets[model.currentPreset];
    for (size_t i = 0; i < model.T.size(); ++i) {
        model.mue[i] = GetMu(preset.ae, preset.be, model.T[i], model.NdPlus[i], model.NaMinus[i]);
        model.muh[i] = GetMu(preset.ah, preset.bh, model.T[i], model.NdPlus[i], model.NaMinus[i]);
    }
    return true;
//    QByteArray mobility;
//    if (!CallMobility({"--mobility"}, mobility)) {
//        return false;
//    } else {
//        std::stringstream ss(mobility.toStdString());
//        return model.LoadMobility(ss);
//    }
}

void Controller::Recalculate()
{
    model.mue.clear();
    model.muh.clear();
    model.Nc.clear();
    model.Nv.clear();
    model.NdPlus.clear();
    model.NaMinus.clear();
    model.n.clear();
    model.p.clear();
    model.sigma.clear();
    Model::Preset preset = model.presets[model.currentPreset];
    for (double t : model.T) {
        model.Nc.push_back(Formulas::CalcEffectiveDensityState(preset.me, t));
        model.Nv.push_back(Formulas::CalcEffectiveDensityState(preset.mh, t));
        double Ec = preset.Eg; // Assume Ev == 0
        double Ef = Formulas::CalcFermiLevel(model.Nc.back(), model.Nv.back(), t, model.Na0, model.Nd0, Ec, model.Ea, model.Ed);
        model.n.push_back(Formulas::CalcN(model.Nc.back(), preset.Eg, Ef, t));
        model.p.push_back(Formulas::CalcP(model.Nv.back(), Ef, t));
        model.NaMinus.push_back(Formulas::CalcNaMinus(t, Ef, model.Ea, model.Na0));
        model.NdPlus.push_back(Formulas::CalcNdPlus(t, Ef, model.Ed, model.Nd0));
    }

    LoadMobility();

    for (size_t i = 0; i < model.T.size(); ++i) {
        model.sigma.push_back(Formulas::FindConductivity(model.n[i], model.p[i], model.mue[i], model.muh[i]));

    }
}

void Controller::UpdateT(double from, double to)
{
    model.T.clear();
    double dt = (to - from) / Model::TCount;
    for (double t = from; t < to + dt; t += dt) {
        model.T.push_back(t);
    }

    Recalculate();
}

void Controller::UpdateMaterial(const QString &name)
{
    model.currentPreset = name;
    Recalculate();
}

void Controller::UpdateEd(double value)
{
    model.Ed = value;
    Recalculate();
}

void Controller::UpdateEa(double value)
{
    model.Ea = value;
    Recalculate();
}

void Controller::UpdateNd0(double value)
{
    model.Nd0 = value;
    Recalculate();
}

void Controller::UpdateNa0(double value)
{
    model.Na0 = value;
    Recalculate();
}

const QString & Controller::GetMaterial() const
{
    return model.currentPreset;
}

QStringList Controller::GetAllMaterials() const
{
    QStringList names;
    for (const auto &kv : model.presets) {
        names.push_back(kv.first);
    }
    return names;
}

double Controller::GetEd() const
{
    return model.Ed;
}

double Controller::GetNd0() const
{
    return model.Nd0;
}

double Controller::GetEa() const
{
    return model.Ea;
}

double Controller::GetNa0() const
{
    return model.Na0;
}

// TODO: initialize model
Controller::Controller() {

}

bool Controller::CallMobility(QStringList args, QByteArray &data)
{
    QProcess process;
    process.start("mobility.exe", args);
    process.waitForBytesWritten();
    process.waitForFinished();

    QByteArray error = process.readAllStandardError();
    if (error.size() != 0) {
        return false;
    } else {
        data = process.readAllStandardOutput();
        return true;
    }
}

void Controller::LoadSettingsDefault()
{
    qDebug() << "LoadSettingsDefault";
    model.presets["Ge"] = {0.661, 0.22, 0.34, 2.33063e7, 6.63994, 1.5116e7, 3.42064};
    model.presets["Si"] = {1.12, 1.08, 0.56, 9.80389e6, 7.55398, 5.58920e6, 7.21393};
    model.currentPreset = "Ge";
    SaveSettings();
}

const std::vector<double> &Controller::GetTemperature()
{
    return model.T;
}

const std::vector<double> &Controller::GetElectronsConcentration()
{
    return model.n;
}

const std::vector<double> &Controller::GetHolesConcentration()
{
    return model.p;
}

const std::vector<double> &Controller::GetDonorsConcentration()
{
    return model.NdPlus;
}

const std::vector<double> &Controller::GetAcceptorsConcentration()
{
    return model.NaMinus;
}

const std::vector<double> &Controller::GetElectronsMobility()
{
    return model.mue;
}

const std::vector<double> &Controller::GetHolesMobility()
{
    return model.muh;
}

const std::vector<double> &Controller::GetConductivity()
{
    return model.sigma;
}
