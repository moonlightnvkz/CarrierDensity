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
    double A = exp(-Eg/k/T);
    double B = exp(Ea/k/T);
    double C = exp((Eg - Ed)/k/T);
    double D = Nd0;
    double R = Na0;
    qDebug() << "Nv*B*C " << Nv*B*C;
    double ln1 = log(Nv*B*C);
    qDebug() << "Nv*(B+C) " << Nv*(B+C);
    double ln2 = log(Nv*(B+C));
    qDebug() << "D*B + Nv - R*C - Nc*A*B*C " << D*B + Nv - R*C - Nc*A*B*C;
    double ln3 = log(D*B + Nv - R*C - Nc*A*B*C);
    qDebug() << "D - Nc*A*(B+C) " << D - Nc*A*(B+C);
    double ln4 = log(D - Nc*A*(B+C));
    qDebug() << "Nc*A " << Nc*A;
    double ln5 = log(Nc*A);
    qDebug() << "6";
    return -(k*T/10)*(ln5 - ln4 - ln3 - ln2 - ln1);

}

double Formulas::FindConductivity(double muE, double muP, double T, double Na0, double Nd0, double Eg, double Ea, double Ed) {
    double Nc = CalcEffectiveDensityState(T);
    double Nv = CalcEffectiveDensityState(T);
    double Ef = CalcFermiLevel(Nc, Nv, T, Na0, Nd0, Eg, Ea, Ed);
    double n = CalcN(Nc, Ef, T);
    double p = CalcP(Nv, Ef, T);
    return e * (n*muE + p*muP);
}


