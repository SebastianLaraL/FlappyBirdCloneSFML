#pragma once
#include <SFML/Graphics.hpp>
#include "UIScreen.hpp"

namespace FlappyBirdGame
{
	class ScoreNumber;
	class ButtonElement;

	// Shown once the bird has landed: score, best score and the Play button.
	class GameOverScreen : public UIScreen
	{
	public:
		explicit GameOverScreen(const sf::Vector2u windowSize);

		void SetScores(const u32 score, const u32 best);

		// 'point' is in window pixels, which is the same space the UI is drawn in.
		bool IsPlayButtonAt(const sf::Vector2f point) const;

	private:
		ScoreNumber* score_ = nullptr;
		ScoreNumber* best_ = nullptr;
		ButtonElement* playButton_ = nullptr;
	};
}
