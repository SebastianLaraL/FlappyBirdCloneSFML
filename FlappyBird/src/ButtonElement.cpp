#include "ButtonElement.hpp"

namespace FlappyBirdGame
{

	ButtonElement::ButtonElement(const std::string& text, const sf::Vector2f center, const sf::Vector2f size)
		: shape_(size), label_(text, 28, center)
	{
		shape_.setOrigin(size / 2.f);
		shape_.setPosition(center);
		shape_.setFillColor(sf::Color(222, 97, 40));
		shape_.setOutlineColor(sf::Color::White);
		shape_.setOutlineThickness(4.f);
	}

	bool ButtonElement::Contains(const sf::Vector2f point) const
	{
		return shape_.getGlobalBounds().contains(point);
	}

	void ButtonElement::Draw(sf::RenderTarget& target) const
	{
		target.draw(shape_);
		label_.Draw(target);
	}

}
