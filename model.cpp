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
        double NdPlus_ = Elvis(NdPlus, i, 0.0);
        double NaMinus_ = Elvis(NaMinus, i, 0.0);
        double n_ = Elvis(n, i, 0.0);
        double p_ = Elvis(p, i, 0.0);
        double sigma_ = Elvis(sigma, i, 0.0);
        os << T_ << "\t" <<  mue_ << "\t" << muh_ << "\t" << NdPlus_ << "\t"
           << NaMinus_ << "\t" << n_ << "\t" << p_ << "\t" << sigma_ << "\n";
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
        is >> T[i] >> mue[i] >> muh[i] >> NdPlus[i] >> NaMinus[i] >> n[i] >> p[i] >> sigma[i];
    }
    return is.status() == QTextStream::Status::Ok;;
}
