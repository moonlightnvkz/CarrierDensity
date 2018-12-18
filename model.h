#ifndef MODEL_H
#define MODEL_H
#include <QStringList>
#include <QTextStream>
#include <map>
#include <vector>
#include <iosfwd>

struct Model {
    struct Preset {
        double Eg;
        double me;
        double mh;
        double ae;
        double be;
        double ah;
        double bh;
    };

    std::map<QString, Preset> presets;

    // UI
    double Ed = 0.647;
    double Nd0 = 5.5e13;
    double Ea = 0;
    double Na0 = 0;
    QString currentPreset;

    static constexpr const int TCount = 1000;

    // UI
    std::vector<double> T;

    // Calculated for each T
    std::vector<double> mue;
    std::vector<double> muh;
    std::vector<double> Nc;
    std::vector<double> Nv;
    std::vector<double> NdPlus;
    std::vector<double> NaMinus;
    std::vector<double> n;
    std::vector<double> p;
    std::vector<double> sigma;

    Model();

    bool Serialize(QTextStream &os) const;

    bool Deserialize(QTextStream &is);

    void LoadPreset(QString preset);

    bool LoadMobility(std::istream &is);
};

#endif // MODEL_H
