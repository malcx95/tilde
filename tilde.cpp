#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "player.hpp"
#include "constants.hpp"
#include "powerup.hpp"
#include "item.hpp"

const unsigned int ITEM_SCORE = 10;
const float PLAYER_SPEED = 80.f;
const int WIN_SCORE = 100;
const float ITEM_SPAWN_INTERVAL = 5.0f;
const float POWERUP_SPAWN_INTERVAL = 1.0;
const float STUN_TIME = 3;


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
        if (p.stunned) continue;

        float speed = p.carried_item ? PLAYER_SPEED / 2.0f : PLAYER_SPEED;

        if (sf::Keyboard::isKeyPressed(p.key_config.up)) {
            p.move(0.f, -speed * dt);
        }
        if (sf::Keyboard::isKeyPressed(p.key_config.down)) {
            p.move(0.f, speed * dt);
        }
        if (sf::Keyboard::isKeyPressed(p.key_config.left)) {
            p.move(-speed * dt, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(p.key_config.right)) {
            p.move(speed * dt, 0.f);
        }

        // Check screen bounds
        auto pos = p.shape.getPosition();
        if (pos.x < 10) {
            p.move(10 - pos.x, 0);
        } else if (pos.x >= WINDOW_WIDTH - 10) {
            p.move(WINDOW_WIDTH - 10 - pos.x, 0);
        }

        if (pos.y < 10) {
            p.move(0, 10 - pos.y);
        } else if (pos.y >= WINDOW_HEIGHT - 10) {
            p.move(0, WINDOW_HEIGHT - 10 - pos.y);
        }
    }
}


void handle_item_pickup(std::vector<Player>& players,
                        std::vector<Item*>& items) {
    for (Player& p : players) {
        if (p.carried_item == nullptr) {
            auto boundingBox = p.shape.getGlobalBounds();
            for (Item* item : items) {

                if (!item->being_carried && 
                        boundingBox.intersects(item->shape.getGlobalBounds())) {
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


void handle_item_stealing(std::vector<Player>& players) {
    for (Player& p : players) {
        auto player_bounds = p.shape.getGlobalBounds();

        for (Player& enemy : players) {
            if (p.index == enemy.index) continue;

            if (player_bounds.intersects(enemy.shape.getGlobalBounds())) {
                if (p.carried_item != nullptr && !enemy.stunned && enemy.carried_item == nullptr) {
                    enemy.carried_item = p.carried_item;
                    enemy.carried_item->shape.setPosition(enemy.shape.getPosition());
                    p.carried_item = nullptr;

                    p.stun_clock.restart().asSeconds();
                    p.stunned = true;
                }
            }
        }
    }
}

void handle_stun(std::vector<Player>& players) {
    for (Player& p : players) {
        if (p.stunned) {
            if (p.stun_clock.getElapsedTime().asSeconds() >= STUN_TIME) {
                p.stunned = false;
            }
        }
    }
}

void remove_powerup(std::vector<Powerup*>& powerups, Powerup* powerup) {
    size_t index = 0;
    for (size_t i = 0; i < powerups.size(); ++i) {
        if (powerups[i] == powerup) {
            index = i;
            break;
        }
    }
    powerups.erase(powerups.begin() + index);
    delete powerup;
}

bool is_free_to_place(Powerup* powerup, 
        std::vector<Player>& players) {
    for (Player& p : players) {
        if (p.house.getGlobalBounds().intersects(
                    powerup->shape.getGlobalBounds())) {
            return false;
        }
    }
    return true;
}

void spawn_powerup(std::vector<Powerup*>& powerups, 
        std::vector<Player>& players) {
    if (powerups.size() < MAX_NUM_POWERUPS) {
        PowerupType type = (PowerupType)(rand() % NUM_POWERUP_TYPES);
        Powerup* p = new Powerup{type, sf::Vector2f{0, 0}};
        unsigned int rand_x;
        unsigned int rand_y;
        do {
            rand_x = (unsigned int)(rand() % WINDOW_WIDTH);
            rand_y = (unsigned int)(rand() % WINDOW_HEIGHT);
            p->shape.setPosition(rand_x, rand_y);
        } while (!is_free_to_place(p, players));
        std::cout << "Spawned powerup at " << rand_x << ", " << rand_y << std::endl;
        powerups.push_back(p);
    }
}

void handle_powerup_pickup(std::vector<Powerup*>& powerups, std::vector<Player>& players) {
    for (Player& p : players) {
        if (p.powerup == nullptr) {
            auto bb = p.shape.getGlobalBounds();
            for (Powerup* powerup : powerups) {

                if (!powerup->active && 
                        bb.intersects(powerup->shape.getGlobalBounds())) {
                    p.powerup = powerup;
                    p.powerup->activate();
                    std::cout << "Picked up powerup" << std::endl;
                    break;
                }
            }
        }
    }
}

void update_powerups(std::vector<Powerup*>& powerups, std::vector<Player>& players) {
    for (Player& p : players) {
        if (p.powerup == nullptr) continue;
        if (p.powerup->should_deactivate()) {
            remove_powerup(powerups, p.powerup);
            p.powerup = nullptr;
            std::cout << "Removed powerup" << std::endl;
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

    std::vector<Item*> items;
    std::vector<Powerup*> powerups;

    sf::Clock delta_clock;
    sf::Clock spawn_clock;
    sf::Clock powerup_clock;

    while (window.isOpen()) {
        float dt = delta_clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (spawn_clock.getElapsedTime().asSeconds() > ITEM_SPAWN_INTERVAL) {
            // defined in item.hpp
            spawn_item(items);
            spawn_clock.restart();
        }

        if (powerup_clock.getElapsedTime().asSeconds() > POWERUP_SPAWN_INTERVAL) {
            // defined in item.hpp
            spawn_powerup(powerups, players);
            powerup_clock.restart();
        }

        handle_input(players, dt);

        handle_item_pickup(players, items);
        handle_powerup_pickup(powerups, players);
        update_powerups(powerups, players);

        handle_item_stealing(players);

        handle_stun(players);

        for (Player& p : players) {
            if (p.carried_item != nullptr && p.is_home()) {
                p.score += ITEM_SCORE;

                // defined in item.hpp
                remove_item(items, p.carried_item);
                p.carried_item = nullptr;

                // Fill a random box
                std::vector<size_t> available_indices;
                for (size_t i = 0; i < p.boxes.size(); i++) {
                    if (!p.boxes[i].filled) {
                        available_indices.push_back(i);
                    }
                }
                size_t empty_boxes = available_indices.size();
                if (empty_boxes > 1) {
                    p.boxes[available_indices[rand() % empty_boxes]].filled = true;
                }
            }
            if (p.score >= WIN_SCORE) {
                std::cout << "player " << p.index << " won the game" << std::endl;
            }
        }

        // Drawing
        window.clear(sf::Color::Black);
        for (int i = 0; i < 4; i++) {
            window.draw(players[i].house);

            for (auto box : players[i].boxes) {
                window.draw(box.shape);

                if (box.filled) {
                    sf::CircleShape shape(10, 3);
                    shape.setPosition(box.shape.getPosition());
                    shape.setFillColor(sf::Color(200, 255, 255));
                    window.draw(shape);
                }
            }

            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(players[i].score));
            text.setCharacterSize(50);
            text.setPosition(players[i].house.getPosition() + sf::Vector2f(0, 15));
            window.draw(text);
        }
        for (auto p : players) {
            window.draw(p.shape);
        }
        for (auto item : items) {
            window.draw(item->shape);
        }
        for (auto powerup : powerups) {
            if (!powerup->active) {
                window.draw(powerup->shape);
            }
        }

        window.display();
    }
    return 0;
}
