#pragma once

#include "Component.hpp"
#include "Math.hpp"

struct TimerTime;

using namespace DirectX::SimpleMath;

class CameraComponent : public Component
{
public:
    virtual void OnInitialise() override;
    virtual void OnUpdate(const TimerTime& time) override;
    virtual void OnWindowSizeChanged() override;

    const Matrix& GetViewProjectionMatrix() const;

private:
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void UpdateViewProjectionMatrix();

protected:
    float mFieldOfView = toRadians(50.f);
    float mNearPlaneDistance = 0.01f;
    float mFarPlaneDistance = 10000.f;

private:
    Matrix mViewMatrix;
    Matrix mProjectionMatrix;
    Matrix mViewProjectionMatrix;
};