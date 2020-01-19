#include "WASDComponent.hpp"

#include <Keyboard.h>

#include "Math.hpp"

using namespace std;

void WASDComponent::OnUpdate(const TimerTime& time)
{
	Vector3 movementAmount = Vector3::Zero;

	Keyboard::State keyboardState = Keyboard::Get().GetState();

	if (keyboardState.A)
		movementAmount.x -= 1.f;
	if (keyboardState.D)
		movementAmount.x += 1.f;
	if (keyboardState.S)
		movementAmount.z -= 1.f;
	if (keyboardState.W)
		movementAmount.z += 1.f;
	if (keyboardState.Q)
		movementAmount.y -= 1.f;
	if (keyboardState.E)
		movementAmount.y += 1.f;

	Mouse::State mouseState = Mouse::Get().GetState();

	movementAmount.z += float(mouseState.scrollWheelValue) * 0.05f;

	mMouseStateTracker.Update(mouseState);

	Vector3 rotationAmount = GetRotation();

	if (mouseState.positionMode == Mouse::Mode::MODE_RELATIVE)
	{
		if (mouseState.leftButton) // pan / tilt
		{
            rotationAmount.x += -mouseState.x * 0.01f;
            rotationAmount.y += -mouseState.y * 0.01f;
		}
		else if (mouseState.rightButton) // truck
		{
			movementAmount.x += mouseState.x * 0.5f;
			movementAmount.y += -mouseState.y * 0.5f;
		}
        else if (mouseState.middleButton) // dolly
        {
            movementAmount.z += -mouseState.y * 0.5f;
        }
	}

    rotationAmount.x = fmod(rotationAmount.x, toRadians(360.f));
    rotationAmount.y = clamp(rotationAmount.y, toRadians(-90.f), toRadians(90.f));
    rotationAmount.z = 0.f;

	if ((mMouseStateTracker.leftButton | mMouseStateTracker.middleButton | mMouseStateTracker.rightButton) == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		Mouse::Get().SetMode(Mouse::Mode::MODE_RELATIVE);
	}
	else if ((mMouseStateTracker.leftButton & mMouseStateTracker.middleButton & mMouseStateTracker.rightButton) == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		Mouse::Get().SetMode(Mouse::Mode::MODE_ABSOLUTE);
	}

	SetRotation(rotationAmount);
	SetTranslation(GetTranslation() + (GetLocalMatrix().Right() * movementAmount.x) + (GetLocalMatrix().Up() * movementAmount.y) + (GetLocalMatrix().Forward() * movementAmount.z));
}
