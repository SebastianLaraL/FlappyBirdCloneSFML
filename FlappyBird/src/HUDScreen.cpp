#include "HUDScreen.hpp"
#include "ScoreNumber.hpp"

namespace FlappyBirdGame
{

	HUDScreen::HUDScreen(const sf::Vector2u windowSize)
	{
		score_ = Add<ScoreNumber>(
			sf::Vector2f{ windowSize.x / 2.f, 58.f }
		);
	}

	void HUDScreen::SetScore(const u32 score)
	{
		score_->SetValue(score);
	}

}
