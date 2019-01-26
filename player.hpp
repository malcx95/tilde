#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "item.hpp"
#include "constants.hpp"
#include "powerup.hpp"

const unsigned int HOUSE_WIDTH = 200;
const unsigned int HOUSE_HEIGHT = 100;

const unsigned int PLAYER_RADIUS = 10;

const unsigned int NUM_BOXES = 10;

struct Box {
    Box(sf::Vector2f pos) : shape{sf::RectangleShape{sf::Vector2f(pos)}}, filled{false} {}
    sf::RectangleShape shape;
    bool filled;
};

enum Direction { Up, Right, Down, Left };

struct Player {
    Player(
        unsigned int index,
        sf::Color color,
        KeyConfig config,
        sf::Vector2f house_pos,
        sf::Texture* texture
    );

    KeyConfig key_config;
    unsigned int index;
    int score;
    bool stunned;
    Direction direction;

    sf::Sprite sprite;
    sf::RectangleShape house;
    std::vector<Box> boxes;
    Item* carried_item;

    Powerup* powerup;

    void set_position(sf::Vector2f position);
    void move(float dx, float dy);
    bool is_home() const;

    sf::Clock stun_clock;

};

#endif /* ifndef PLAYER_H */
