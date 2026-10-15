#pragma once
#include "Types.hpp"

// Draw order used by the world's render pipeline. A LARGER value is drawn first (farther back).
namespace RenderOrder
{
	inline constexpr u32 TOP_UI = 1U;
	inline constexpr u32 BOTTTOM = 4294967295U; // Last possible element.
	inline constexpr u32 GAMEPLAY = BOTTTOM / 2U; // Halfway to both limits to give room for other possibilities.
	inline constexpr u32 BACKGROUND = BOTTTOM / 4U * 3U; // Between GAMEPLAY AND BOTTOM. NOTE: this value was decided quickly, if you find a more suitable value don't hesitate to change this in the final "engine".
}
