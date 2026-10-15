#pragma once
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Types.hpp"

class Entity;
class UIScreen;

class World
{
public:
	explicit World(sf::RenderWindow& window);
	virtual ~World();

	template<typename T, typename... Args>
	T* Add(Args&&... args)
	{
		static_assert(std::is_base_of_v<Entity, T>, "T must derive from Entity.");
		auto ent = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = ent.get();
		ent->CenterOrigin();
		entities_.push_back(std::move(ent));
		return ptr;
	}

	template<typename T, typename... Args>
	T* AddScreen(Args&&... args)
	{
		static_assert(std::is_base_of_v<UIScreen, T>, "T must derive from UIScreen.");
		auto screen = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = screen.get();
		screens_.push_back(std::move(screen));
		return ptr;
	}

	// Updates all entities.
	virtual void Update(const f32 deltaTime);

	virtual void HandleInput(const std::optional<sf::Event>& event);

	// Helper to modify entities values. Override this.
	virtual void OnImGuiUpdateValues();

	// Personally I don't like the idea of the world
	// being responsible for rendering, but I want to keep this game as
	// small and simple as possible.
	// TODO: rendering in a seperate system in future releases.
#pragma region RENDERING

	virtual void RenderDebug(sf::RenderWindow& window) const;

	// Render everything.
	void RenderAll(sf::RenderWindow& window) const;

	// Register an extra sprite (not the inherited sprite_ from Entity)
	// in order to draw next to everything else, resoecting its depth.
	void AddAdditionalSprite(const sf::Sprite* sprite, u32 renderDepth);

#pragma endregion

	inline auto* GetFirst() const { return entities_.empty() ? nullptr : entities_[0].get(); }
protected:
	sf::RenderWindow& window_; // Easy access to window to call getSize(), etc.
	std::vector<std::unique_ptr<Entity>> entities_;
	std::vector<std::unique_ptr<UIScreen>> screens_;
	std::vector<std::pair<const sf::Sprite*, u32>> additionalSprites_; // Default render loop renders all entities' sprites, but in some cases an entity has several sprites to render.
};
