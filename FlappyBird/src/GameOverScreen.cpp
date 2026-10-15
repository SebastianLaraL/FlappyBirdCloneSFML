#include "GameOverScreen.hpp"
#include "ButtonElement.hpp"
#include "LabelElement.hpp"
#include "PanelElement.hpp"
#include "ScoreNumber.hpp"

namespace FlappyBirdGame
{

	GameOverScreen::GameOverScreen(const sf::Vector2u windowSize)
	{
		const f32 centerX = windowSize.x / 2.f;
		const f32 scoreColumnX = centerX - 90.f;
		const f32 bestColumnX = centerX + 90.f;

		Add<LabelElement>("GAME OVER", 56, sf::Vector2f{ centerX, 120.f });
		Add<PanelElement>(sf::Vector2f{ centerX, 252.f }, sf::Vector2f{ 340.f, 110.f }, sf::Color(222, 216, 149), sf::Color(84, 56, 71));

		Add<LabelElement>("SCORE", 22, sf::Vector2f{ scoreColumnX, 228.f });
		score_ = Add<ScoreNumber>(
			sf::Vector2f{ scoreColumnX, 268.f }
		);
		Add<LabelElement>("BEST", 22, sf::Vector2f{ bestColumnX, 228.f });
		best_ = Add<ScoreNumber>(
			sf::Vector2f{ bestColumnX, 268.f }
		);

		playButton_ = Add<ButtonElement>("PLAY", sf::Vector2f{ centerX, 370.f }, sf::Vector2f{ 160.f, 56.f });
	}

	void GameOverScreen::SetScores(const u32 score, const u32 best)
	{
		score_->SetValue(score);
		best_->SetValue(best);
	}

	bool GameOverScreen::IsPlayButtonAt(const sf::Vector2f point) const
	{
		return playButton_->Contains(point);
	}

}
