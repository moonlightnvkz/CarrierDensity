#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <iosfwd>

struct Model {
    enum class Preset {
        Ge,
        Si,
        GaAs,
        Custom
    };

    // Apriori
    double Eg = 0;
    double me = 0;
    double mh = 0;

    // UI
    double Ed = 0.647;
    double Nd0 = 5.5e13;
    double Ea = 0;
    double Na0 = 0;
    Preset preset;

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

    bool Serialize(std::ostream &os) const;

    bool Deserialize(std::istream &is);

    void LoadPreset(Preset preset);

    bool LoadMobility(std::istream &is);
};

#endif // MODEL_H
