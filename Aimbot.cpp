#include "Aimbot.h"
#include "Main.h"
#include "utils/Offsets.h"

// Nicht von mir weiss aber nicht von wem ichs hab.
CVector ClosestPlayer(const std::vector<CVector> playerPositions)
{
	if (playerPositions.empty()) {
		return { 0.0f, 0.0f, 0.0f };
	}

	CVector center_of_screen{ (float)GetSystemMetrics(0) / 2, (float)GetSystemMetrics(1) / 2, 0.0f };

	float lowestDistance = 10000;

	int index = -1;

	for (int i = 0; i < playerPositions.size(); ++i) {
		float distance(std::pow(playerPositions[i].x - center_of_screen.x, 2) + std::pow(playerPositions[i].y - center_of_screen.y, 2));

		if (distance < lowestDistance) {
			lowestDistance = distance;
			index = i;
		}
	}

	if (index == -1) {
		return { 0.0f, 0.0f, 0.0f };
	}

	return { playerPositions[index].x, playerPositions[index].y, 0.0f };
}

void MoveToPos(CVector pos)
{
	if (pos.IsZero())
		return;

	std::pair<char, char> data = std::make_pair(static_cast<char>(pos.x - GetSystemMetrics(0) / 2), static_cast<char>(pos.y - GetSystemMetrics(1) / 2));
	arduino->SendCmd({ data.first, data.second });
}

void aim::DoAim()
{
	playerPositions.clear();

	uintptr_t entityList = mem->Read<uintptr_t>(clientModule + offset::dwEntityList);
	uintptr_t localPlayerPtr = mem->Read<uintptr_t>(clientModule + offset::dwLocalPlayerPawn);

	ViewMatrix_t viewMatrix = mem->Read<ViewMatrix_t>(clientModule + offset::dwViewMatrix);

	for (int i = 0; i < 64; ++i)
	{
		uintptr_t listEntry = mem->Read<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);
		uintptr_t playerController = mem->Read<uintptr_t>(listEntry + 120 * (i & 0x1FF));
		uint32_t playerPawn = mem->Read<uint32_t>(playerController + offset::m_hPawn);
		uintptr_t listEntry2 = mem->Read<uintptr_t>(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
		uintptr_t playerPawnPtr = mem->Read<uintptr_t>(listEntry2 + 120 * (playerPawn & 0x1FF));

		int health = mem->Read<int>(playerPawnPtr + offset::m_iHealth);
		if (health <= 0 || health > 100)
			continue;

		int targetTeam = mem->Read<int>(playerPawnPtr + offset::m_iTeamNum);
		if (targetTeam == mem->Read<int>(localPlayerPtr + offset::m_iTeamNum))
			continue;

		CVector playerPosition = mem->Read<CVector>(playerPawnPtr + offset::m_vOldOrigin);

		uintptr_t spotted = mem->Read<uintptr_t>(playerPawnPtr + offset::m_entitySpottedState);

		if (!mem->Read<bool>(spotted + 0x8))
			continue;

		// Verdammt ghetto aber egal
		CVector headPos = { playerPosition.x, playerPosition.y, playerPosition.z += 66.0f };

		if (CVector pos = math::W2S(viewMatrix, headPos); !pos.IsZero())
			playerPositions.push_back(pos);
	}

	if (GetAsyncKeyState(VK_XBUTTON2)) // Seten Maus Taste kann personalisiert werden. 
		MoveToPos(ClosestPlayer(playerPositions));
}