#ifndef __COW_HEADER__
#define __COW_HEADER__ 1

#include "GameObject.h"

class Cow : public GameObject {
private:

public:
    Cow (ResourcesManager* tm);
    Cow (ResourcesManager* tm, float size);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
