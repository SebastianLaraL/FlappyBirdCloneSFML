#include "SFXManager.hpp"
#include <iostream>

namespace FlappyBirdGame
{

	bool SFXManager::Load(const std::string& name, const std::string& path)
	{
		auto buffer = std::make_unique<sf::SoundBuffer>();

		if (!buffer->loadFromFile(path))
		{
			std::cerr << "Failed to load SFX: "
				<< path << '\n';
			return false;
		}

		sounds_.emplace(name,
			SoundData{
				std::move(buffer),
				nullptr
			});

		sounds_.at(name).sound =
			std::make_unique<sf::Sound>(*sounds_.at(name).buffer);

		return true;
	}

	void SFXManager::Play(const std::string& name)
	{
		auto it = sounds_.find(name);

		if (it == sounds_.end())
			return;

		it->second.sound->play();
	}

}
