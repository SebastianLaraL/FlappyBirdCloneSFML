#include "World.hpp"
#include <algorithm>
#include "Entity.hpp"
#include "UIScreen.hpp"

World::World(sf::RenderWindow& window) : window_(window)
{
}

World::~World() = default;

void World::Update(const f32 deltaTime)
{
	for (auto& ent : entities_)
	{
		ent->Update(deltaTime);
	}
	for (auto& screen : screens_)
	{
		screen->Update(deltaTime);
	}
}

void World::HandleInput(const std::optional<sf::Event>& event)
{
}

void World::OnImGuiUpdateValues()
{
}

void World::RenderDebug(sf::RenderWindow& window) const
{
}

void World::RenderAll(sf::RenderWindow& window) const
{
	struct RenderItem
	{
		const sf::Sprite* sprite;
		u32 depth;
	};

	// This sorting-every-frame design is not optimal, but will work for the
	// current project.
	// TODO: implement optimization like dirty flag.
	std::vector<RenderItem> items;
	items.reserve(entities_.size() + additionalSprites_.size());

	for (const auto& ent : entities_)
		items.push_back({ &ent->GetSprite(), ent->GetRenderOrder() });
	for (const auto& [sprite, depth] : additionalSprites_)
		items.push_back({ sprite, depth });

	std::stable_sort(items.begin(), items.end(), [](const RenderItem& a, const RenderItem& b)
		{
			return a.depth > b.depth;
		});

	for (const auto& item : items)
	{
		window.draw(*item.sprite);
		// LOG std::cout << item.depth << std::endl;
	}

	// UI elements.
	const sf::View previousView = window.getView();
	window.setView(window.getDefaultView());
	for (const auto& screen : screens_)
	{
		screen->Draw(window);
	}
	window.setView(previousView);
}

void World::AddAdditionalSprite(const sf::Sprite* sprite, u32 renderDepth)
{
	additionalSprites_.push_back({ sprite, renderDepth });
}
