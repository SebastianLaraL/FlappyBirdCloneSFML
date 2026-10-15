#pragma once
#include <string_view>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

namespace FlappyBirdGame
{
	class Bird : public Entity
	{
	public:
		Bird();

		// Playing: the wings flap and physics apply.
		void Update(const f32 deltaTime) override;

		// Start screen: the wings flap and the bird floats around its base height (no gravity).
		// 'offsetY' is how far from that base height the bird is right now.
		void Hover(const f32 deltaTime, const f32 offsetY);

		// Gravity, rotation and movement, without animating the wings.
		// Also used while the bird falls after a crash.
		void ApplyPhysics(const f32 deltaTime);

		// The world only calls this while the player can control the bird.
		void Flap();

		// Back to the initial state at 'position'.
		void Reset(const sf::Vector2f position);

		// Distance from the sprite position (its center) down to the lowest VISIBLE pixel of the bird,
		// for the current wing frame and rotation. The sprite bounds can't be used to rest the bird on
		// the floor: they are the rotated rectangle of the whole frame, and its lowest corner is transparent.
		f32 GetLowestVisibleOffset() const;

	private:
		// Bird uses spritesheet;
		static sf::Sprite MakeSprite();

		void Animate(const f32 deltaTime);

	private:
		f32 velY_ = 0.f;
		f32 rotation_ = 0.f;
		f32 rotationSpeed_ = 2.f;
		f32 hoverBaseY_ = 0.f; // Height around which the bird floats on the start screen.
	public:
		bool bChangesVerticalVelocity = true;

		// CONSTANTS

		// Gameplay.
		static constexpr f32 GRAVITY = 900.f;
		static constexpr f32 FLAP_IMPULSE = -300.f;

		// Sprites.
	public:
		static constexpr sf::Vector2u SPRITE_SIZE = { 34u,24u }; // I'm not sure why but the sprite size is 34 px width (in the spritesheet), but original sprites are 32 px.
	private:
		static constexpr std::string_view SPRITE_SHEET_PATH = "Assets/spritesheetYellow.png";

		// Animation.
		static constexpr f32 UPDATE_ANIM_SPAN = 0.1f;

		// SFX.
		sf::SoundBuffer soundBuffer_;
		sf::Sound sound_;
	};
}
