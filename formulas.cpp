#include <cmath>
#include "formulas.h"
#include "model.h"
#include <QDebug>

double Formulas::CalcEffectiveDensityState(double mcv, double T){
    return 2.51 * pow(10, 19) * pow((mcv*T/300), 1.5) * 1e-6;
}

double Formulas::CalcN(double Nc, double Eg, double Ef, double T) {
    return Nc * exp((Ef-Eg)/(k*T));
}

double Formulas::CalcP(double Nv, double Ef, double T){
    return Nv * exp((-Ef)/(k*T));
}

double Formulas::CalcFermiLevel(double Nc, double Nv, double T, double Na0, double Nd0, double Eg, double Ea, double Ed) {
    double A = Na0;
    double D = Nd0;
    double C = Nc;
    double V = Nv;
    double B = C*exp(Ec/k/T);
    double K = 4*exp(Ea/k/T);
    double F = V*exp(-Ec*k*T);
    double E = 0.5*exp(-Ed/k/T);
    qDebug() << "6";
    return (k*T/4)*(2*log(B) - log(K) - 3*log(F) + log(A) - log(D) + log(B*E + A + F*K - D));

}

double Formulas::CalcNdPlus(double T, double Ef, double Ed, double Nd0){
    return Nd0/(1 + 0.5*exp((Ef- Ed)/k/T))
}

double Formulas::CalcNaMinus(double T, double Ef, double Ea, double Na0){
    return Nda/(1 + 4*exp((Ea - Ef)/k/T))
}

double Formulas::FindConductivity(double muE, double muP, double T, double Na0, double Nd0, double Eg, double Ea, double Ed) {
    double Nc = CalcEffectiveDensityState(T);
    double Nv = CalcEffectiveDensityState(T);
    double Ef = CalcFermiLevel(Nc, Nv, T, Na0, Nd0, Eg, Ea, Ed);
    double n = CalcN(Nc, Ef, T);
    double p = CalcP(Nv, Ef, T);
    return e * (n*muE + p*muP);
}


