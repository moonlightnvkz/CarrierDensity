#include <numeric> // std::iota
#include <sstream>
#include <QProcess>
#include <QDebug>
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

bool Controller::LoadFromASCII(std::istream &is)
{
    return model.Deserialize(is);
}

bool Controller::SaveToASCII(std::ostream &os)
{
    return model.Serialize(os);
}

static double GetMue(double a, double b, double T, double NdPlus, double NaMinus) {
    double Tp = pow(T, 1.5);
    return a / (Tp + b * (NdPlus + NaMinus) / Tp);
}

// FIXME: LoadMobility: model.muh
bool Controller::LoadMobility()
{
    model.mue.resize(model.T.size());
    model.muh.resize(model.T.size());
    constexpr const double a = 1e6;
    constexpr const double b = 1e-7;
    for (size_t i = 0; i < model.T.size(); ++i) {
        model.mue[i] = GetMue(a, b, model.T[i], model.NdPlus[i], model.NaMinus[i]);
        model.muh[i] = model.mue[i];
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

    for (double t : model.T) {
        model.Nc.push_back(Formulas::CalcEffectiveDensityState(model.me, t));
        model.Nv.push_back(Formulas::CalcEffectiveDensityState(model.mh, t));
        double Ec = model.Eg; // Assume Ev == 0
        double Ef = Formulas::CalcFermiLevel(model.Nc.back(), model.Nv.back(), t, model.Na0, model.Nd0, Ec, model.Ea, model.Ed);
        model.n.push_back(Formulas::CalcN(model.Nc.back(), model.Eg, Ef, t));
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

void Controller::UpdateMaterial(int idx)
{
    Model::Preset p = static_cast<Model::Preset>(idx);
    model.LoadPreset(p);
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

void Controller::UpdateEg(double value)
{
    model.Eg = value;
    Recalculate();
}

void Controller::UpdateMe(double value)
{
    model.me = value;
    Recalculate();
}

void Controller::UpdateMh(double value)
{
    model.mh = value;
    Recalculate();
}

int Controller::GetMaterial() const
{
    return static_cast<int>(model.preset);
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

double Controller::GetEg() const
{
    return model.Eg;
}

double Controller::GetMe() const
{
    return model.me;
}

double Controller::GetMh() const
{
    return model.mh;
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
