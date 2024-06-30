#include "Math.h"

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

constexpr const CVector& CVector::operator-(const CVector& other) const noexcept
{
    return CVector{ x - other.x, y - other.y, z - other.z };
}

constexpr const CVector& CVector::operator+(const CVector& other) const noexcept
{
    return CVector{ x + other.x, y + other.y, z + other.z };
}

constexpr const CVector& CVector::operator/(const float factor) const noexcept
{
    return CVector{ x / factor, y / factor, z / factor };
}

constexpr const CVector& CVector::operator*(const float factor) const noexcept
{
    return CVector{ x * factor, y * factor, z * factor };
}

bool CVector::IsZero()
{
    if (x == 0 && y == 0 && z == 0)
        return true;
    return false;
}

CVector math::W2S(ViewMatrix_t vm, CVector& in)
{
    CVector out;

    out.x = vm[0][0] * in.x + vm[0][1] * in.y + vm[0][2] * in.z + vm[0][3];
    out.y = vm[1][0] * in.x + vm[1][1] * in.y + vm[1][2] * in.z + vm[1][3];

    float width = vm[3][0] * in.x + vm[3][1] * in.y + vm[3][2] * in.z + vm[3][3];

    if (width < 0.01f) {
        return out;
    }

    float inverseWidth = 1.f / width;

    out.x *= inverseWidth;
    out.y *= inverseWidth;

    float x = screenWidth / 2;
    float y = screenHeight / 2;

    x += 0.5f * out.x * screenWidth + 0.5f;
    y -= 0.5f * out.y * screenHeight + 0.5f;

    out.x = x;
    out.y = y;

    return out;
}