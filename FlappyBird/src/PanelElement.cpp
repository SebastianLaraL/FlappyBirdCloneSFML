#include "PanelElement.hpp"

namespace FlappyBirdGame
{

	PanelElement::PanelElement(const sf::Vector2f center, const sf::Vector2f size, const sf::Color fill, const sf::Color outline)
		: shape_(size)
	{
		shape_.setOrigin(size / 2.f);
		shape_.setPosition(center);
		shape_.setFillColor(fill);
		shape_.setOutlineColor(outline);
		shape_.setOutlineThickness(4.f);
	}

	void PanelElement::Draw(sf::RenderTarget& target) const
	{
		target.draw(shape_);
	}

}
