#pragma once

#include "Component.hpp"

struct TimerTime;

class LookAtComponent : public Component
{
public:
	void OnUpdate(const TimerTime& time) override;

	void SetLookAt(const Vector3& lookAt); // look at global position

private:
	Vector3 mLookAt;
};