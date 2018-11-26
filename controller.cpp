#include "controller.h"

Controller &Controller::GetInstance()
{
    static Controller instance;
    return instance;
}

bool Controller::LoadFromASCII(std::istream &is)
{
    return model.Deserialize(is);
}

bool Controller::SaveToASCII(std::ostream &os)
{
    return model.Serialize(os);
}

bool Controller::LoadMobility(std::istream &is)
{
    return model.LoadMobility(is);
}
