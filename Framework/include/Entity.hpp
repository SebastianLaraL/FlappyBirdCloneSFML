#pragma once
#include <utility>
#include <SFML/Graphics.hpp>
#include "Types.hpp"
#include "RenderOrder.hpp"

class Entity
{
public:
	Entity(sf::Sprite sprite, u32 renderOrder = RenderOrder::GAMEPLAY)
		: sprite_(std::move(sprite)), renderOrder_(renderOrder) {
	}
	virtual ~Entity() = default;

	virtual void Update(const f32 deltaTime) = 0;

	void CenterOrigin();

	inline sf::Sprite& GetSprite() { return sprite_; }
	inline u32 GetRenderOrder() const { return renderOrder_; }
	inline auto GetHalfWidth() const { return sprite_.getLocalBounds().size.x / 2; }
	inline auto GetHalfHeight() const { return sprite_.getLocalBounds().size.y / 2; }
protected:
	sf::Sprite sprite_;
private:
	u32 renderOrder_; // TODO: When checking memory layout of classes that derive from Entity, it seems that after this variable there is a 4-byte padding, so the next variable will be aligned to 8 bytes. This is not a problem for this project, but it is something to keep in mind for future projects.
};
