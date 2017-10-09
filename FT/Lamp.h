#ifndef __LAMP_HEADER__
#define __LAMP_HEADER__ 1

#include "GameObject.h"

class Lamp : public GameObject {
private:
    /* data */

public:
    Lamp ();

    void update(GLFWwindow* window, float deltaTime);
};

#endif
