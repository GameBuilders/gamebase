#include <SFML/Graphics.hpp>

// Program entry point
int main(int argc, char ** argv) {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game!");

    sf::Event event;    // For polling events that SFML sends us

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

        // Notify the window that we're ready to render
        window.display();
    }

    return 0;
}
