#include "CameraComponent.hpp"

#include "Engine.hpp"
#include "Timer.hpp"

void CameraComponent::OnInitialise()
{
    UpdateProjectionMatrix();
}

void CameraComponent::OnUpdate(const TimerTime& time)
{
	UpdateViewMatrix();
}

void CameraComponent::OnWindowSizeChanged()
{
    UpdateProjectionMatrix();
}

void CameraComponent::UpdateViewMatrix()
{
	Vector3 globalRotation = GetGlobalRotation();
	mViewMatrix = Matrix::CreateTranslation(GetGlobalTranslation()).Invert() * Matrix::CreateFromYawPitchRoll(globalRotation.x, globalRotation.y, globalRotation.z).Transpose();
	
	UpdateViewProjectionMatrix();
}

void CameraComponent::UpdateProjectionMatrix()
{
    mProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(mFieldOfView, Engine::Get()->GetViewport().AspectRatio(), mNearPlaneDistance, mFarPlaneDistance);

	UpdateViewProjectionMatrix();
}

void CameraComponent::UpdateViewProjectionMatrix()
{
	mViewProjectionMatrix = mViewMatrix * mProjectionMatrix;
}

const Matrix& CameraComponent::GetViewProjectionMatrix() const
{
	return mViewProjectionMatrix;
}