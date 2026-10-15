#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "UIElement.hpp"

namespace FlappyBirdGame
{
	// Text in screen space. 'anchor' is the CENTER of the text.
	class LabelElement : public UIElement
	{
	public:
		LabelElement(const std::string& text, const u32 characterSize, const sf::Vector2f anchor);

		void SetCenter(const sf::Vector2f anchor);

		void Draw(sf::RenderTarget& target) const override;

	private:
		sf::Text text_;
	};
}
