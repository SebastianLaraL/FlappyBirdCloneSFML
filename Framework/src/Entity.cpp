#include "Entity.hpp"

void Entity::CenterOrigin()
{
	const auto bounds = sprite_.getLocalBounds().size;
	sprite_.setOrigin({ bounds.x / 2 , bounds.y / 2 });
}
