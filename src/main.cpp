// Developed by Sebastian Lara.

#include <SFML/Graphics.hpp>
#include "Application.hpp"
#include "FlappyBirdWorld.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 480, 600 }),
		"Flappy bird clone by Sebastian Lara!",
		sf::Style::Titlebar | sf::Style::Close // No resizing.
	);
	window.setFramerateLimit(60U);

	App<FlappyBirdGame::World> app{ window };

	app.Run();

	return 0;
}
