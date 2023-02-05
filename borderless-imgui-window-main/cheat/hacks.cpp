#include "hacks.h"
#include "gui.h"
#include "globals.h"
#include "vector.h"

#include<thread>
#include <Windows.h>
#include <iostream>


constexpr Vector3 CalculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

void hacks::VisualsThread(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientaddress + offsets::dwLocalPlayer);

		if (!localPlayer)
				continue;

		const auto glowManager = mem.Read<std::uintptr_t>(globals::clientaddress + offsets::dwGlowObjectManager);

		if (!glowManager)
			continue;

		const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

		for (auto i = 1; i <= 32; ++i)
		{
			const auto player = mem.Read<std::uintptr_t>(globals::clientaddress + offsets::dwEntityList + i * 0x10);

			if (!player)
				continue;

			const auto team = mem.Read<std::uintptr_t>(player + offsets::m_iTeamNum);

			if (team == localTeam)
				continue;

			const auto lifeState = mem.Read<std::int32_t>(player + offsets::m_lifeState);

			if (lifeState != 0)
				continue;

			if (globals::glow)
			{
				const auto glowIndex = mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

				mem.Write(glowManager + (glowIndex * 0x38) + 0x8, globals::glowColor[0]);
				mem.Write(glowManager + (glowIndex * 0x38) + 0xC, globals::glowColor[1]);
				mem.Write(glowManager + (glowIndex * 0x38) + 0x10, globals::glowColor[2]);
				mem.Write(glowManager + (glowIndex * 0x38) + 0x14, globals::glowColor[3]);

				mem.Write(glowManager + (glowIndex * 0x38) + 0x28, true);
				mem.Write(glowManager + (glowIndex * 0x38) + 0x29, false);
			}


		}

	}
}

void hacks::MiscThread(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientaddress + offsets::dwLocalPlayer);

		if (globals::bhops)
		{
			const auto localPlayerFlags = mem.Read<std::uintptr_t>(localPlayer + offsets::m_fFlags);

			// bhop
			if (GetAsyncKeyState(VK_SPACE))
				(localPlayerFlags & (1 << 0)) ?
				mem.Write<std::uintptr_t>(globals::clientaddress + offsets::dwForceJump, 6) :
				mem.Write<std::uintptr_t>(globals::clientaddress + offsets::dwForceJump, 4);
		}

		const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

		const auto localEyePosition = mem.Read<Vector3>(localPlayer + offsets::m_vecOrigin) +
			mem.Read<Vector3>(localPlayer + offsets::m_vecViewOffset);

		const auto clientState = mem.Read<std::uintptr_t>(globals::engineaddress + offsets::dwClientState);

		const auto localPlayerId =
			mem.Read<std::int32_t>(clientState + offsets::dwClientState_GetLocalPlayer);

		const auto viewAngles = mem.Read<Vector3>(clientState + offsets::dwClientState_ViewAngles);
		const auto aimPunch = mem.Read<Vector3>(localPlayer + offsets::m_aimPunchAngle) * 2;

		auto bestFov = globals::aimbotfov;
		auto bestAngle = Vector3{ };

		for (auto i = 1; i <= 32; ++i)
		{
			const auto player = mem.Read<std::uintptr_t>(globals::clientaddress + offsets::dwEntityList + i * 0x10);

			if (mem.Read<std::int32_t>(player + offsets::m_iTeamNum) == localTeam)
				continue;

			if (mem.Read<bool>(player + offsets::m_bDormant))
				continue;

			if (mem.Read<std::int32_t>(player + offsets::m_lifeState))
				continue;

			if (mem.Read<std::int32_t>(player + offsets::m_bSpottedByMask) & (1 << localPlayerId))
			{
				const auto boneMatrix = mem.Read<std::uintptr_t>(player + offsets::m_dwBoneMatrix);

				// pos of player head in 3d space
				// 8 is the head bone index :)
				const auto playerHeadPosition = Vector3{
					mem.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
					mem.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
					mem.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
				};

				const auto angle = CalculateAngle(
					localEyePosition,
					playerHeadPosition,
					viewAngles + aimPunch
				);

				const auto fov = std::hypot(angle.x, angle.y);

				if (fov < bestFov)
				{
					bestFov = fov;
					bestAngle = angle;
				}
			}
		}

		if (globals::aimbot)
		{
			if (!GetAsyncKeyState(VK_LBUTTON))
				continue;
			// if we have a best angle, do aimbot
			if (!bestAngle.IsZero())
				mem.Write<Vector3>(clientState + offsets::dwClientState_ViewAngles, viewAngles + bestAngle / globals::aimbotsmooth); // smoothing
		}

	}
}