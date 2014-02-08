/*
 * This is a simple demo highlighting basic usage of the SFML and Box2D libraries.
 * Hopefully this code will help you get started if you're unfamiliar with either.
 * For more advanced usage, be sure to consult the respective libraries' documentation!
 */
 
// Include the SFML and Box2D headers
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

// Function prototypes
void drawShapes(sf::RenderWindow& window);

// Program entry point
int main() {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Crane Game!");

    sf::Event event;	// For polling events that SFML sends us
    sf::Clock clock;	// For keeping track of time

    while (window.isOpen()) {
		// Process any events sent by SFML since the last frame
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
				// SFML sent us a close event, so clean up
				window.close();
            }
        }

		// Clear the window before we start drawing to it
		window.clear();
		
		// Render some shapes
		drawShapes(window);
		
		// Notify the window that we're ready to render
		window.display();
    }
	return 0;
}

// Draws some shapes!
void drawShapes(sf::RenderWindow& window) {
	// Draw a circle to the window
	sf::CircleShape shape(50);
	shape.setFillColor(sf::Color(100, 250, 50));
	window.draw(shape);
}
