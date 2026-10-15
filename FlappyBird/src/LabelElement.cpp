#include "LabelElement.hpp"

namespace
{
	// Font used by all the text of this game's UI.
	constexpr const char* UI_FONT_PATH = "Assets/FlappyBirdy.ttf";

	const sf::Font& GetUIFont()
	{
		static const sf::Font font(UI_FONT_PATH);
		return font;
	}
}

namespace FlappyBirdGame
{

	LabelElement::LabelElement(const std::string& text, const u32 characterSize, const sf::Vector2f anchor)
		: text_(GetUIFont(), text, characterSize)
	{
		text_.setFillColor(sf::Color::White);
		text_.setOutlineColor(sf::Color::Black);
		text_.setOutlineThickness(3.f);
		SetCenter(anchor);
	}

	void LabelElement::SetCenter(const sf::Vector2f anchor)
	{
		const sf::FloatRect bounds = text_.getLocalBounds();
		text_.setOrigin(bounds.position + bounds.size / 2.f);
		text_.setPosition(anchor);
	}

	void LabelElement::Draw(sf::RenderTarget& target) const
	{
		target.draw(text_);
	}

}
