#include "ObstaclePair.hpp"
#include <stdexcept>
#include "Bird.hpp"
#include "FlappyRenderOrder.hpp"
#include "Math.hpp"

namespace FlappyBirdGame
{

	ObstaclePair::ObstaclePair(const sf::Vector2f spawnPos, const f32 loopLength) : Entity(MakeSprite(), RenderOrder::OBSTACLES), top_(MakeSprite()), bottom_(MakeSprite()), loopLength_(loopLength), spawnPos_(spawnPos)
	{
		// NOTE: sprite_ will be used as a pivot so
		// it is made invisible even though it uses the obstacle texture.
		sprite_.setColor(sf::Color::Transparent);
		sprite_.setPosition(spawnPos);

		const auto centerOrigin = sf::Vector2f(top_.getLocalBounds().size.x / 2, top_.getLocalBounds().size.y / 2);
		top_.setOrigin(centerOrigin); // Both sprites use the same texture.
		bottom_.setOrigin(centerOrigin);

		top_.setRotation(sf::degrees(180.f));

		RandomizeSpritesY();
		UpdateSpritePositions(); // Place the pipes now: the pair may not be updated for a while (start screen).
	}

	void ObstaclePair::Update(const f32 deltaTime)
	{
		sprite_.move({ speedX * deltaTime, 0.f });
		CheckResetObstaclePair();

		UpdateSpritePositions();
	}

	void ObstaclePair::Reset()
	{
		sprite_.setPosition(spawnPos_);
		RandomizeSpritesY();
		UpdateSpritePositions();
	}

	void ObstaclePair::CheckResetObstaclePair()
	{
		const f32 halfPipeWidth = top_.getLocalBounds().size.x / 2.f;
		if (sprite_.getPosition().x < -halfPipeWidth)
		{
			sprite_.move({ loopLength_, 0.f }); // At the end of the line.
			RandomizeSpritesY();
		}
	}

	void ObstaclePair::PlaceBottomRandomY()
	{
		// SFML y coords... inverted...
		constexpr auto UPPER_POS = 110.f;
		constexpr auto LOWER_POS = 405.f;
		bottomOffset.y = Random(UPPER_POS, LOWER_POS);
	}

	void ObstaclePair::PlaceTopRandomY()
	{
		// SFML y coords... inverted...
		constexpr auto UPPER_POS = -433.f;
		constexpr auto LOWER_POS = -140.f;
		topOffset.y = Random(UPPER_POS, LOWER_POS);
	}

	void ObstaclePair::RandomizeSpritesY()
	{
		do
		{
			PlaceBottomRandomY();
			PlaceTopRandomY();
		} while (!PipesInReasonableDistance());
	}

	bool ObstaclePair::PipesInReasonableDistance() const
	{
		// Max gap distance is around 4 to 5.5 times the height of the bird.
		constexpr auto MIN_Y_GAP = FlappyBirdGame::Bird::SPRITE_SIZE.y * 4;
		constexpr auto MAX_Y_GAP = FlappyBirdGame::Bird::SPRITE_SIZE.y * 5.5f;
		const f32 tubeHalfHeight = bottom_.getLocalBounds().size.y / 2.f;

		// Offsets, not sprite positions: positions only refresh in Update().
		const f32 topTipY = topOffset.y + tubeHalfHeight;          // lowest point of the top tube
		const f32 bottomTipY = bottomOffset.y - tubeHalfHeight;   // highest point of the bottom tube

		const f32 gap = bottomTipY - topTipY;

		return gap >= MIN_Y_GAP && gap <= MAX_Y_GAP;
	}

	f32 ObstaclePair::GetPipeWidth()
	{
		return MakeSprite().getLocalBounds().size.x;
	}

	sf::Sprite ObstaclePair::MakeSprite()
	{
		static sf::Texture texture = [] {
			sf::Texture tex;
			if (!tex.loadFromFile(TEXTURE_PATH))
				throw std::runtime_error("Could not load OBSTACLE texture.");
			return tex;
			}();
		return sf::Sprite(texture);
	}

	void ObstaclePair::UpdateSpritePositions()
	{
		const auto currentPos = sprite_.getPosition();
		top_.setPosition(currentPos + topOffset);
		bottom_.setPosition(currentPos + bottomOffset);
	}

}
