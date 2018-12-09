#include <ostream>
#include <istream>
#include "model.h"

#define Elvis(container, i, value) container.size() > i ? container[i] : value

bool Model::Serialize(std::ostream &os) const
{
    os << Eg << " " << me << " " << mh << " " << Ed << " " << Nd0 << " " << Ea << " " << Na0 << std::endl;
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
           << Nv_ << "\t" << n_ << "\t" << p_ << "\t" << sigma_ << std::endl;
    }
    return os.good();
}

bool Model::Deserialize(std::istream &is)
{
    is >> Eg  >> me >> mh >> Ed >> Nd0 >> Ea >> Na0;
    for (size_t i = 0; i < T.size(); ++i) {
        is >> T[i] >> mue[i] >> muh[i] >> Nc[i] >> Nv[i] >> n[i] >> p[i] >> sigma[i];
    }
    return is.good();
}

// TODO: implement LoadPreset
void Model::LoadPreset(Model::Preset preset)
{
    (void)preset;
}
