#include "FlappyBirdWorld.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <string_view>
#include "Background.hpp"
#include "Bird.hpp"
#include "Floor.hpp"
#include "FlappyRenderOrder.hpp"
#include "GameOverScreen.hpp"
#include "HUDScreen.hpp"
#include "ObstaclePair.hpp"
#include "StartScreen.hpp"

#ifdef GAME_DEBUG
#include "imgui.h"
#endif // GAME_DEBUG

namespace
{
	constexpr std::string_view SFX_SCORE_PATH = "Assets/sfx_point.wav";
	constexpr std::string_view SFX_HIT_PATH = "Assets/sfx_hit.wav";
	constexpr std::string_view SFX_DIE_PATH = "Assets/sfx_die.wav"; // This audio is played when the bird dies crashing agains an obstacle.
	constexpr std::string_view SFX_RESTART_PATH = "Assets/sfx_swooshing.wav"; // Played when player clics Play Again button in game over screen.
}

namespace FlappyBirdGame
{

	World::World(sf::RenderWindow& window) : ::World(window)
	{
		// I do not think it is necessary for this small game to implement an Init() approach.


		entities_.reserve(static_cast<size_t>(8)); // Bird, floor, and 3 pairs of obstacles.

		// Create bird.
		bird_ = Add<Bird>();
		assert(bird_);
		assert(entities_[0].get() == bird_);

		birdStartPos_ = { static_cast<f32>(window_.getSize().x) / 3, static_cast<f32>(window_.getSize().y) / 2 };
		bird_->Reset(birdStartPos_);

		// Create Floor.
		floor_ = Add<Floor>();

		// Create obstacles.
		const f32 windowWidth = static_cast<f32>(window_.getSize().x);
		// A pipe is still visible while its center is within half a pipe width of the edge, hence the addition of the pipe width.
		const f32 pairSpacing = (windowWidth + ObstaclePair::GetPipeWidth()) / MAX_VISIBLE_PAIRS;
		const f32 loopLength = pairSpacing * NUM_OBSTACLE_PAIRS;
		// The first pair spawns just outside the window (half a pipe past the right border),
		// so no pipe is visible on the start screen and they all enter from the right.
		const f32 firstPairX = windowWidth + ObstaclePair::GetPipeWidth() / 2.f;

		for (i32 i = 0; i < NUM_OBSTACLE_PAIRS; i++)
		{
			const sf::Vector2f spawnPos = { firstPairX + i * pairSpacing, static_cast<f32>(window_.getSize().y) / 2.f };
			obstaclePairs_[i] = Add<ObstaclePair>(spawnPos, loopLength);

			// Add sprites to draw order pipeline.
			AddAdditionalSprite(&obstaclePairs_[i]->GetTopSprite(), FlappyBirdGame::RenderOrder::OBSTACLES);
			AddAdditionalSprite(&obstaclePairs_[i]->GetBottomSprite(), FlappyBirdGame::RenderOrder::OBSTACLES);
		}

		// Create background.
		background_ = Add<Background>(window_.getSize());

		// SFX loading.
		sfxManager_.Load("SCORE", SFX_SCORE_PATH.data());
		sfxManager_.Load("HIT", SFX_HIT_PATH.data());
		sfxManager_.Load("DIE", SFX_DIE_PATH.data());
		sfxManager_.Load("RESTART", SFX_RESTART_PATH.data());

		// UI screens. Which one is visible depends on the game state (see SetState()).
		hud_ = AddScreen<HUDScreen>(window_.getSize());
		startScreen_ = AddScreen<StartScreen>(window_.getSize());
		gameOverScreen_ = AddScreen<GameOverScreen>(window_.getSize());

		EnterReady();
	}

	void World::Update(const f32 deltaTime)
	{
		switch (state_)
		{
		case EGameState::Ready:    UpdateReady(deltaTime);   break;
		case EGameState::Playing:  UpdatePlaying(deltaTime); break;
		case EGameState::Dying:    UpdateDying(deltaTime);   break;
		case EGameState::GameOver: UpdateScreens(deltaTime); break; // Nothing moves.
		}
	}

	void World::HandleInput(const std::optional<sf::Event>& event)
	{
		const auto* click = event->getIf<sf::Event::MouseButtonPressed>();
		if (!click || click->button != sf::Mouse::Button::Left) return;

		switch (state_)
		{
		case EGameState::Ready:
			StartGame();
			break;
		case EGameState::Playing:
			bird_->Flap();
			break;
		case EGameState::Dying:
			break; // The bird can't be controlled anymore.
		case EGameState::GameOver:
			if (gameOverScreen_->IsPlayButtonAt(sf::Vector2f(click->position)))
			{
				Restart();
			}
			break;
		}
	}

	void World::OnImGuiUpdateValues()
	{
#ifdef GAME_DEBUG
		ImGui::DragFloat2("bottom offset:", &obstaclePairs_[0]->bottomOffset.x, 0.1f);

		if (ImGui::Button("Randomize Y", ImGui::CalcTextSize("pivot new random Y")) && &obstaclePairs_[0])
		{
			obstaclePairs_[0]->RandomizeSpritesY();
			std::cout << "clicked on button" << std::endl;
		}
		ImGui::Checkbox("Bird Vertical Velocity can change", &bird_->bChangesVerticalVelocity);

		ImGui::Separator();
		ImGui::Text("Hitboxes");
		ImGui::Checkbox("Show hitboxes", &hitbox_.show);
		ImGui::Checkbox("Show sprite bounds", &hitbox_.showSpriteBounds);
		ImGui::DragFloat2("Bird scale", &hitbox_.birdScale.x, 0.01f, 0.1f, 2.f);
#endif // GAME_DEBUG
	}

	void World::RenderDebug(sf::RenderWindow& window) const
	{
		if (!hitbox_.show) return;

		const sf::Color spriteBoundsColor(150, 150, 150);

		// Actual bounding box of the sprites, for comparison against the hitbox.
		if (hitbox_.showSpriteBounds)
		{
			DrawDebugRect(window, bird_->GetSprite().getGlobalBounds(), spriteBoundsColor);
			for (auto* pair : obstaclePairs_)
			{
				if (!pair) continue;
				DrawDebugRect(window, pair->GetTopSprite().getGlobalBounds(), spriteBoundsColor);
				DrawDebugRect(window, pair->GetBottomSprite().getGlobalBounds(), spriteBoundsColor);
			}
		}

		DrawDebugRect(window, GetBirdHitbox(), sf::Color::Green);
		for (auto* pair : obstaclePairs_)
		{
			if (!pair) continue;
			DrawDebugRect(window, GetPipeHitbox(pair->GetTopSprite()), sf::Color::Red);
			DrawDebugRect(window, GetPipeHitbox(pair->GetBottomSprite()), sf::Color::Red);
		}
	}

	sf::FloatRect World::GetBirdHitbox() const
	{
		const sf::Vector2f size = {
			Bird::SPRITE_SIZE.x * hitbox_.birdScale.x,
			Bird::SPRITE_SIZE.y * hitbox_.birdScale.y
		};
		const sf::Vector2f center = bird_->GetSprite().getPosition();
		return sf::FloatRect(center - size / 2.f, size);
	}

	sf::FloatRect World::GetPipeHitbox(sf::Sprite& pipeSprite) const
	{
		const sf::FloatRect bounds = pipeSprite.getGlobalBounds();
		const sf::Vector2f size = {
			bounds.size.x,
			bounds.size.y
		};
		return sf::FloatRect(bounds.position, size);
	}

	void World::CheckCollisions()
	{
		if (state_ != EGameState::Playing) return; // Collisions only matter while playing (and it logs only once).

		const sf::FloatRect birdBox = GetBirdHitbox();

		// Ground.
		if (birdBox.findIntersection(floor_->GetSprite().getGlobalBounds()))
		{
			OnBirdHit("floor");
			return;
		}

		// Obstacles.
		for (auto* pair : obstaclePairs_)
		{
			if (!pair) continue;

			if (birdBox.findIntersection(GetPipeHitbox(pair->GetTopSprite())) ||
				birdBox.findIntersection(GetPipeHitbox(pair->GetBottomSprite())))
			{
				OnBirdHit("obstacle");
				sfxManager_.Play("DIE");
				return;
			}
		}
	}

	void World::DrawDebugRect(sf::RenderWindow& window, const sf::FloatRect& rect, sf::Color color)
	{
		sf::RectangleShape shape(rect.size);
		shape.setPosition(rect.position);
		shape.setFillColor(sf::Color(color.r, color.g, color.b, 40)); // Semi-transparent fill.
		shape.setOutlineColor(color);
		shape.setOutlineThickness(-1.f);
		window.draw(shape);
	}

	void World::SetState(const EGameState state)
	{
		state_ = state;

		const auto setVisible = [](UIScreen* screen, const bool visible)
			{
				visible ? screen->Show() : screen->Hide();
			};
		setVisible(startScreen_, state == EGameState::Ready);
		setVisible(hud_, state == EGameState::Playing || state == EGameState::Dying);
		setVisible(gameOverScreen_, state == EGameState::GameOver);
	}

	void World::EnterReady()
	{
		readyTime_ = 0.f;
		startScreen_->SetTopScore(scoreManager_.GetTopScore());
		SetState(EGameState::Ready);
	}

	void World::StartGame()
	{
		SetState(EGameState::Playing);
		bird_->Flap(); // The click that starts the game is also the first flap.
	}

	void World::Restart()
	{
		scoreManager_.ResetScore();
		hud_->SetScore(0);
		sfxManager_.Play("RESTART");
		bird_->Reset(birdStartPos_);
		for (auto* obstPair : obstaclePairs_)
		{
			if (obstPair)
			{
				obstPair->Reset();
			}
		}
		nextObstacleIndex_ = 0;

		EnterReady();
	}

	void World::UpdateReady(const f32 deltaTime)
	{
		readyTime_ += deltaTime;
		const f32 bob = std::sin(readyTime_ * BOB_SPEED) * BOB_AMPLITUDE;

		background_->Update(deltaTime);
		floor_->Update(deltaTime);
		bird_->Hover(deltaTime, bob);
		startScreen_->SetBob(bob);

		UpdateScreens(deltaTime);
	}

	void World::UpdatePlaying(const f32 deltaTime)
	{
		::World::Update(deltaTime); // Everything moves: entities and screens.
		CheckCollisions();
		if (state_ != EGameState::Playing) return; // The bird just crashed.

		auto* nextObstacle = obstaclePairs_[nextObstacleIndex_];

		if (nextObstacle->GetSprite().getPosition().x < bird_->GetSprite().getPosition().x) // Could use [[unlikely]] but I don't think it is necessary.
		{
			scoreManager_.AddPoint();
			hud_->SetScore(scoreManager_.GetCurrentScore());
			std::cout << "SCORE: " << scoreManager_.GetCurrentScore()
				<< " | TOP SCORE: " << scoreManager_.GetTopScore() << std::endl;
			sfxManager_.Play("SCORE");
			nextObstacleIndex_ = (nextObstacleIndex_ + 1) % NUM_OBSTACLE_PAIRS; // Next obstacle pair in circular manner.
		}
	}

	void World::UpdateDying(const f32 deltaTime)
	{
		bird_->ApplyPhysics(deltaTime);

		// Landed: the lowest visible pixel of the bird touches the top of the floor.
		const f32 floorTop = floor_->GetSprite().getGlobalBounds().position.y;
		const f32 birdBottomOffset = bird_->GetLowestVisibleOffset();
		if (bird_->GetSprite().getPosition().y + birdBottomOffset >= floorTop)
		{
			// Rest exactly on the floor.
			bird_->GetSprite().setPosition({ bird_->GetSprite().getPosition().x, floorTop - birdBottomOffset });
			gameOverScreen_->SetScores(scoreManager_.GetCurrentScore(), scoreManager_.GetTopScore());
			SetState(EGameState::GameOver);
		}

		UpdateScreens(deltaTime);
	}

	void World::UpdateScreens(const f32 deltaTime)
	{
		for (auto& screen : screens_)
		{
			screen->Update(deltaTime);
		}
	}

	void World::OnBirdHit(const char* what)
	{
		SetState(EGameState::Dying);
		scoreManager_.SaveTopScore();
		std::cout << "COLLISION with " << what
			<< " | Final Score: " << scoreManager_.GetCurrentScore()
			<< " | Best: " << scoreManager_.GetTopScore() << std::endl;
		sfxManager_.Play("HIT");
	}

}
