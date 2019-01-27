#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "config.h"
#include "player.hpp"
#include "constants.hpp"
#include "powerup.hpp"
#include "item.hpp"
#include "input.hpp"

const float PLAYER_SPEED = 80.f;
const float ITEM_SPAWN_INTERVAL = 5.0f;
const float POWERUP_SPAWN_INTERVAL = 1.0;
const float STUN_TIME = 3;
const float BURN_TIME = 5;


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
        if (p.stunned) {
            p.moving = false;
            continue;
        }

        sf::Vector2f dir;
        if (p.input_handler->get_value(input::Action::UP) > 0.5) {
            dir.y -= 1;
        }
        if (p.input_handler->get_value(input::Action::DOWN) > 0.5) {
            dir.y += 1;
        }
        if (p.input_handler->get_value(input::Action::LEFT) > 0.5) {
            dir.x -= 1;
        }
        if (p.input_handler->get_value(input::Action::RIGHT) > 0.5) {
            dir.x += 1;
        }

        // Prevent diagonal walking from being faster
        if (dir.x != 0 && dir.y != 0) {
            dir /= (float)sqrt(2);
        }

        float speed = p.carried_item ? PLAYER_SPEED / 2.0f : PLAYER_SPEED;

        if (p.powerup != nullptr && p.powerup->type == PowerupType::FASTER) {
            speed *= SPEED_INCREASE;
        }

        auto movement = dir * speed * dt;
        p.move(movement.x, movement.y);

        // Change character direction if moving
        if (dir.y < 0) {
            p.direction = Up;
        } else if (dir.y > 0) {
            p.direction = Down;
        } else if (dir.x < 0) {
            p.direction = Left;
        } else if (dir.x > 0) {
            p.direction = Right;
        }

        p.moving = dir.x != 0 || dir.y != 0;

        // Check screen bounds
        auto pos = p.sprite.getPosition();
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
                        !item->in_box &&
                        boundingBox.intersects(item->sprite.getGlobalBounds())) {
                    p.carried_item = item;

                    item->sprite.setPosition(p.sprite.getPosition());
                    item->being_carried = true;
                    break;
                }
            }
        }
    }
}


void handle_item_stealing(std::vector<Player>& players) {
    for (Player& p : players) {
        if (p.powerup != nullptr &&
                p.powerup->type == PowerupType::IMMUNITY) {
            continue;
        }
        auto player_bounds = p.shape.getGlobalBounds();

        for (Player& enemy : players) {
            if (p.index == enemy.index) continue;

            if (player_bounds.intersects(enemy.shape.getGlobalBounds())) {
                if (p.carried_item != nullptr && !enemy.stunned && enemy.carried_item == nullptr) {
                    enemy.carried_item = p.carried_item;
                    enemy.carried_item->sprite.setPosition(enemy.shape.getPosition());
                    p.carried_item = nullptr;

                    p.stun_clock.restart().asSeconds();
                    p.stunned = true;
                    p.sprite.setRotation(90.0f);
                }
            }
        }
    }
}

void handle_fire(std::vector<Player>& players, std::vector<Item*>& items) {
    for (Player& p : players) {
        // Light items that player walks on
        if (p.powerup != nullptr && p.powerup->type == PowerupType::FIRE) {
            for (Player& other : players) {
                for (Box& box : other.boxes) {
                    auto box_bb = box.shape.getGlobalBounds();
                    auto player_bb = p.shape.getGlobalBounds();
                    if (box.filled && !box.on_fire && box_bb.intersects(player_bb)) {
                        box.on_fire = true;
                        box.fire_clock.restart();
                    }
                }
            }
        }

        for (Box& box : p.boxes) {
            if (box.on_fire) {
                auto box_bb = box.shape.getGlobalBounds();
                auto player_bb = p.shape.getGlobalBounds();

                // Let players put out fires on their own stuff
                if (box_bb.intersects(player_bb)) {
                    box.on_fire = false;
                }

                if (box.fire_clock.getElapsedTime().asSeconds() > BURN_TIME) {
                    box.filled = false;
                    box.on_fire = false;
                    remove_item(items, box.item);
                    box.item = nullptr;
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
                p.sprite.setRotation(0);
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
        if (p.house_sprite.getGlobalBounds().intersects(
                    powerup->sprite.getGlobalBounds())) {
            return false;
        }
    }
    return true;
}

void spawn_powerup(std::vector<Powerup*>& powerups,
                   std::vector<Player>& players, PowerupTextures powerup_textures) {
    if (powerups.size() < MAX_NUM_POWERUPS) {
        PowerupType type = (PowerupType)(rand() % NUM_POWERUP_TYPES);
        Powerup* p = new Powerup{type, sf::Vector2f{0, 0}, powerup_textures};
        unsigned int rand_x;
        unsigned int rand_y;
        do {
            rand_x = (unsigned int)(rand() % WINDOW_WIDTH);
            rand_y = (unsigned int)(rand() % WINDOW_HEIGHT);
            p->sprite.setPosition(rand_x, rand_y);
        } while (!is_free_to_place(p, players));
        powerups.push_back(p);
    }
}

void handle_powerup_pickup(std::vector<Powerup*>& powerups, std::vector<Player>& players) {
    for (Player& p : players) {
        if (p.powerup == nullptr) {
            auto bb = p.shape.getGlobalBounds();
            for (Powerup* powerup : powerups) {

                if (!powerup->active &&
                        bb.intersects(powerup->sprite.getGlobalBounds())) {
                    p.powerup = powerup;
                    p.powerup->activate();
                    p.powerup->bar.set_position(p.sprite.getPosition()
                            + sf::Vector2f{-(float)PROGRESSBAR_WIDTH/2, PROGRESSBAR_DISTANCE});
                    break;
                }
            }
        }
    }
}

void update_powerups(std::vector<Powerup*>& powerups, std::vector<Player>& players) {
    for (Player& p : players) {
        if (p.powerup == nullptr) continue;
        p.powerup->update_progress();
        if (p.powerup->should_deactivate()) {
            remove_powerup(powerups, p.powerup);
            p.powerup = nullptr;
        }
    }
}

void update_powerup_animations(std::vector<Powerup*>& powerups, float dt) {
    for (Powerup* p : powerups) {
        if (!p->active) {
            p->update_animation(dt);
        }
    }
}

void handle_stealing_from_house(std::vector<Player>& players) {
    for (Player& p : players) {
        if (p.powerup == nullptr ||
                p.powerup->type != PowerupType::STEALING) continue;
        auto bb = p.shape.getGlobalBounds();
        for (Player& enemy : players) {
            for (Box& box: enemy.boxes) {
                if (box.item != nullptr &&
                    bb.intersects(box.item->sprite.getGlobalBounds())) {
                    box.filled = false;
                    Item* item = box.item;
                    box.item = nullptr;
                    p.carried_item = item;
                    item->sprite.setOrigin(10, 10);
                    item->being_carried = true;
                    item->in_box = false;
                    item->sprite.setPosition(p.sprite.getPosition());
                    return;
                }
            }
        }
    }
}


void game_startup(std::vector<Player>& players, sf::RenderWindow& window,
        sf::Font& font, sf::Sprite& background,
        std::vector<input::InputHandler*> handlers) {
    sf::Text welcome_text;
    welcome_text.setFont(font);
    welcome_text.setString("Welcome to ~!\nPress the up key on your controller or keyboard to join.\nPress Enter or START to start");
    welcome_text.setPosition(WINDOW_WIDTH/10, WINDOW_HEIGHT/3);
    unsigned int curr_player = 0;
    while (window.isOpen()) {
        // only add new players if there are less than 4 current players
        if (curr_player < 4) {
            for (auto handler : handlers) {
                // add player if this handler presses up
                if (handler->is_connected() &&
                        !handler->in_use &&
                        handler->get_value(input::Action::UP) > 0.5) {
                    std::cout << "Player " << curr_player << " connected" << std::endl;
                    Player& p = players[curr_player];
                    p.input_handler = handler;
                    handler->in_use = true;
                    p.connected = true;
                    curr_player++;
                }
            }
        }


        window.draw(background);
        window.draw(welcome_text);
        for (Player& p : players) {
            // only draw connected ones
            if (p.connected) {
                if (p.input_handler->get_value(input::Action::UP) > 0.5) {
                    p.house_sprite.setScale(1.05, 1.05);
                } else {
                    p.house_sprite.setScale(1, 1);
                }
                window.draw(p.house_sprite);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            // remove not used players and start game
            while (players.size() != curr_player) {
                players.erase(players.begin() + curr_player);
                std::cout << "removed player" << std::endl;
            }
            std::cout << players.size() << " joined" << std::endl;
            return;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.display();
    }
}

void initialize_input_handlers(std::vector<input::InputHandler*>& handlers) {
    for (KeyConfig& c : PLAYER_KEYS) {
        auto handler = new input::KeyboardInputHandler(
                c.up, c.down, c.left, c.right);
        handlers.push_back(handler);
    }

    // for some reason, controller 0 and 1 are always connected and press up
    for (int i = 2; i < 8; ++i) {
        auto handler = new input::ControllerInputHandler(i);
        handlers.push_back(handler);
    }
}

void destroy_handlers(std::vector<input::InputHandler*>& handlers) {
    for (auto handler: handlers) {
        delete handler;
    }
}

int main() {
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "~");

    sf::Font font;
    font.loadFromFile("../arial.ttf");

    // These need to stay alive for the entire game or players will be drawn
    // as white squares
    sf::Texture red_texture;
    sf::Texture green_texture;
    sf::Texture blue_texture;
    sf::Texture yellow_texture;
    red_texture.loadFromFile("../assets/red_player.png");
    green_texture.loadFromFile("../assets/green_player.png");
    blue_texture.loadFromFile("../assets/blue_player.png");
    yellow_texture.loadFromFile("../assets/yellow_player.png");

    sf::Texture red_house_texture;
    sf::Texture blue_house_texture;
    sf::Texture green_house_texture;
    sf::Texture yellow_house_texture;
    red_house_texture.loadFromFile("../assets/house_red.png");
    blue_house_texture.loadFromFile("../assets/house_blue.png");
    green_house_texture.loadFromFile("../assets/house_green.png");
    yellow_house_texture.loadFromFile("../assets/house_yellow.png");

    sf::Texture speed_texture;
    sf::Texture immunity_texture;
    sf::Texture fire_texture;
    sf::Texture stealing_texture;
    speed_texture.loadFromFile("../assets/speed.png");
    immunity_texture.loadFromFile("../assets/immunity.png");
    fire_texture.loadFromFile("../assets/fire.png");
    stealing_texture.loadFromFile("../assets/stealing.png");

    PowerupTextures powerup_textures;
    powerup_textures.speed = &speed_texture;
    powerup_textures.immunity = &immunity_texture;
    powerup_textures.fire = &fire_texture;
    powerup_textures.stealing = &stealing_texture;

    sf::Texture chair_texture;
    sf::Texture fridge_texture;
    sf::Texture tv_texture;
    sf::Texture plant_texture;
    chair_texture.loadFromFile("../assets/chair.png");
    fridge_texture.loadFromFile("../assets/fridge.png");
    tv_texture.loadFromFile("../assets/tv.png");
    plant_texture.loadFromFile("../assets/plant.png");

    std::vector<sf::Texture*> item_textures = {
        &chair_texture,
        &fridge_texture,
        &tv_texture,
        &plant_texture
    };

    sf::Texture burning_texture;
    burning_texture.loadFromFile("../assets/Fiyah.png");

    sf::Texture background_texture;
    background_texture.loadFromFile("../assets/background.png");
    background_texture.setRepeated(true);

    sf::Sprite background_sprite;
    background_sprite.setTextureRect(sf::IntRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT));
    background_sprite.setTexture(background_texture);

    float house_height = red_house_texture.getSize().y;
    float house_width = red_house_texture.getSize().x;
    std::vector<Player> players = {
        Player(0, sf::Vector2f{WINDOW_MARGIN, WINDOW_MARGIN}, &red_texture, &red_house_texture),
        Player(1, sf::Vector2f{WINDOW_MARGIN, WINDOW_HEIGHT - house_height - WINDOW_MARGIN}, &green_texture, &green_house_texture),
        Player(2, sf::Vector2f{WINDOW_WIDTH - house_width - WINDOW_MARGIN, WINDOW_MARGIN}, &blue_texture, &blue_house_texture),
        Player(3, sf::Vector2f{WINDOW_WIDTH - house_width - WINDOW_MARGIN,
                                WINDOW_HEIGHT - house_height - WINDOW_MARGIN}, &yellow_texture, &yellow_house_texture)
    };

    window.draw(background_sprite);
    std::vector<input::InputHandler*> handlers;
    initialize_input_handlers(handlers);
    game_startup(players, window, font, background_sprite, handlers);

    std::vector<Item*> items;
    std::vector<Powerup*> powerups;

    sf::Clock delta_clock;
    sf::Clock spawn_clock;
    sf::Clock powerup_clock;

    bool js_connnected = sf::Joystick::isConnected(0);
    unsigned int buttons = sf::Joystick::getButtonCount(0);
    std::cout << js_connnected << ":" << buttons << std::endl;

    bool game_finished = false;
    std::string win_text = "";
    while (window.isOpen()) {
        float dt = delta_clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (spawn_clock.getElapsedTime().asSeconds() > ITEM_SPAWN_INTERVAL) {
            spawn_item(items, item_textures);
            spawn_clock.restart();
        }

        if (powerup_clock.getElapsedTime().asSeconds() > POWERUP_SPAWN_INTERVAL) {
            spawn_powerup(powerups, players, powerup_textures);
            powerup_clock.restart();
        }

        handle_input(players, dt);

        handle_item_pickup(players, items);
        handle_powerup_pickup(powerups, players);
        update_powerups(powerups, players);
        update_powerup_animations(powerups, dt);

        handle_stealing_from_house(players);

        handle_item_stealing(players);
        handle_fire(players, items);

        handle_stun(players);
        

        for (Player& p : players) {
            if (p.carried_item != nullptr && p.is_home()) {
                //remove_item(items, p.carried_item);
                Item* item = p.carried_item;
                p.carried_item = nullptr;

                item->in_box = true;
                item->being_carried = false;

                // Fill a random box
                std::vector<size_t> available_indices;
                for (size_t i = 0; i < p.boxes.size(); i++) {
                    if (!p.boxes[i].filled) {
                        available_indices.push_back(i);
                    }
                }
                size_t empty_boxes = available_indices.size();
                if (empty_boxes > 0) {
                    Box* b = &p.boxes[available_indices[rand() % empty_boxes]];
                    item->sprite.setOrigin(0, 0);
                    item->sprite.setPosition(
                            b->shape.getPosition());
                    b->filled = true;
                    b->item = item;
                }
                if (empty_boxes == 1 && !game_finished) {
                    win_text = "player " + std::to_string(p.index) + " won the game!";
                    game_finished = true;
                }
            }
        }

        struct PowerupTextures {
            sf::Texture* speed;
            sf::Texture* immunity;
            sf::Texture* fire;
        };
        // Drawing
        window.clear(sf::Color::Black);
        window.draw(background_sprite);
        for (Player& player : players) {
            window.draw(player.house_sprite);

            for (auto box : player.boxes) {
                window.draw(box.shape);

                if (box.filled) {
                    window.draw(box.item->sprite);
                }

                if (box.on_fire) {
                    sf::Sprite fire_sprite;
                    fire_sprite.setTexture(burning_texture);
                    float time = box.fire_clock.getElapsedTime().asSeconds() * 8;
                    int frame = (int)time % 6;
                    fire_sprite.setTextureRect(sf::IntRect(frame * 10, 0, 10, 13));
                    fire_sprite.setPosition(box.shape.getPosition() + sf::Vector2f(5, 5));
                    window.draw(fire_sprite);
                }
            }
        }
        for (auto p : players) {
            float time = p.animation_clock.getElapsedTime().asSeconds() * 4;
            int frame = (int)time % 4;
            if (frame == 3) {
                frame = 1; // To get ping pong effect
            }
            if (!p.moving) {
                frame = 1;
            }
            p.sprite.setTextureRect(sf::IntRect(frame * 16, (int)p.direction * 18, 16, 18));
            window.draw(p.sprite);
        }
        for (auto item : items) {
            // we draw those in boxes earlier
            if (!item->in_box) {
                window.draw(item->sprite);
            }
        }
        for (auto powerup : powerups) {
            if (!powerup->active) {
                window.draw(powerup->sprite);
            } else {
                powerup->bar.draw(window);
            }
        }

        if (win_text != "") {
            sf::Text text;
            text.setFont(font);
            text.setString(win_text);
            text.setCharacterSize(60);
            text.setPosition(sf::Vector2f(100, WINDOW_HEIGHT / 2 - 30));
            window.draw(text);
        }

        window.display();
    }
    return 0;
}
