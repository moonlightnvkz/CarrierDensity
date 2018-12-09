#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <iosfwd>

struct Model {
    enum class Preset {
        Ge,
        Si,
        GaAs
    };

    double Eg = 0;
    double me = 0;
    double mh = 0;
    double Ed = 0;
    double Nd0 = 0;
    double Ea = 0;
    double Na0 = 0;

    std::vector<double> T;
    // For each T
    std::vector<double> mue;
    std::vector<double> muh;
    std::vector<double> Nc;
    std::vector<double> Nv;
    std::vector<double> NdPlus;
    std::vector<double> NaMinus;
    std::vector<double> n;
    std::vector<double> p;
    std::vector<double> sigma;

    bool Serialize(std::ostream &os) const;

    bool Deserialize(std::istream &is);

    void LoadPreset(Preset preset);
};

#endif // MODEL_H
