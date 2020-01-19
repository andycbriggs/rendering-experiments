#pragma once

const static float PI = 3.141592654f;

constexpr inline float toRadians(float degrees) { return degrees * PI / 180.f; }
constexpr inline float toDegrees(float radians) { return radians * 180.f / PI; }