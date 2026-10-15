#pragma once
#include "RenderOrder.hpp"

namespace FlappyBirdGame
{
	namespace RenderOrder
	{
		inline constexpr u32 FLOOR = ::RenderOrder::GAMEPLAY + 1; // A bit deeper than normal gameplay elements.
		inline constexpr u32 OBSTACLES = FLOOR + 1; // Obstacles are behind the floor.
	}
}
