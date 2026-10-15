#pragma once
#include <SFML/Graphics.hpp>
#include "UIScreen.hpp"

namespace FlappyBirdGame
{
	class LabelElement;
	class ScoreNumber;

	// Shown when the game is launched (and after pressing Play): title, hint and top score.
	class StartScreen : public UIScreen
	{
	public:
		explicit StartScreen(const sf::Vector2u windowSize);

		// The world gives the title the same offset it gives the bird, so both bob together.
		void SetBob(const f32 offsetY);
		void SetTopScore(const u32 score);

	private:
		static constexpr f32 TITLE_Y = 130.f;

		f32 centerX_;
		LabelElement* title_ = nullptr;
		ScoreNumber* topScore_ = nullptr;
	};
}
