#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

namespace FlappyBirdGame
{
	class Floor : public Entity
	{
	public:
		Floor();

		void Update(const f32 deltaTime) override;

	private:
		void UpdateTextureRect();

		// Fly-weight approach (reused for no specific reason). // LEARNING NOTE: now that I think about it, I think I could have implemented something like a factory method or a factory library
		static sf::Sprite MakeSprite();		// since FlyWeight implementation was used several times across this project.

		f32 uvOffset_ = 0.f;

		static constexpr sf::Vector2f TEXTURE_SIZE = { 336.f, 112.f };
		// All these values were found through iteration using imgui.
		static constexpr i32 VISIBLE_TEXTURE_WIDTH = 495;
		static constexpr f32 FLOOR_Y = 626.f;
		static constexpr f32 SPEED = -120.f;
		static constexpr const char* FLOOR_TEXTURE_PATH = "Assets/base.png";
	};
}
