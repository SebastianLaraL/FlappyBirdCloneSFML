#include "Background.hpp"
#include <stdexcept>
#include "RenderOrder.hpp"

namespace FlappyBirdGame
{

	Background::Background(const sf::Vector2u windowSize)
		: Entity(MakeSprite(), ::RenderOrder::BACKGROUND)
	{
		// Scale so the texture height matches the window height; width follows proportionally.
		scale_ = static_cast<f32>(windowSize.y) / TEXTURE_SIZE.y;
		sprite_.setScale({ scale_, scale_ });

		// Texture pixels needed to cover the window width (+1 so rounding never leaves a gap).
		visibleWidth_ = static_cast<i32>(windowSize.x / scale_) + 1;
		UpdateTextureRect(); // Defines the sprite size before World::Add() centers the origin.

		// Add() centers the origin, so place the center such that the left edge sits at x = 0.
		sprite_.setPosition({ visibleWidth_ * scale_ / 2.f, windowSize.y / 2.f });
	}

	void Background::Update(const f32 deltaTime)
	{
		// Content moves left -> the visible window of the texture moves right -> offset grows.
		// SPEED is in screen pixels, so convert to texture pixels by dividing by the scale.
		uvOffset_ += SPEED * deltaTime / scale_;

		if (uvOffset_ >= TEXTURE_SIZE.x) // Texture repeats, restart accumulator.
		{
			uvOffset_ -= TEXTURE_SIZE.x;
		}

		UpdateTextureRect();
	}

	void Background::UpdateTextureRect()
	{
		sprite_.setTextureRect(sf::IntRect(
			{ static_cast<i32>(uvOffset_), 0 },
			{ visibleWidth_, static_cast<i32>(TEXTURE_SIZE.y) }
		));
	}

	sf::Sprite Background::MakeSprite()
	{
		static sf::Texture texture = [] {
			sf::Texture tex;
			if (!tex.loadFromFile(TEXTURE_PATH))
				throw std::runtime_error("Could not load BACKGROUND texture.");
			tex.setRepeated(true);
			return tex;
			}();
		return sf::Sprite(texture);
	}

}
