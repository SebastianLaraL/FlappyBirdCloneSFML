#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "UIElement.hpp"

namespace FlappyBirdGame
{
	// Draws a number with this game's digit spritesheet.
	class ScoreNumber : public UIElement
	{
	public:
		explicit ScoreNumber(const sf::Vector2f anchor);

		void SetValue(const u32 value);

		void Draw(sf::RenderTarget& target) const override;

	private:
		void Rebuild();

		// The ONLY place that knows the layout of the spritesheet.
		static sf::IntRect GetDigitRect(const i32 digit);

		static const sf::Texture& GetTexture();

		sf::Vector2f anchor_;
		u32 value_ = 0;
		std::vector<sf::Sprite> digits_;

		// Layout of the digit spritesheet.
		static constexpr const char* TEXTURE_PATH = "Assets/score_numbers_spritesheet.png";
		static constexpr sf::Vector2i DIGIT_SIZE = { 24, 36 }; // 10 digits in one row, same size each.
		static constexpr f32 SPACING = 2.f;                    // Pixels between digits.
	};
}
