#include "ScoreManager.hpp"
#include <fstream>
#include <utility>

namespace FlappyBirdGame
{

	ScoreManager::ScoreManager(std::string topScoreFilePath) : topScoreFilePath_(std::move(topScoreFilePath))
	{
		LoadTopScore();
	}

	void ScoreManager::AddPoint()
	{
		score_++;
		if (score_ > topScore_)
		{
			topScore_ = score_;
			bTopScoreChanged_ = true;
		}
	}

	void ScoreManager::ResetScore()
	{
		score_ = 0;
	}

	void ScoreManager::SaveTopScore()
	{
		if (!bTopScoreChanged_)
		{
			return;
		}
		// LEARNING NOTE: The following implementation writes the top score as a human readable number,
		// when using std::ios::binary, the top score will be written as a binary format (it's supposedly the perfect format to prevent cheating).
		std::ofstream file(topScoreFilePath_);//, std::ios::binary);
		if (file.is_open())
		{
			// file.write(reinterpret_cast<const char*>(&topScore_), sizeof(topScore_));
			file << topScore_;
			bTopScoreChanged_ = false;
		}
	}

	void ScoreManager::LoadTopScore()
	{
		std::ifstream file(topScoreFilePath_);//, std::ios::binary);
		u32 loadedScore = 0;

		if (file.is_open() && (file >> loadedScore))// file.read(reinterpret_cast<char*>(&loadedScore), sizeof(topScore_)))
		{
			topScore_ = loadedScore;
		}
		else
		{
			topScore_ = 0;
		}
	}

}
