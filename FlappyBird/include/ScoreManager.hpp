#pragma once
#include <string>
#include "Types.hpp"

namespace FlappyBirdGame
{
	// Same as SFXManager, this is a quick solution for this game, not designed to be a scalable score system.
	class ScoreManager
	{
	public:
		explicit ScoreManager(std::string topScoreFilePath = "topScore.dat");

		void AddPoint();

		void ResetScore();

		void SaveTopScore();

		void LoadTopScore();

		[[nodiscard]] u32 GetCurrentScore() const { return score_; }
		[[nodiscard]] u32 GetTopScore() const { return topScore_; }
		[[nodiscard]] bool IsNewHighScore() const { return bTopScoreChanged_; }

	private:
		u32 score_ = 0;
		u32 topScore_ = 0;
		std::string topScoreFilePath_;
		bool bTopScoreChanged_ = false;
	};
}
