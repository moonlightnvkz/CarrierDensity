#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>
#include <iosfwd>
#include <QStringList>
#include "model.h"

class Controller
{
public:
    static Controller &GetInstance();

    static void Init();

    bool LoadFromASCII(std::istream &is);

    bool SaveToASCII(std::ostream &os);

    bool LoadMobility();

    void Recalculate();

    void UpdateT(double from, double to);

    void UpdateMaterial(int idx);

    void UpdateEd(double value);

    void UpdateEa(double value);

    void UpdateNd0(double value);

    void UpdateNa0(double value);

    void UpdateEg(double value);

    void UpdateMe(double value);

    void UpdateMh(double value);

    double GetEd() const;

    double GetNd0() const;

    double GetEa() const;

    double GetNa0() const;

    double GetEg() const;

    double GetMe() const;

    double GetMh() const;

    const std::vector<double> &GetTemperature();

    const std::vector<double> &GetElectronsConcentration();

    const std::vector<double> &GetHolesConcentration();

    const std::vector<double> &GetDonorsConcentration();

    const std::vector<double> &GetAcceptorsConcentration();

    const std::vector<double> &GetElectronsMobility();

    const std::vector<double> &GetHolesMobility();

    const std::vector<double> &GetConductivity();

private:
    Controller();

    Model model;

    bool inited = false;

    bool CallMobility(QStringList args, QByteArray &data);
};

#endif // CONTROLLER_H
