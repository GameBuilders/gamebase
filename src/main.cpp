#include <SFML/Graphics.hpp>

// Program entry point
int main(int argc, char ** argv) {
    // Create the SFML window
    sf::Vector2i screenDimensions(780, 600);
    sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Zero Requiem");

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("../src/sprites/gundam_flying_up.png"))
        return EXIT_FAILURE;
    sf::Sprite player(texture);

    // Load the background
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("../src/sprites/background_long.png"))
        return EXIT_FAILURE;
    sf::Sprite background(bgTexture);
    background.setScale((float) screenDimensions.x / bgTexture.getSize().x, 1.0f);

    sf::View view;

    view.reset(sf::FloatRect(0, 0, screenDimensions.x, screenDimensions.y));
    view.setViewport(sf::FloatRect(0, 0, 0.6f, 1.0f));

    sf::Vector2f position(bgTexture.getSize().x - screenDimensions.x / 2, bgTexture.getSize().y - screenDimensions.y / 2);
    player.setPosition(sf::Vector2f(bgTexture.getSize().x - screenDimensions.x / 2 - player.getTexture()->getSize().x / 2, bgTexture.getSize().y - screenDimensions.y / 2 + 100));

    // For polling events that SFML sends us
    sf::Event event;

    while (window.isOpen()) {
        // Process any events sent by SFML since the last frame
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // SFML sent us a close event, so clean up
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.getPosition().x > 0) {
		    // left key is pressed: move our character
		    player.move(sf::Vector2f(-0.15, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.getPosition().x + player.getTexture()->getSize().x < screenDimensions.x) {
		    // left key is pressed: move our character
		    player.move(sf::Vector2f(0.15, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player.getPosition().y > position.y - screenDimensions.y / 2) {
		    // left key is pressed: move our character
		    player.move(sf::Vector2f(0, -0.09));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player.getPosition().y + player.getTexture()->getSize().y < position.y + screenDimensions.y / 2) {
		    // left key is pressed: move our character
		    player.move(sf::Vector2f(0, 0.12));
		}

        // Clear the window before we start drawing to it
        window.clear();

        if (position.y > screenDimensions.y / 2) {
	        position.y -= 0.05;
	        player.move(sf::Vector2f(0, -0.05));
	    }
        view.setCenter(position);
        window.setView(view);

        // Draw the background
        window.draw(background);

        // Draw the sprite
        window.draw(player);

        // Notify the window that we're ready to render
        window.display();
    }

    return EXIT_SUCCESS;
}
