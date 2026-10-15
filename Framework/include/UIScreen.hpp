#pragma once
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Types.hpp"
#include "UIElement.hpp"

// A group of UI elements that is shown / hidden as a whole (HUD, start screen, game over...).
class UIScreen
{
public:
	virtual ~UIScreen() = default;

	template<typename T, typename... Args>
	T* Add(Args&&... args)
	{
		static_assert(std::is_base_of_v<UIElement, T>, "T must derive from UIElement.");
		auto element = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = element.get();
		elements_.push_back(std::move(element));
		return ptr;
	}

	virtual void Update(const f32 deltaTime);

	void Draw(sf::RenderTarget& target) const;

	void Show() { bVisible_ = true; }
	void Hide() { bVisible_ = false; }
	bool IsVisible() const { return bVisible_; }

private:
	std::vector<std::unique_ptr<UIElement>> elements_;
	bool bVisible_ = true;
};
