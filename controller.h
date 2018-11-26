#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>
#include <iosfwd>
#include "model.h"

class Controller
{
public:
    static Controller &GetInstance();

    bool LoadFromASCII(std::istream &is);

    bool SaveToASCII(std::ostream &os);

    bool LoadMobility(std::istream &is);
private:
    Controller() = default;

    Model model;
};

#endif // CONTROLLER_H
