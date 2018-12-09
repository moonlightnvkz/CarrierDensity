#pragma once

class Formulas final {
public:
    Formulas() = delete;

    static double CalcEffectiveDensityState(double mcv, double T  = 300);

    static double CalcN(double Nc, double Eg, double Ef, double T  = 300);

    static double CalcP(double Nv, double Ef, double T  = 300);

    static double CalcFermiLevel(double Nc, double Nv, double T, double Na0, double Nd0, double Eg, double Ea, double Ed);

    static double FindConductivity(double muE, double muP, double T, double Na0, double Nd0, double Eg, double Ea, double Ed);

private:
    static constexpr const double k  = 1.380648 * 1e-23;
    static constexpr const double e  = -1.9 * 1e-19;
};
