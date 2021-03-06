#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "item.hpp"
#include "constants.hpp"
#include "powerup.hpp"
#include "input.hpp"

const unsigned int PLAYER_RADIUS = 8;

const unsigned int NUM_BOXES = 10;

const unsigned int PROGRESSBAR_DISTANCE = 10;
const unsigned int ITEM_SPACING = 40;

struct Box {
    Box(sf::Vector2f pos) : shape{sf::RectangleShape{sf::Vector2f(pos)}}, filled{false}, on_fire{false}, item{nullptr} {}
    sf::RectangleShape shape;
    sf::Clock fire_clock;
    bool filled;
    bool on_fire;
    Item* item;
};

enum Direction { Up, Right, Down, Left };

struct Player {
    Player(
        unsigned int index,
        sf::Vector2f house_pos,
        sf::Texture* texture,
        sf::Texture* house_texture
    );

    input::InputHandler* input_handler;
    bool connected;

    unsigned int index;
    bool stunned;
    bool moving;
    Direction direction;

    sf::Sprite sprite;
    sf::CircleShape shape;
    sf::Sprite house_sprite;
    std::vector<Box> boxes;
    Item* carried_item;

    Powerup* powerup;

    void set_position(sf::Vector2f position);
    void move(float dx, float dy);
    bool is_home() const;

    sf::Clock stun_clock;
    sf::Clock animation_clock;
};

#endif /* ifndef PLAYER_H */
