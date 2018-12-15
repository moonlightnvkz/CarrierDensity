#include <cmath>
#include "formulas.h"
#include "model.h"
#include <QDebug>

double Formulas::CalcEffectiveDensityState(double mcv, double T){
    return 2.51e19 * pow((mcv*T/300.0), 1.5);
}

double Formulas::CalcN(double Nc, double Eg, double Ef, double T) {
    return Nc * exp((Ef-Eg)/(k*T));
}

double Formulas::CalcP(double Nv, double Ef, double T){
    return Nv * exp((-Ef)/(k*T));
}

double Formulas::CalcFermiLevel(double Nc, double Nv, double T, double Na0, double Nd0, double Eg, double Ea, double Ed) {
    double left = 0;
    double right = Eg;
    auto func = [=](double Ef) {
        double n = Formulas::CalcN(Nc, Eg, Ef, T);
        double p = Formulas::CalcP(Nv, Ef, T);
        double NdPlus = Formulas::CalcNdPlus(T, Ef, Ed, Nd0);
        double NaMinus = Formulas::CalcNaMinus(T, Ef, Ea, Na0);
        return NdPlus + p - n - NaMinus;
    };

    double middle = (left + right) / 2.0;
    double fm = func(middle);
    size_t iters = 0;
    while(abs(fm) > 1 && iters < 1000) {
        double fleft = func(left);
        double fright = func(right);
        if (fleft * fm < 0) {
            right = middle;
        } else if (fright * fm < 0) {
            left = middle;
        } else {    // there is no zero
            qDebug() << "No zeros found";
            break;
        }
        middle = (left + right) / 2.0;
        fm = func(middle);
        ++iters;
    }
//    qDebug() << "NdPlus + p - n - NaMinus = " << fm;
//    qDebug() << "Ef = " << middle;
    return middle;
//    qDebug() << Nc << " " << Nv << " " << T << " " << Na0 << " " << Nd0 << " " << Ec << " " << Ea << " " << Ed;
//    qDebug() << k*T << " " << Ed /k/T;
//    double A = Na0;
//    double D = Nd0;
//    double C = Nc;
//    double V = Nv;
//    double B = C*exp(Ec/k/T);
//    double K = 4*exp(Ea/k/T);
//    double F = V*exp(-Ec*k*T);
//    double E = 0.5*exp(-Ed/k/T);
//    qDebug() << A << " " << B << " " << C << " " << D << " " << E << " " << F << " " << K << " " << V;
//    qDebug() << log(B) << " " << log(K) << " " << log(F) << " " << log(A) << " " << log(D) << " " << log(B*E + A + F*K - D);
//    return (k*T/4)*(2*log(B) - log(K) - 3*log(F) + log(A) - log(D) + log(B*E + A + F*K - D));
}

double Formulas::CalcNaMinus(double T, double Ef, double Ea, double Na0){
    return Na0/(1 + 1.0*exp((Ea - Ef)/(k*T)));
}

double Formulas::CalcNdPlus(double T, double Ef, double Ed, double Nd0){
    return Nd0/(1 + 1.0*exp((Ef- Ed)/(k*T)));
}

double Formulas::FindConductivity(double n, double p, double muE, double muP) {
    return e * (n*muE + p*muP);
}


