#include <numeric> // std::iota
#include <sstream>
#include <QProcess>
#include "controller.h"
#include "formulas.h"

Controller &Controller::GetInstance()
{
    static Controller instance;
    return instance;
}

bool Controller::LoadFromASCII(std::istream &is)
{
    return model.Deserialize(is);
}

bool Controller::SaveToASCII(std::ostream &os)
{
    return model.Serialize(os);
}

// FIXME: LoadMobility
bool Controller::LoadMobility()
{
    model.mue.resize(model.T.size());
    model.muh.resize(model.T.size());
    return true;
//    QByteArray mobility;
//    if (!CallMobility({"--mobility"}, mobility)) {
//        return false;
//    } else {
//        std::stringstream ss(mobility.toStdString());
//        return model.LoadMobility(ss);
//    }
}

// TODO: implement Recalculate
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

    LoadMobility();
    for (double t : model.T) {
        model.Nc.push_back(Formulas::CalcEffectiveDensityState(t));
        model.Nv.push_back(Formulas::CalcEffectiveDensityState(t));
        double Ef = Formulas::CalcFermiLevel(model.Nc.back(), model.Nv.back(), t, model.Na0, model.Nd0, model.Eg, model.Ea, model.Ed);
        model.n.push_back(Formulas::CalcN(model.Nc.back(), Ef, t));
        model.p.push_back(Formulas::CalcP(model.Nv.back(), Ef, t));
        // FIXME: NdPlus
        model.NdPlus.push_back(1);
        // FIXME: NaMinus
        model.NaMinus.push_back(1);
        model.sigma.push_back(Formulas::FindConductivity(model.mue.back(), model.muh.back(), t,
                                                         model.Na0, model.Nd0, model.Eg, model.Ea, model.Ed));
    }
}

void Controller::UpdateT(double from, double to)
{
    model.T.clear();
    double dt = (to - from) / Model::TCount;
    for (double t = from; t < to; t += dt) {
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
Controller::Controller()
{
    std::vector<double> res(100);

    std::iota(res.begin(), res.end(), 200);
    model.T = res;
    std::iota(res.begin(), res.end(), 200);
    model.n = res;
    std::iota(res.begin(), res.end(), -300);
    model.p = res;
    std::iota(res.begin(), res.end(), 1000);
    model.NdPlus = res;
    std::iota(res.begin(), res.end(), -1100);
    model.NaMinus = res;
    std::iota(res.begin(), res.end(), 10000);
    model.mue = res;
    std::iota(res.begin(), res.end(), -10100);
    model.muh = res;
    std::iota(res.begin(), res.end(), 0);
    model.sigma = res;
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
