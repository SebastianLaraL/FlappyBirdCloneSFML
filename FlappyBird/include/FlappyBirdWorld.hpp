#pragma once
#include <array>
#include <cstddef>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "SFXManager.hpp"
#include "ScoreManager.hpp"

namespace FlappyBirdGame
{
	class Bird;
	class Floor;
	class Background;
	class ObstaclePair;
	class HUDScreen;
	class StartScreen;
	class GameOverScreen;

	/*
	* Flappy bird:
	* Uses a maximum of 3 pairs of obstacles at the same time.
	* When a pair is exiting the left border of the screen, a new pair on the right border appears.
	*
	* Player scores 1 point every time he exceeds the x center of a pair of obstacles.
	*/

	/*
	* OK so I think I should do this for the game loop:
	* world updates entities.
	* Entities update position.
	* World checks for bird collision.
	* bird applies impulse and gravity.
	*/
	class World : public ::World
	{
	public:
		static constexpr i32 NUM_OBSTACLE_PAIRS = 3;
		static constexpr i32 MAX_VISIBLE_PAIRS = 2; // The rest waits outside the window, to the right.
		static_assert(NUM_OBSTACLE_PAIRS > MAX_VISIBLE_PAIRS, "At least one pair must wait outside the window.");

		World(sf::RenderWindow& window);

		void Update(const f32 deltaTime) override;

		void HandleInput(const std::optional<sf::Event>& event) override;

		void OnImGuiUpdateValues() override;

		void RenderDebug(sf::RenderWindow& window) const override;

		sf::FloatRect GetBirdHitbox() const;

		sf::FloatRect GetPipeHitbox(sf::Sprite& pipeSprite) const;

		void CheckCollisions();

	private:
		Bird* bird_;
		Floor* floor_;
		Background* background_ = nullptr;
		sf::Vector2f birdStartPos_;
		std::array<ObstaclePair*, NUM_OBSTACLE_PAIRS> obstaclePairs_ = {};

		struct HitboxSettings
		{
			bool show = true;
			bool showSpriteBounds = true;
			sf::Vector2f birdScale = { 0.75f, 0.7f };
		};
		HitboxSettings hitbox_;

		// Draw in SFML default origin. TODO: center origin?
		static void DrawDebugRect(sf::RenderWindow& window, const sf::FloatRect& rect, sf::Color color);

		enum class EGameState
		{
			Ready,    // Start screen: bird floating, world scrolling, no pipes yet.
			Playing,  // Normal gameplay.
			Dying,    // The bird crashed: everything is frozen except the bird falling to the floor.
			GameOver  // The bird landed: score panel and Play button.
		};

		// Everything that changes when the state changes is here, so it can't be forgotten.
		void SetState(const EGameState state);

		void EnterReady();

		void StartGame();

		// Back to the start screen with a fresh game.
		void Restart();

		// Start screen: the world scrolls, the bird and the title bob up and down.
		void UpdateReady(const f32 deltaTime);

		void UpdatePlaying(const f32 deltaTime);

		// Only the bird moves (falls). Floor, background and pipes stay frozen.
		void UpdateDying(const f32 deltaTime);

		// World::Update() also updates the screens. The states that don't call it do it here.
		void UpdateScreens(const f32 deltaTime);

		void OnBirdHit(const char* what);

		EGameState state_ = EGameState::Ready;
		f32 readyTime_ = 0.f;
		std::size_t nextObstacleIndex_ = 0; // Next pair the bird has to pass. A member (not a static) so a restart can reset it.
		static constexpr f32 BOB_SPEED = 4.f;      // Radians per second.
		static constexpr f32 BOB_AMPLITUDE = 8.f;  // Pixels up and down.
		SFXManager sfxManager_; // I think an audio system should be placed in the application, instead of the world, but for this small game it is not necessary.
		ScoreManager scoreManager_;

		HUDScreen* hud_ = nullptr;
		StartScreen* startScreen_ = nullptr;
		GameOverScreen* gameOverScreen_ = nullptr;
	};
}
