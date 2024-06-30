#pragma once
#include <numbers>
#include <cmath>
#include <Windows.h>

struct ViewMatrix_t
{
	float* operator[](int idx)
	{
		return matrix[idx];
	}

	float matrix[4][4];
};

class CVector
{
public:
	constexpr CVector(const float x = 0.f, const float y = 0.f, const float z = 0.f) noexcept : x(x), y(y), z(z) { }

	constexpr const CVector& operator-(const CVector& other) const noexcept;
	constexpr const CVector& operator+(const CVector& other) const noexcept;
	constexpr const CVector& operator/(const float factor) const noexcept;
	constexpr const CVector& operator*(const float factor) const noexcept;
	bool IsZero();

	float x, y, z;
};

namespace math
{
	CVector W2S(ViewMatrix_t vm, CVector& in);
}