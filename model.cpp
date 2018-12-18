#include <ostream>
#include <istream>
#include "model.h"

#define Elvis(container, i, value) container.size() > i ? container[i] : value

Model::Model() { }

bool Model::Serialize(QTextStream &os) const
{
    const Preset &pr = presets.at(currentPreset);
    os << pr.Eg << " " << pr.me << " " << pr.mh << " " << pr.ae << " " << pr.be << " " << pr.ah << " " << pr.bh
       << " " << Ed << " " << Nd0 << " " << Ea << " " << Na0 << " " << currentPreset << "\n";

    for (size_t i = 0; i < T.size(); ++i) {
        double T_ = T[i];
        double mue_ = Elvis(mue, i, 0.0);
        double muh_ = Elvis(muh, i, 0.0);
        double Nc_ = Elvis(Nc, i, 0.0);
        double Nv_ = Elvis(Nv, i, 0.0);
        double n_ = Elvis(n, i, 0.0);
        double p_ = Elvis(p, i, 0.0);
        double sigma_ = Elvis(sigma, i, 0.0);
        os << T_ << "\t" <<  mue_ << "\t" << muh_ << "\t" << Nc_ << "\t"
           << Nv_ << "\t" << n_ << "\t" << p_ << "\t" << sigma_ << "\n";
    }
    return os.status() == QTextStream::Status::Ok;
}

bool Model::Deserialize(QTextStream &is)
{
    QString preset_;
    double Eg, me, mh, ae, be, ah, bh;
    is >> Eg  >> me >> mh >> ae >> be >> ah >> bh >> Ed >> Nd0 >> Ea >> Na0 >> preset_;

    currentPreset = preset_;
    presets[currentPreset] = {Eg, me, mh, ae, be, ah, bh};

    for (size_t i = 0; i < T.size(); ++i) {
        is >> T[i] >> mue[i] >> muh[i] >> Nc[i] >> Nv[i] >> n[i] >> p[i] >> sigma[i];
    }
    return is.status() == QTextStream::Status::Ok;;
}

//https://ru.wikipedia.org/wiki/%D0%AD%D1%84%D1%84%D0%B5%D0%BA%D1%82%D0%B8%D0%B2%D0%BD%D0%B0%D1%8F_%D0%BC%D0%B0%D1%81%D1%81%D0%B0
void Model::LoadPreset(QString preset)
{
    currentPreset = preset;
}


bool Model::LoadMobility(std::istream &is)
{
    return true;
}
