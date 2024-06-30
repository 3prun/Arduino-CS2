#pragma once
#include <cstddef>

// Patterns sind besser aber ist nur ein Schulprojekt.
// https://github.com/sezzyaep/CS2-OFFSETS/tree/main
namespace offset
{
	constexpr std::ptrdiff_t dwEntityList = 0x19BDE58;
	constexpr std::ptrdiff_t dwViewMatrix = 0x1A1FDB0;
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1823A08;
	constexpr std::ptrdiff_t m_hPawn = 0x5FC;
	constexpr std::ptrdiff_t m_iHealth = 0x324;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3C3;
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1274;
	constexpr std::ptrdiff_t m_entitySpottedState = 0x2288;
}