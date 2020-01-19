#pragma once

#include "Light.hpp"

class PointLight : public Light
{
public:
    float mRadius = 10.f;
    Vector3 mTranslation;
};