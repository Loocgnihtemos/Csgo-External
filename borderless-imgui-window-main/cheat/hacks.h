#pragma once
#include "memory.h"

namespace hacks
{
	void VisualsThread(const Memory& mem) noexcept;
	void MiscThread(const Memory& mem) noexcept;
}