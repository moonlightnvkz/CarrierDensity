#include <cmath>
#include "formulas.h"
#include "model.h"

double Formulas::CalcEffectiveDensityState(double mcv, double T){
    return 2.51 * pow(10, 19) * pow((mcv*T/300), 1.5) * 1e-6;
}

double Formulas::CalcN(double Nc, double Ef, double T) {
    return Nc * exp((Ef-Eg)/(k*T));
}

double Formulas::CalcP(double Nv, double Ef, double T){
    return Nv * exp((-Ef)/(k*T));
}

double Formulas::CalcFermiLevel(double Nc, double Nv, double T, double Nd0, double Eg, double Ea, double Ed) {
    double c1  = Nd0 - Nc;
    double c21 = Nd0 * exp(Ea/k/T);
    double c22  = exp(Ea / k / T);
    double c23  = exp(Eg - Ed);
    double c2 = c21 - Nc*(c22+c23);
    double c3 = Nv*(1+exp(Ea/k/T)+exp((Eg-Ed)/k/T)+exp((Eg-Ed+Ea)/k/T)) - Nc*exp((Eg-Ed+Ea)/k/T);
    return k * T / 3 * (std::log(c1) + std::log(c2) + std::log(c3));
}

double Formulas::FindConductivity(double muE, double muP, double T, double Nd0, double Eg, double Ea, double Ed) {
    double Nc = CalcEffectiveDensityState(T);
    double Nv = CalcEffectiveDensityState(T);
    double Ef = CalcFermiLevel(Nc, Nv, T, Nd0, Eg, Ea, Ed);
    double n = CalcN(Nc, Ef, T);
    double p = CalcP(Nv, Ef, T);
    return e * (n*muE + p*muP);
}


