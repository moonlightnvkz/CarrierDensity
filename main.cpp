#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "formulas.h"
#include "model.h"
#include "controller.h"

// Ge(T=15.6183; Jd=0.0140; Ja=0.0000; Nd=1.000e+13; Na=0.000e+00)

//static double T = 15.6183;
//static double Jd = 0.014;
//static double Ja = 0.011;
//static double Nd = 1e13;
//static double Na = Nd / 2.0;


int main(int argc, char *argv[])
{
    qDebug() << "Before init";
    Controller::Init();
    qDebug() << "Inited";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    Model model;
//    model.LoadPreset(Model::Preset::Ge);

//    double Nc = Formulas::CalcEffectiveDensityState(model.me);
//    qDebug() << Nc;
//    double Nv = Formulas::CalcEffectiveDensityState(model.mh);
//    qDebug() << Nv;
//    double Ef = Formulas::CalcFermiLevel(Nc, Nv, T, Na, Nd, model.Eg, Ja, model.Eg - Jd);
//    double NaMinus = Formulas::CalcNaMinus(T, Ef, Ja, Na);
//    double NdPlus = Formulas::CalcNdPlus(T, Ef, model.Eg - Jd, Nd);
//    qDebug() << "NaMinus = " << NaMinus << "\nNdPlus = " << NdPlus;
    return a.exec();
}
