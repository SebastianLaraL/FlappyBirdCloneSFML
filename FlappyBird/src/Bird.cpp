#include "Bird.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace FlappyBirdGame
{

	Bird::Bird() : Entity(MakeSprite()),
		soundBuffer_("Assets/sfx_wing.wav"), sound_(soundBuffer_)
	{
		sprite_.setTextureRect(sf::IntRect(
			{ 0,0 },
			{ SPRITE_SIZE.x, SPRITE_SIZE.y }
		));
	}

	void Bird::Update(const f32 deltaTime)
	{
		Animate(deltaTime);
		ApplyPhysics(deltaTime);
	}

	void Bird::Hover(const f32 deltaTime, const f32 offsetY)
	{
		Animate(deltaTime);
		sprite_.setPosition({ sprite_.getPosition().x, hoverBaseY_ + offsetY });
	}

	void Bird::ApplyPhysics(const f32 deltaTime)
	{
		// New velocity.
		velY_ = velY_ + GRAVITY * deltaTime;
		velY_ = std::clamp(velY_, FLAP_IMPULSE, -FLAP_IMPULSE * 2);
		// Apply rotation.

		// LEARNING NOTE:
		// Tried to apply rotation using the rotation included in the Sprite,
		// but when calling getRotation, that rotation is normalized to 
		// [0,360], breaking my logic.
		// Solution: using a separate rotation variable.
		constexpr f32 ROTATION_LIMIT = 35.f;
		const auto GOING_UP = velY_ < 0; // NOTE TO MYSELF: SFML has inverted Y values.
		const f32 targetRotation = GOING_UP ? -ROTATION_LIMIT : ROTATION_LIMIT;
		rotation_ = std::lerp(rotation_, targetRotation,
			GOING_UP ? rotationSpeed_ * 0.2f : // When going upwards rotation is fast but still perceptible.
			deltaTime * rotationSpeed_); // When falling, bit by bit.

		rotation_ = std::clamp(rotation_, -ROTATION_LIMIT, ROTATION_LIMIT);
		sprite_.setRotation(sf::degrees(rotation_));


		// Update position.

		const auto oldY = sprite_.getPosition().y;
		const auto newY = oldY + velY_ * deltaTime * bChangesVerticalVelocity; // solution to avoid bird falling. this is to speed up the debug of the obstacles.

		sprite_.setPosition({
			sprite_.getPosition().x,
			newY
			});

	}

	void Bird::Flap()
	{
		velY_ = FLAP_IMPULSE;
		sound_.play();
	}

	void Bird::Reset(const sf::Vector2f position)
	{
		sprite_.setPosition(position);
		sprite_.setRotation(sf::degrees(0.f));
		velY_ = 0.f;
		rotation_ = 0.f;
		hoverBaseY_ = position.y;
	}

	f32 Bird::GetLowestVisibleOffset() const
	{
		static const sf::Image image = sprite_.getTexture().copyToImage(); // Same texture for every bird: read it once.

		const sf::IntRect frame = sprite_.getTextureRect();
		const sf::Vector2f origin = sprite_.getOrigin();
		const f32 angle = sprite_.getRotation().asRadians();
		const f32 sinA = std::sin(angle);
		const f32 cosA = std::cos(angle);
		const f32 pixelHalfExtent = 0.5f * (std::abs(sinA) + std::abs(cosA)); // Lowest point of a rotated 1x1 pixel.

		f32 lowest = 0.f;
		for (i32 y = 0; y < frame.size.y; ++y)
		{
			for (i32 x = 0; x < frame.size.x; ++x)
			{
				const sf::Vector2u texel = { static_cast<u32>(frame.position.x + x), static_cast<u32>(frame.position.y + y) };
				if (image.getPixel(texel).a < 128) continue; // Transparent.

				// Pixel center relative to the origin, rotated the same way the sprite is rotated.
				const f32 localX = static_cast<f32>(x) + 0.5f - origin.x;
				const f32 localY = static_cast<f32>(y) + 0.5f - origin.y;
				lowest = std::max(lowest, localX * sinA + localY * cosA + pixelHalfExtent);
			}
		}
		return lowest;
	}

	sf::Sprite Bird::MakeSprite()
	{
		static sf::Texture texture = []
			{
				sf::Texture tex;
				if (!tex.loadFromFile(SPRITE_SHEET_PATH))
					throw std::runtime_error("Could not load bird texture.");
				return tex;
			}();
		return sf::Sprite(texture);
	}

	void Bird::Animate(const f32 deltaTime)
	{
		static f32 elapsedTimeSinceLastAnimUpdate = 0.f;
		elapsedTimeSinceLastAnimUpdate += deltaTime;


		// I hate the need of using an "Update" approach every frame,
		// specially because of the need for an "if", but I could not find a better
		// implementation. I also thought of using a while loop, but I read
		// it evaluates to something similar to an if, therefore being prone to
		// branch misprediction as well.
		if (elapsedTimeSinceLastAnimUpdate < UPDATE_ANIM_SPAN) return;


		// Clean elapsed time.
		elapsedTimeSinceLastAnimUpdate -= UPDATE_ANIM_SPAN;

		// Up->middle->down->middle->up. Loop this.
		static i32 frame = 0;
		static i32 direction = 1; // 1 means go up, -1 means go down.
		constexpr sf::Vector2i SPRITE_SIZE_INT = { static_cast<i32>(SPRITE_SIZE.x), static_cast<i32>(SPRITE_SIZE.y) };
		sprite_.setTextureRect(sf::IntRect(
			{ SPRITE_SIZE_INT.x * frame, 0 },
			SPRITE_SIZE_INT // Typed this way instead of only SPRITE_SIZE because it would not compile due to type conversion failure.
		));


		frame += direction;
		// Exceeds limits? reverse state direction.
		if (frame >= 2 || frame <= 0)
		{
			direction *= -1;
			// LOG std::cout << "Reached the border of an animation... inversed direction." << std::endl;
		}
		// LOG std::cout << "Updated BIRD animation" << std::endl;
	}

}
