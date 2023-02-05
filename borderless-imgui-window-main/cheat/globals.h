#pragma once
#include <cstdint>
#include <cstddef>


struct Color
{
	std::uint8_t r{ }, g{ }, b{ };
};


namespace globals
{
	inline std::uintptr_t clientaddress = 0;
	inline std::uintptr_t engineaddress = 0;

	inline bool glow = false;
	inline float glowColor[] = { 1.f, 0.f, 0.f, 1.f };

	inline bool aimbot = false;
	inline float aimbotsmooth = 250.f;
	inline float aimbotfov = 180.f;

	inline bool bhops = false;

	inline bool minesweeper = false;

}





namespace offsets
{
	constexpr ::std::ptrdiff_t dwForceJump = 0x52BBC7C;
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA964;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DFFEF4;
	constexpr ::std::ptrdiff_t dwClientState = 0x59F19C;
	constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D90;
	constexpr ::std::ptrdiff_t dwClientState_GetLocalPlayer = 0x180;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x535A9C8;
	constexpr ::std::ptrdiff_t model_ambient_min = 0x5A1194;
	constexpr ::std::ptrdiff_t m_clrRender = 0x70;
	constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
	constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;
	constexpr ::std::ptrdiff_t m_bDormant = 0xED;
	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
	constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
	constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
	constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x303C;
	constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
	constexpr ::std::ptrdiff_t m_fFlags = 0x104;
}
