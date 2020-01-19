#pragma once

#include "Light.hpp"

class DirectionalLight : public Light
{
public:
    Vector3 mDirection = Vector3::Forward;
};