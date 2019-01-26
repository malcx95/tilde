#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "player.hpp"
#include "constants.hpp"
#include "item.hpp"

const unsigned int ITEM_SCORE = 10;
const unsigned int NUM_PLAYERS = 4;
const float PLAYER_SPEED = 80.f;
const int WIN_SCORE = 100;


// SFML doesn't support scancodes...
#ifdef COLEMAK
KeyConfig PLAYER_KEYS[] = {
    KeyConfig { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right },
    KeyConfig { sf::Keyboard::W, sf::Keyboard::R, sf::Keyboard::A, sf::Keyboard::S },
    KeyConfig { sf::Keyboard::G, sf::Keyboard::D, sf::Keyboard::T, sf::Keyboard::H },
    KeyConfig { sf::Keyboard::U, sf::Keyboard::E, sf::Keyboard::N, sf::Keyboard::I }
};
#else
KeyConfig PLAYER_KEYS[] = {
    KeyConfig { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right },
    KeyConfig { sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D },
    KeyConfig { sf::Keyboard::I, sf::Keyboard::K, sf::Keyboard::J, sf::Keyboard::L },
    KeyConfig { sf::Keyboard::T, sf::Keyboard::G, sf::Keyboard::F, sf::Keyboard::H }
};
#endif


void handle_input(std::vector<Player>& players, float dt) {
    for (Player& p : players) {
        if (sf::Keyboard::isKeyPressed(p.key_config.up)) {
            p.move(0.f, -PLAYER_SPEED * dt);
        }
        if (sf::Keyboard::isKeyPressed(p.key_config.down)) {
            p.move(0.f, PLAYER_SPEED * dt);
        }
        if (sf::Keyboard::isKeyPressed(p.key_config.left)) {
            p.move(-PLAYER_SPEED * dt, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(p.key_config.right)) {
            p.move(PLAYER_SPEED * dt, 0.f);
        }
    }
}


void handle_item_pickup(std::vector<Player>& players, 
        std::vector<Item*>& items) {
    for (Player& p : players) {
        if (p.carried_item == nullptr) {
            auto boundingBox = p.shape.getGlobalBounds();
            for (Item* item : items) {

                if (!item->being_carried && boundingBox.intersects(item->shape.getGlobalBounds())) {
                    p.carried_item = item;

                    // TODO maybe remove
                    item->shape.setPosition(p.shape.getPosition());
                    item->being_carried = true;
                    break;
                }
            }
        }
    }
}


int main() {
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "~");

    sf::Font font;
    font.loadFromFile("arial.ttf");

    std::vector<Player> players = {
        Player(0, sf::Color(255, 100, 100), PLAYER_KEYS[0],
                sf::Vector2f{WINDOW_MARGIN, WINDOW_MARGIN}),
        Player(1, sf::Color(100, 255, 100), PLAYER_KEYS[1],
                sf::Vector2f{WINDOW_MARGIN, WINDOW_HEIGHT - HOUSE_HEIGHT - WINDOW_MARGIN}),
        Player(2, sf::Color(100, 100, 255), PLAYER_KEYS[2],
                sf::Vector2f{WINDOW_WIDTH - HOUSE_WIDTH - WINDOW_MARGIN, WINDOW_MARGIN}),
        Player(3, sf::Color(255, 255, 100), PLAYER_KEYS[3],
                sf::Vector2f{WINDOW_WIDTH - HOUSE_WIDTH - WINDOW_MARGIN,
        WINDOW_HEIGHT - HOUSE_HEIGHT - WINDOW_MARGIN})
    };

    float spawnInterval = 5.0f;
    std::vector<Item*> items;

    sf::Clock deltaClock;
    sf::Clock spawnClock;

    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
            // defined in item.hpp
            spawn_item(items);
            spawnClock.restart();
        }

        handle_input(players, dt);

        handle_item_pickup(players, items);

        for (Player& p : players) {
            if (p.carried_item != nullptr && p.is_home()) {
                p.score += ITEM_SCORE;

                // defined in item.hpp
                remove_item(items, p.carried_item);
                p.carried_item = nullptr;
            }
            if (p.score >= WIN_SCORE) {
                std::cout << "player " << p.index << " won the game" << std::endl;
            }
        }

        // Drawing
        window.clear(sf::Color::Black);
        for (int i = 0; i < 4; i++) {
            window.draw(players[i].house);

            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(players[i].score));
            text.setCharacterSize(50);
            text.setPosition(players[i].house.getPosition());
            window.draw(text);
        }
        for (auto p : players) {
            window.draw(p.shape);
        }
        for (auto item : items) {
            window.draw(item->shape);
        }

        window.display();
    }
    return 0;
}
