#pragma once
#include <SFML/Graphics.hpp>
#include "Types.hpp"

// Base of everything drawn in screen space (score, buttons, panels...).
// Of all aspects of 2D game dev I think UI and screen space are the areas where I have the least knowledge and the greatest room for improvement.
// So even though this implementation may work for now, it's very likely that this module will change in the future.
class UIElement
{
public:
	virtual ~UIElement() = default;

	virtual void Update(const f32 deltaTime) {}
	virtual void Draw(sf::RenderTarget& target) const = 0;

	void SetVisible(const bool visible) { bVisible_ = visible; }
	bool IsVisible() const { return bVisible_; }

private:
	bool bVisible_ = true;
};
