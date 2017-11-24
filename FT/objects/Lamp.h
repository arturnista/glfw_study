#ifndef __LAMP_HEADER__
#define __LAMP_HEADER__ 1

#include "GameObject.h"
#include "../vendor/json.h"

using tJson = nlohmann::json;

class Lamp : public GameObject {
private:
public:
    Lamp (ResourcesManager* tm);
};

#endif
