#include <numeric> // std::iota
#include <sstream>
#include <QProcess>
#include "controller.h"

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

bool Controller::LoadMobility()
{
    QByteArray mobility;
    if (!CallMobility({"--mobility"}, mobility)) {
        return false;
    } else {
        std::stringstream ss(mobility.toStdString());
        return model.LoadMobility(ss);
    }
}

// TODO: implement Recalculate
void Controller::Recalculate()
{

}

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
