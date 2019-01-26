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
const int WIN_SCORE = 100;

struct KeyConfig {
    sf::Keyboard::Key up;
    sf::Keyboard::Key down;
    sf::Keyboard::Key left;
    sf::Keyboard::Key right;
};

// SFML doesn't support scancodes...
// #define COLEMAK
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


int main() {
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "~");

    sf::Font font;
    font.loadFromFile("arial.ttf");

    Player players[] = {
        Player(0, sf::Color(255, 100, 100), sf::Vector2f{WINDOW_MARGIN, WINDOW_MARGIN}),
        Player(1, sf::Color(100, 255, 100), sf::Vector2f{WINDOW_MARGIN, WINDOW_HEIGHT - HOUSE_HEIGHT - WINDOW_MARGIN}),
        Player(2, sf::Color(100, 100, 255), sf::Vector2f{WINDOW_WIDTH - HOUSE_WIDTH - WINDOW_MARGIN, WINDOW_MARGIN}),
        Player(3, sf::Color(255, 255, 100), sf::Vector2f{WINDOW_WIDTH - HOUSE_WIDTH - WINDOW_MARGIN,
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

        const float PLAYER_SPEED = 80.f;
        for (size_t i = 0; i < 4; i++) {
            if (sf::Keyboard::isKeyPressed(PLAYER_KEYS[i].up)) {
                players[i].move(0.f, -PLAYER_SPEED * dt);
            }
            if (sf::Keyboard::isKeyPressed(PLAYER_KEYS[i].down)) {
                players[i].move(0.f, PLAYER_SPEED * dt);
            }
            if (sf::Keyboard::isKeyPressed(PLAYER_KEYS[i].left)) {
                players[i].move(-PLAYER_SPEED * dt, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(PLAYER_KEYS[i].right)) {
                players[i].move(PLAYER_SPEED * dt, 0.f);
            }

            if (players[i].carried_item == nullptr) {
                auto boundingBox = players[i].shape.getGlobalBounds();
                for (Item* item : items) {

                    if (!item->being_carried && boundingBox.intersects(item->shape.getGlobalBounds())) {
                        players[i].carried_item = item;

                        // TODO maybe remove
                        item->shape.setPosition(players[i].shape.getPosition());
                        item->being_carried = true;
                        break;
                    }
                }
            }
        }

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
