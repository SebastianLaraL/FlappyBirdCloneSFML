#pragma once
#include <SFML/Graphics.hpp>
#include "UIElement.hpp"

namespace FlappyBirdGame
{
	// Filled rectangle in screen space. 'center' is the center of the rectangle.
	class PanelElement : public UIElement
	{
	public:
		PanelElement(const sf::Vector2f center, const sf::Vector2f size, const sf::Color fill, const sf::Color outline);

		void Draw(sf::RenderTarget& target) const override;

	private:
		sf::RectangleShape shape_;
	};
}
