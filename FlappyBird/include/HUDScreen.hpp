#pragma once
#include <SFML/Graphics.hpp>
#include "UIScreen.hpp"

namespace FlappyBirdGame
{
	class ScoreNumber;

	class HUDScreen : public UIScreen
	{
	public:
		explicit HUDScreen(const sf::Vector2u windowSize);

		void SetScore(const u32 score);

	private:
		ScoreNumber* score_ = nullptr;
	};
}
