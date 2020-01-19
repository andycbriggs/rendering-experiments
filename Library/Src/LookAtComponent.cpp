#include "LookAtComponent.hpp"

#include "Timer.hpp"

using namespace std;

static void toEulerAngle(const Quaternion& q, float& roll, float& pitch, float& yaw)
{
	// roll (x-axis rotation)
	float sinr_cosp = +2.0f * (q.w * q.x + q.y * q.z);
	float cosr_cosp = +1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	roll = atan2f(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = +2.0f * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		pitch = copysign(PI / 2, sinp); // use 90 degrees if out of range
	else
		pitch = asinf(sinp);

	// yaw (z-axis rotation)
	float siny_cosp = +2.0f * (q.w * q.z + q.x * q.y);
	float cosy_cosp = +1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	yaw = atan2f(siny_cosp, cosy_cosp);
}

void LookAtComponent::OnUpdate(const TimerTime& time)
{
	if (GetGlobalTranslation() == mLookAt)
		return;

	const Vector3 translation = GetGlobalTranslation();

	// update position for look at
	Matrix m = Matrix::CreateLookAt(translation, mLookAt, Vector3::Up);
	Quaternion q = Quaternion::CreateFromRotationMatrix(m);

	//float x, y, z;
	//toEulerAngle(q, x, y, z);

	// decompose rotation matrix
	//float y = atan2f(m._32, m._33);
	//float x = atan2f(-m._31, sqrtf((m._23*m._23) + (m._33*m._33)));
	//float z = 0.f;// atan2f(m._21, m._11);

	Vector3 distance = (GetGlobalTranslation() - mLookAt);

	// adjx = 
	//float x = asin(distance.x / distance.Length());
	//float y = asin(distance.y / distance.Length());
	//float z = asin(distance.z / distance.Length());

	float x = atan2f(translation.x - mLookAt.x, translation.z - mLookAt.z);
	float y = atan2f(translation.y - mLookAt.y, translation.x - mLookAt.x);
	float z = atan2f(translation.z - mLookAt.z, translation.y - mLookAt.y);

	std::wstringstream ss;
	ss << z << std::endl;
	OutputDebugString(ss.str().c_str());

	SetRotation({ clamp(x, -PI, PI), clamp(z, -PI, PI), 0.f });
}

void LookAtComponent::SetLookAt(const Vector3& lookAt)
{
	mLookAt = lookAt;
}
