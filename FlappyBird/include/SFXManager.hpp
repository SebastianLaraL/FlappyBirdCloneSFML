#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>

namespace FlappyBirdGame
{
	// Inside FlappyBirdGame namespace because it's a quick solution for this game, not designed to be a scalable audio system.
	class SFXManager
	{
	public:
		bool Load(const std::string& name, const std::string& path);

		void Play(const std::string& name);

	private:
		struct SoundData
		{
			std::unique_ptr<sf::SoundBuffer> buffer;
			std::unique_ptr<sf::Sound> sound;
		};

		std::unordered_map<std::string, SoundData> sounds_;
	};
}
