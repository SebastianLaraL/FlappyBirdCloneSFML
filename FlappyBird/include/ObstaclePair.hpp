#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

namespace FlappyBirdGame
{
	/*
	* Known "issues":
	* // NOTE: Unfortunately because of the height of the pipes asset, the Top obstacle lowest point is -140, so in gameplay it doesnt look so random and gives the illusion that the obstacles are usually in the top zone of the screen.
	*/

	// Uses inherited sprite as center.
	class ObstaclePair : public Entity
	{
	public:

		static constexpr f32 speedX = -120.f;
		sf::Vector2f topOffset = sf::Vector2f();
		sf::Vector2f bottomOffset = sf::Vector2f();

		explicit ObstaclePair(const sf::Vector2f spawnPos, const f32 loopLength);

		void Update(const f32 deltaTime) override;

		// Puts the pair back at its spawn position with new random heights (used when restarting).
		void Reset();

		void CheckResetObstaclePair();

		void PlaceBottomRandomY();

		void PlaceTopRandomY();

		void RandomizeSpritesY();

		// NOTE: Unfortunately because of the height of the pipes asset, the Top obstacle lowest point is -140, so in gameplay it doesnt look so random and gives the illusion that the obstacles are usually in the top zone of the screen.
		bool PipesInReasonableDistance() const;

		inline sf::Sprite& GetTopSprite() { return top_; }
		inline sf::Sprite& GetBottomSprite() { return bottom_; }
		static f32 GetPipeWidth();
	private:
		// For faster iteration. TODO: remove.
		static sf::Sprite MakeTempOrigin();

		// Fly-weight approach is very useful in this case since there are 3 instances of the obstacle.
		static sf::Sprite MakeSprite();

		void UpdateSpritePositions();

		sf::Sprite top_;
		sf::Sprite bottom_;
		sf::Vector2f spawnPos_;
		f32 loopLength_;

		static constexpr const char* TEXTURE_PATH = "Assets/pipe-green.png";
	};
}
