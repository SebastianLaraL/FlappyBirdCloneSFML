#include "Floor.hpp"
#include <stdexcept>
#include "FlappyRenderOrder.hpp"

namespace FlappyBirdGame
{

	Floor::Floor() : Entity(MakeSprite(), RenderOrder::FLOOR)
	{
		// Define the final size of the sprite (495 x 112).
		// World will use this to center the origin.
		UpdateTextureRect();

		// On X we center the sprite on its own width so it starts in x = 0;
		sprite_.setPosition({ VISIBLE_TEXTURE_WIDTH / 2.f, FLOOR_Y });
	}

	void Floor::Update(const f32 deltaTime)
	{
		// LEARNING NOTE:
		// In order for the content to go left, the "window" of the texture
		// must go to the right, thats why we subtract.
		uvOffset_ -= SPEED * deltaTime;

		// Texture repeats, restart accumulator.
		if (uvOffset_ >= TEXTURE_SIZE.x) [[unlikely]]
		{
			uvOffset_ -= TEXTURE_SIZE.x;
		}

		UpdateTextureRect();
	}

	void Floor::UpdateTextureRect()
	{
		sprite_.setTextureRect(sf::IntRect(
			{ static_cast<i32>(uvOffset_), 0 },
			{ VISIBLE_TEXTURE_WIDTH, static_cast<i32>(TEXTURE_SIZE.y) }
		));
	}

	sf::Sprite Floor::MakeSprite()
	{
		static sf::Texture texture = [] {
			sf::Texture tex;
			if (!tex.loadFromFile(FLOOR_TEXTURE_PATH))
				throw std::runtime_error("Could not load FLOOR texture.");
			tex.setRepeated(true);
			return tex;
			}();
		return sf::Sprite(texture);
	}

}
