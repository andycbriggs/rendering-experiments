#pragma once

#include <Mouse.h>

#include "Component.hpp"
#include "Timer.hpp"

class WASDComponent : public Component
{
public:
	void OnUpdate(const TimerTime& time) override;

private:
	Mouse::ButtonStateTracker mMouseStateTracker;
};