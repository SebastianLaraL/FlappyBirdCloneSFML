#include "Application.hpp"
#include <cassert>
#include <optional>
#include <stdexcept>

#ifdef GAME_DEBUG
#include "imgui.h"
#include "imgui-SFML.h"
#endif // GAME_DEBUG




Application::Application(sf::RenderWindow& window) : window_(window)
{
#ifdef GAME_DEBUG
	if (!ImGui::SFML::Init(window))
		throw std::runtime_error("ImGui-SFML FAILED TO INITIALIZE.");
#endif // GAME_DEBUG
}

Application::~Application()
{
#ifdef GAME_DEBUG
	ImGui::SFML::Shutdown();
#endif // GAME_DEBUG
}

void Application::Run()
{
	assert(world_);

	while (window_.isOpen())
	{
		// Handle events
		while (const std::optional event = window_.pollEvent())
		{
#ifdef GAME_DEBUG
			ImGui::SFML::ProcessEvent(window_, *event);
#endif // GAME_DEBUG
			// Close window event.
			if (event->is<sf::Event::Closed>())
				window_.close();

			world_->HandleInput(event);
		}
		const auto time = deltaTimeClock.restart();
		const f32 deltaTime = time.asSeconds();

#ifdef GAME_DEBUG
		// IMGUI drawing.
		ImGui::SFML::Update(window_, time);
		ImGui::Begin("BEGIN IMGUI");
		DrawImGuiStuff();
		world_->OnImGuiUpdateValues();
		ImGui::End();
#endif // GAME_DEBUG

		world_->Update(deltaTime);

		window_.clear();
		// Draw.

		world_->RenderAll(window_);
#ifdef GAME_DEBUG
		world_->RenderDebug(window_);
		ImGui::SFML::Render(window_);
#endif // GAME_DEBUG

		window_.display();
	}
}

void Application::DrawImGuiStuff() const
{
	//
}
