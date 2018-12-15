#pragma once

class Formulas final {
public:
    Formulas() = delete;

    static double CalcEffectiveDensityState(double mcv, double T);

    static double CalcN(double Nc, double Eg, double Ef, double T);

    static double CalcP(double Nv, double Ef, double T);

    static double CalcNdPlus(double T, double Ef, double Ed, double Nd0);

    static double CalcNaMinus(double T, double Ef, double Ea, double Na0);

    static double CalcFermiLevel(double Nc, double Nv, double T, double Na0, double Nd0, double Eg, double Ea, double Ed);

    static double FindConductivity(double n, double p, double muE, double muP);

private:
    static constexpr const double k = 8.6173303e-05;  // Boltzmann constant, eV / K
//    static constexpr const double k  = 1.380648 * 1e-23;    // SI
    static constexpr const double e  = 1.60217662e-19 * 1e-19;
};
