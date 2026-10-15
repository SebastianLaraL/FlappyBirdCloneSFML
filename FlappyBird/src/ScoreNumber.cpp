#include "ScoreNumber.hpp"
#include <string>

namespace FlappyBirdGame
{

	ScoreNumber::ScoreNumber(const sf::Vector2f anchor) : anchor_(anchor)
	{
		Rebuild();
	}

	void ScoreNumber::SetValue(const u32 value)
	{
		if (value == value_) return; // Only rebuild when it changes.
		value_ = value;
		Rebuild();
	}

	void ScoreNumber::Draw(sf::RenderTarget& target) const
	{
		for (const auto& digit : digits_)
			target.draw(digit);
	}

	void ScoreNumber::Rebuild()
	{
		const std::string text = std::to_string(value_);

		// Total width first, to be able to center.
		f32 totalWidth = SPACING * static_cast<f32>(text.size() - 1);
		for (const char c : text)
			totalWidth += static_cast<f32>(GetDigitRect(c - '0').size.x);

		f32 x = anchor_.x - totalWidth / 2.f; // Left edge of the next digit.

		digits_.clear();
		digits_.reserve(text.size());

		for (const char c : text)
		{
			const sf::IntRect rect = GetDigitRect(c - '0');
			sf::Sprite& digit = digits_.emplace_back(GetTexture());

			digit.setTextureRect(rect);

			// Center the origin of each digit.
			digit.setOrigin(sf::Vector2f(rect.size) / 2.f);

			// Position the digit using its center.
			digit.setPosition({
				x + static_cast<f32>(rect.size.x) / 2.f,
				anchor_.y
				});

			x += static_cast<f32>(rect.size.x) + SPACING;
		}
	}

	sf::IntRect ScoreNumber::GetDigitRect(const i32 digit)
	{
		return sf::IntRect({ digit * DIGIT_SIZE.x, 0 }, DIGIT_SIZE);
	}

	const sf::Texture& ScoreNumber::GetTexture()
	{
		static const sf::Texture texture(TEXTURE_PATH);
		return texture;
	}

}
