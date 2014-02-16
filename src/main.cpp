#include <SFML/Graphics.hpp>

//local includes
#include "bullet.h"
#include "objects.h"
#include "level.h"

//stores all the acutal game data
Level game;

sf::Texture bgTexture;
sf::Texture playerTexture;
sf::Texture bulletTexture;
sf::Texture enemyTexture;

void render(sf::RenderWindow& window){
    int max_e = game.enemy.size();
    int max_b = game.bullet.size();
    int max_bp = game.player_bullet.size();

    // cout << "render " << max_bp << endl;

    for (int i = 0; i < max_b; i++){
        sf::Sprite _bullet;
        _bullet.setTexture(bulletTexture);
        game.bullet[i].change_position(game.bullet[i].get_x(), game.bullet[i].get_y() - 0.05);
        _bullet.setPosition(game.bullet[i].get_x(), game.bullet[i].get_y());
        window.draw(_bullet);
    }

    //render player bullets
    for (int j = 0; j < max_bp; j++){
        sf::Sprite _bullet_p;
        _bullet_p.setTexture(bulletTexture);
        game.player_bullet[j].change_position(game.player_bullet[j].get_x(), game.player_bullet[j].get_y() - 0.05);
        _bullet_p.setPosition(game.player_bullet[j].get_x(), game.player_bullet[j].get_y());
        window.draw(_bullet_p);

        // cout << game.player_bullet.back().get_y() << endl;
    }

    for (int k = 0; k < max_e; k++){
        if (game.enemy[k].entry_flag == 1){
            sf::Sprite _enemy;
            _enemy.setTexture(enemyTexture);
	        game.enemy[k].change_position(game.enemy[k].get_x(), game.enemy[k].get_y() - 0.05);
            _enemy.setPosition(game.enemy[k].get_x(), game.enemy[k].get_y());
            window.draw(_enemy);
        }
    }

    //render the player last
    sf::Sprite _player;
    _player.setTexture(playerTexture);
    _player.setPosition(game.player.get_x(), game.player.get_y());
    window.draw(_player);
}

// Program entry point
int main(int argc, char ** argv) {
    // Create the SFML window
    sf::Vector2i screenDimensions(800, 600);
    sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Zero Requiem");

    sf::Font font;
    if (!font.loadFromFile("../src/DejaVuSans.ttf"))
        return EXIT_FAILURE;
    sf::Text text("", font, 30);

    // Load player sprite to display
    if (!playerTexture.loadFromFile("../src/sprites/gundam_flying_up.png"))
        return EXIT_FAILURE;

    // Load bullet sprites
    if (!bulletTexture.loadFromFile("../src/sprites/bullet.png"))
    	return EXIT_FAILURE;

    // Load enemy sprites
    if (!enemyTexture.loadFromFile("../src/sprites/enemy.png"))
    	return EXIT_FAILURE;

    // Load the background
    if (!bgTexture.loadFromFile("../src/sprites/background.png"))
        return EXIT_FAILURE;
   	bgTexture.setRepeated(true);
    sf::Sprite background(bgTexture);
    size_t backgroundHeight = 12000;
   	background.setTextureRect(sf::IntRect(0,0,800,backgroundHeight));
    background.setScale((float) screenDimensions.x / bgTexture.getSize().x, 1.0f);

    // The entire level is here:
    game.add_enemy("test", sf::seconds(0.5f),
                      0, backgroundHeight - screenDimensions.y + 100, 0.1, 0, 20, "cir", 10);
    game.add_enemy("test", sf::seconds(1.0f),
                      0, backgroundHeight - screenDimensions.y + 200, 0.1, 0, 20, "burst", 10);
    game.add_enemy("test", sf::seconds(1.5f),
                      0, backgroundHeight - screenDimensions.y + 300, 0.1, 0, 20, "bomb", 10);
    game.add_enemy("test", sf::seconds(2.0f),
                      0, backgroundHeight - screenDimensions.y + 400, 0.1, 0, 20, "cir", 10);
    game.add_enemy("test", sf::seconds(2.5f),
                      0, backgroundHeight - screenDimensions.y + 50, 0.1, 0, 20, "dir_bomb", 10);
    // level end;

    // clocks for trigger all the stuff
    sf::Clock global_clock; // global clock that is only reset in the end

    sf::View view;
    view.reset(sf::FloatRect(0, 0, screenDimensions.x, screenDimensions.y));
    view.setViewport(sf::FloatRect(0, 0, 0.6f, 1.0f));
    // view.setSize(screenDimensions.x * 3 / 5, screenDimensions.y);

	sf::View view2;
	view2.setViewport(sf::FloatRect(0.6f, 0, 1.0f, 1.0f));
    // view2.setSize(screenDimensions.x * 2 / 5, screenDimensions.y);

    sf::Vector2f position(bgTexture.getSize().x - screenDimensions.x / 2, backgroundHeight - screenDimensions.y / 2);
    game.player.change_position(bgTexture.getSize().x - screenDimensions.x / 2 - playerTexture.getSize().x / 2, backgroundHeight - screenDimensions.y / 2 + 100);

    // For polling events that SFML sends us
    sf::Event event;

    while (window.isOpen()) {
        // Process any events sent by SFML since the last frame
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // SFML sent us a close event, so clean up
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard:: LControl){
                    if (game.player.fire == false)
                        game.player.fire = true;
                    else
                        game.player.fire = false;
                }
            }
        }

        float player_x = 0;
        float player_y = 0;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && game.player.get_x() > 0) {
		    player_x -= 0.15f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && game.player.get_x() + playerTexture.getSize().x < screenDimensions.x) {
		    player_x += 0.15f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && game.player.get_y() > position.y - screenDimensions.y / 2) {
		    player_y -= 0.09f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && game.player.get_y() + playerTexture.getSize().y < position.y + screenDimensions.y / 2) {
		    player_y += 0.12f;
		}

        position.y -= 0.05;
        player_y -= 0.05;

        game.player.change_position(game.player.get_x() + player_x, game.player.get_y() + player_y);
		game.add_bullet_player();
		game.damage_judging();
        game.move_stuff(global_clock.getElapsedTime());

        // Clear the window before we start drawing to it
        window.clear();

        view.setCenter(position);
        window.setView(view);

        // Draw the background
        window.draw(background);

        // Render major elements of the window!
        render(window);

        // Display coordinates of player
        window.setView(view2);
        text.setString(std::to_string(game.player.get_x()) + ", " + std::to_string(game.player.get_y()));
        window.draw(text);

	    for (unsigned int j = 0; j < game.enemy.size(); j++){
	        sf::Text text2(std::to_string(game.enemy[j].get_x()) + ", " + std::to_string(game.enemy[j].get_y()), font, 30);
	        text2.setPosition(sf::Vector2f(0, j*30));
	        window.draw(text2);
	    }

        // Notify the window that we're ready to render
        window.display();
    }

    return EXIT_SUCCESS;
}