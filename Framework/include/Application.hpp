#pragma once
#include <memory>
#include <type_traits>
#include <utility>
#include <SFML/Graphics.hpp>
#include "World.hpp"

// Non-template part of the application: window loop, ImGui and world ownership.
// LEARNING NOTE: separation of concerns and
// Split application logic in one class and the template class in
// a separate class to avoid code duplication with every different world class created.
class Application
{
public:
	virtual ~Application();

	// Loop.
	void Run();

	inline World* GetWorld() { return world_.get(); }

protected:
	explicit Application(sf::RenderWindow& window);

	// Override this to draw imgui elements. Begin is called before and IMGUI::END after, so it's safe.
	virtual void DrawImGuiStuff() const;

	sf::RenderWindow& window_;
	std::unique_ptr<World> world_;

private:
	sf::Clock deltaTimeClock;
};


template<typename TWorld>
class App : public Application
{
public:
	template<typename... Args>
	explicit App(sf::RenderWindow& window, Args&&... worldArgs) : Application(window)
	{
		world_ = CreateWorld(std::forward<Args>(worldArgs)...);
	}

	template<typename... Args>
	std::unique_ptr<TWorld> CreateWorld(Args&&... worldArgs)
	{
		static_assert(std::is_base_of_v<World, TWorld>, "TWorld must derive from World");
		return std::make_unique<TWorld>(window_, std::forward<Args>(worldArgs)...);
	}

	// Maybe I won't use this in this project, but maybe needed in future projects.
	template<typename... Args>
	void ChangeLevel(Args&&... args)
	{
		world_.reset();
		world_ = CreateWorld(std::forward<Args>(args)...);
	}
};
