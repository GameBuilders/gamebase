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

float y_delta = 0.5;

void render(sf::RenderWindow& window){
    int max_e = game.enemy.size();
    int max_b = game.bullet.size();
    int max_bp = game.player_bullet.size();

    for (int i = 0; i < max_b; i++){
        sf::Sprite _bullet;
        _bullet.setTexture(bulletTexture);
        game.bullet[i].change_position(game.bullet[i].get_x(), game.bullet[i].get_y() - y_delta);
        _bullet.setPosition(game.bullet[i].get_x(), game.bullet[i].get_y());
        window.draw(_bullet);
    }

    //render player bullets
    sf::Texture player_bullet;
    player_bullet.loadFromFile("../src/sprites/bullet_player.png");

    for (int j = 0; j < max_bp; j++){
        sf::Sprite _bullet_p;
        _bullet_p.setTexture(player_bullet);
        game.player_bullet[j].change_position(game.player_bullet[j].get_x(), game.player_bullet[j].get_y() - y_delta);
        _bullet_p.setPosition(game.player_bullet[j].get_x(), game.player_bullet[j].get_y());
        window.draw(_bullet_p);
    }

    for (int k = 0; k < max_e; k++){
        if (game.enemy[k].entry_flag == 1){
            sf::Sprite _enemy;
            _enemy.setTexture(enemyTexture);
	        game.enemy[k].change_position(game.enemy[k].get_x(), game.enemy[k].get_y() - y_delta);
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

void render_game_over(sf::RenderWindow& window, float y){
    sf::Font font;
    font.loadFromFile("../src/DejaVuSans.ttf");
    // Create a text
    sf::Text text("Game Over", font);
    text.setCharacterSize(100);
    text.setPosition(100,y);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::Red);
    // Draw it
    window.draw(text);
}

void makeLevel(float offset) {
    // The entire level is here:
    game.add_enemy("n", sf::seconds(0.5f),
                      0, offset-1000, 0.1, 0, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(1.0f),
                      0, offset-2000, 0.1, 0, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(1.5f),
                      0, offset-3000, 0.1, 0, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(2.0f),
                      0, offset-4000, 0.1, 0, 20, "cir", 10);

    game.add_enemy("n", sf::seconds(3.0f),
                      800, offset-500, -0.1, 0, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(3.5f),
                      800, offset-1500, -0.1, 0, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(4.0f),
                      800, offset-2500, -0.1, 0, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(4.5f),
                      800, offset-3500, -0.1, 0, 20, "cir", 10);

    game.add_enemy("n", sf::seconds(6.5f),
                      100, offset-5500, 0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(7.0f),
                      100, offset-5500, 0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(7.5f),
                      100, offset-5500, 0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(8.0f),
                      100, offset-5500, 0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(8.5f),
                      100, offset-5500, 0.05, 0.3, 20, "cir", 10);

    game.add_enemy("n", sf::seconds(9.0f),
                      700, offset-5500, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(9.5f),
                      700, offset-5500, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(10.0f),
                      700, offset-5500, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(10.5f),
                      700, offset-5500, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(11.0f),
                      700, offset-5500, -0.05, 0.3, 20, "cir", 10);


    game.add_enemy("n", sf::seconds(12.0f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(12.5f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(13.0f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(13.5f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(14.0f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(15.5f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(16.0f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(16.5f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(17.0f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(17.5f),
                      0, offset-4000, 0.05, 0.3, 20, "burst", 10);


    game.add_enemy("n", sf::seconds(18.0f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(18.5f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(19.0f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(19.5f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(20.0f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(20.5f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(21.0f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(21.5f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(22.0f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(22.5f),
                      750, offset-6000, -0.05, 0.3, 20, "cir", 10);


    game.add_enemy("n", sf::seconds(23.0f),
                      350, offset-8000, -0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(23.5f),
                      350, offset-8000, 0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(24.0f),
                      350, offset-8000, -0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(24.5f),
                      350, offset-8000, 0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(25.0f),
                      350, offset-8000, -0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(25.5f),
                      350, offset-8000, 0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(26.0f),
                      350, offset-8000, -0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(26.5f),
                      350, offset-8000, 0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(27.0f),
                      350, offset-8000, -0.05, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(27.5f),
                      350, offset-8000, 0.05, 0.3, 20, "bomb", 10);

    game.add_enemy("n", sf::seconds(28.0f),
                      200, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(28.5f),
                      200, offset-8000, 0, -0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(29.0f),
                      200, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(29.5f),
                      200, offset-8000, 0, -0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(30.0f),
                      200, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(30.5f),
                      200, offset-8000, 0, -0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(31.0f),
                      200, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(31.5f),
                      200, offset-8000, 0, -0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(32.0f),
                      200, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(32.5f),
                      200, offset-8000, 0, -0.3, 20, "bomb", 10);


    game.add_enemy("n", sf::seconds(28.0f),
                      500, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(28.5f),
                      500, offset-8000, 0, -0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(29.0f),
                      500, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(29.5f),
                      500, offset-8000, 0, -0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(30.0f),
                      500, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(30.5f),
                      500, offset-8000, 0, -0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(31.0f),
                      500, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(31.5f),
                      500, offset-8000, 0, -0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(32.0f),
                      500, offset-8000, 0, 0.3, 20, "bomb", 10);
    game.add_enemy("n", sf::seconds(32.5f),
                      500, offset-8000, 0, -0.3, 20, "bomb", 10);

    game.add_enemy("n", sf::seconds(32.5f),
                      50, offset-9000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(33.0f),
                      50, offset-9000, 0, -0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(33.5f),
                      50, offset-9000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(34.0f),
                      50, offset-9000, 0, -0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(34.5f),
                      50, offset-9000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(35.0f),
                      50, offset-9000, 0, -0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(35.5f),
                      50, offset-9000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(36.0f),
                      50, offset-9000, 0, -0.3, 20, "burst", 10);
    game.add_enemy("n", sf::seconds(36.5f),
                      50, offset-9000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(37.0f),
                      50, offset-9000, 0, -0.3, 20, "burst", 10);


    game.add_enemy("n", sf::seconds(32.5f),
                      650, offset-10000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(33.0f),
                      650, offset-10000, 0, -0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(33.5f),
                      650, offset-10000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(34.0f),
                      650, offset-10000, 0, -0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(34.5f),
                      650, offset-10000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(35.0f),
                      650, offset-10000, 0, -0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(35.5f),
                      650, offset-10000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(36.0f),
                      650, offset-10000, 0, -0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(36.5f),
                      650, offset-10000, 0, 0.3, 20, "cir", 10);
    game.add_enemy("n", sf::seconds(37.0f),
                      650, offset-10000, 0, -0.3, 20, "cir", 10);


    game.add_enemy("n", sf::seconds(34.5f),
                      650, offset-10000, -0.5, 0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(35.0f),
                      50, offset-10000, 0.5, -0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(35.5f),
                      650, offset-10000, -0.5, 0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(36.0f),
                      50, offset-10000, 0.5, -0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(36.5f),
                      650, offset-10000, -0.5, 0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(37.0f),
                      50, offset-10000, 0.5, -0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(37.5f),
                      650, offset-10000, -0.5, 0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(38.0f),
                      50, offset-10000, 0.5, -0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(38.5f),
                      650, offset-10000, -0.5, 0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(39.0f),
                      50, offset-10000, 0.5, -0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(39.5f),
                      650, offset-10000, -0.5, 0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(40.0f),
                      50, offset-10000, 0.5, -0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(40.5f),
                      650, offset-10000, -0.5, 0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(41.0f),
                      50, offset-10000, 0.5, -0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(41.5f),
                      650, offset-10000, -0.5, 0.03, 50, "cir", 10);
    game.add_enemy("n", sf::seconds(42.0f),
                      50, offset-10000, 0.5, -0.03, 50, "cir", 10);
    // level end;
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
    size_t backgroundHeight = 120000;
   	background.setTextureRect(sf::IntRect(0,0,800,backgroundHeight));
    background.setScale((float) screenDimensions.x / bgTexture.getSize().x, 1.0f);

    float offset = backgroundHeight - screenDimensions.y;
    makeLevel(offset);

    // clocks for trigger all the stuff
    sf::Clock global_clock; // global clock that is only reset in the end

    sf::View view;
    view.reset(sf::FloatRect(0, 0, screenDimensions.x, screenDimensions.y));
    view.setViewport(sf::FloatRect(0, 0, 0.75f, 1.0f));
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
		    player_x -= 0.45f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && game.player.get_x() + playerTexture.getSize().x < screenDimensions.x) {
		    player_x += 0.45f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && game.player.get_y() > position.y - screenDimensions.y / 2) {
		    player_y -= 0.40f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && game.player.get_y() + playerTexture.getSize().y < position.y + screenDimensions.y / 2) {
		    player_y += 0.68f;
		}

        position.y -= y_delta;
        player_y -= y_delta;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		        game.super_gm = true;
		}

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
        if (game.game_over == false)
            render(window);
        else{
            if (game.super_gm == false){
                render_game_over(window, position.y);
                y_delta = 0;
            }
            else
                render(window);
        }

        // Display coordinates of player
        window.setView(view2);
        text.setString(std::to_string(game.player.get_x()) + ", " + std::to_string(game.player.get_y()));
        window.draw(text);

	    // for (unsigned int j = 0; j < game.enemy.size(); j++){
	    //     sf::Text text2(std::to_string(game.enemy[j].get_x()) + ", " + std::to_string(game.enemy[j].get_y()), font, 30);
	    //     text2.setPosition(sf::Vector2f(0, j*30 + 30));
	    //     window.draw(text2);
	    // }

        sf::Text text2("Lives: 1", font, 30);
        text2.setPosition(sf::Vector2f(0, 30));
        window.draw(text2);

        // Notify the window that we're ready to render
        window.display();
    }

    return EXIT_SUCCESS;
}
