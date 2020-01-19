#pragma once

// DirectXTK
#include <d3d11_1.h>
#include <DirectXHelpers.h>
#include <DirectXColors.h>
#include <SimpleMath.h>

#include "Math.hpp"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// stream writers
inline std::wostream& operator<<(std::wostream& os, const Vector3& v)
{
	os << L"x: " << v.x << L", y: " << v.y << L", z: " << v.z;
	return os;
}

inline std::wostream& operator<<(std::wostream& os, const Quaternion& q)
{
	os << L"x: " << toDegrees(q.x) << L", y: " << toDegrees(q.y) << L", z: " << toDegrees(q.z) << L", w: " << toDegrees(q.w);
	return os;
}

