#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>
#include <QStringList>
#include "model.h"

class Controller
{
public:
    static Controller &GetInstance();

    static void Init();

    void LoadSettings();

    void SaveSettings();

    bool LoadFromASCII(QTextStream &is);

    bool SaveToASCII(QTextStream &os);

    bool LoadMobility();

    void Recalculate();

    void UpdateT(double from, double to);

    void UpdateMaterial(const QString &name);

    void UpdateEd(double value);

    void UpdateEa(double value);

    void UpdateNd0(double value);

    void UpdateNa0(double value);

    const QString &GetMaterial() const;

    QStringList GetAllMaterials() const;

    double GetEd() const;

    double GetNd0() const;

    double GetEa() const;

    double GetNa0() const;

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

    void LoadSettingsDefault();

    static constexpr const char *SettingsFile = "/presets.ini";
};

#endif // CONTROLLER_H
