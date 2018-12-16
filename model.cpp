#include <ostream>
#include <istream>
#include "model.h"

#define Elvis(container, i, value) container.size() > i ? container[i] : value

Model::Model()
{
    LoadPreset(Preset::Ge);
}

bool Model::Serialize(std::ostream &os) const
{
    os << Eg << " " << me << " " << mh << " " << Ed << " " << Nd0 << " " << Ea << " " << Na0 << " "
       << static_cast<int>(preset) << std::endl;
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
    int preset_;
    is >> Eg  >> me >> mh >> Ed >> Nd0 >> Ea >> Na0 >> preset_;
    preset = static_cast<Preset>(preset_);
    for (size_t i = 0; i < T.size(); ++i) {
        is >> T[i] >> mue[i] >> muh[i] >> Nc[i] >> Nv[i] >> n[i] >> p[i] >> sigma[i];
    }
    return is.good();
}

//https://ru.wikipedia.org/wiki/%D0%AD%D1%84%D1%84%D0%B5%D0%BA%D1%82%D0%B8%D0%B2%D0%BD%D0%B0%D1%8F_%D0%BC%D0%B0%D1%81%D1%81%D0%B0
void Model::LoadPreset(Model::Preset preset_)
{
    preset = preset_;
    switch (preset) {
    case Preset::Ge:
       Eg = 0.661;
       me = 0.22;
       mh = 0.34;
       break;
    case Preset::Si:
       Eg = 1.12;
       me = 1.08;
       mh = 0.56;
       break;
    case Preset::GaAs:
       Eg = 1.42;
       me = 0.067;
       mh = 0.45;
       break;
    default:
        break;
    }
}


bool Model::LoadMobility(std::istream &is)
{
    return true;
}
