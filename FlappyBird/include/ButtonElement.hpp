#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "UIElement.hpp"
#include "LabelElement.hpp"

namespace FlappyBirdGame
{
	// Rectangle with a text. It doesn't read input by itself: whoever owns it asks Contains().
	class ButtonElement : public UIElement
	{
	public:
		ButtonElement(const std::string& text, const sf::Vector2f center, const sf::Vector2f size);

		bool Contains(const sf::Vector2f point) const;

		void Draw(sf::RenderTarget& target) const override;

	private:
		sf::RectangleShape shape_;
		LabelElement label_;
	};
}
