#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

namespace FlappyBirdGame
{
	// Background that loops horizontally. Same technique as Floor: the texture repeats
	// and we slide the visible "window" (texture rect) over it.
	class Background : public Entity
	{
	public:
		explicit Background(const sf::Vector2u windowSize);

		void Update(const f32 deltaTime) override;

	private:
		void UpdateTextureRect();

		static sf::Sprite MakeSprite();

		f32 uvOffset_ = 0.f;
		f32 scale_ = 1.f;
		i32 visibleWidth_ = 0;

		static constexpr sf::Vector2f TEXTURE_SIZE = { 288.f, 512.f };
		static constexpr f32 SPEED = 20.f; // Screen px/s. Slower than floor and pipes (120) to give depth illusion.
		static constexpr const char* TEXTURE_PATH = "Assets/background-day.png";
	};
}
