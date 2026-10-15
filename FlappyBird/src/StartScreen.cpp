#include "StartScreen.hpp"
#include "LabelElement.hpp"
#include "ScoreNumber.hpp"

namespace FlappyBirdGame
{

	StartScreen::StartScreen(const sf::Vector2u windowSize) : centerX_(windowSize.x / 2.f)
	{
		title_ = Add<LabelElement>("FLAPPY BIRD", 54, sf::Vector2f{ centerX_, TITLE_Y });
		Add<LabelElement>("CLICK TO START", 24, sf::Vector2f{ centerX_, 420.f });
		Add<LabelElement>("CLICK TO FLAP", 24, sf::Vector2f{ centerX_, 455.f });

		// Top score, top right corner.
		const f32 topScoreX = windowSize.x - 50.f;
		Add<LabelElement>("BEST", 18, sf::Vector2f{ topScoreX, 20.f });
		topScore_ = Add<ScoreNumber>(
			sf::Vector2f{ topScoreX, 54.f }
		);
	}

	void StartScreen::SetBob(const f32 offsetY)
	{
		title_->SetCenter({ centerX_, TITLE_Y + offsetY });
	}

	void StartScreen::SetTopScore(const u32 score)
	{
		topScore_->SetValue(score);
	}

}
