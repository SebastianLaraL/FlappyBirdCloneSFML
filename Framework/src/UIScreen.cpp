#include "UIScreen.hpp"

void UIScreen::Update(const f32 deltaTime)
{
	if (!bVisible_) return;
	for (auto& element : elements_)
		element->Update(deltaTime);
}

void UIScreen::Draw(sf::RenderTarget& target) const
{
	if (!bVisible_) return;
	for (const auto& element : elements_)
	{
		if (element->IsVisible())
		{
			element->Draw(target);
		}
	}
}
