#pragma once

#include "Light.hpp"

class SpotLight : public Light
{
public:
	Vector3 mDirection = Vector3::Zero;
	Vector3 mTranslation = Vector3::Zero;
	float mRadius = 100.f;
    float mInnerAngle = toRadians(40.f);
    float mOuterAngle = toRadians(90.f);
};