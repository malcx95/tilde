#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "item.hpp"
#include "constants.hpp"

const unsigned int HOUSE_WIDTH = 200;
const unsigned int HOUSE_HEIGHT = 100;

const unsigned int PLAYER_RADIUS = 10;

struct Player {

    Player(unsigned int index,
            sf::Color color,
            KeyConfig config,
            sf::Vector2f house_pos);

    KeyConfig key_config;

    unsigned int index;

    int score;

    bool stunned;

    sf::CircleShape shape;

    sf::RectangleShape house;

    Item* carried_item;

    void set_position(sf::Vector2f position);
    void move(float dx, float dy);
    bool is_home() const;

};

#endif /* ifndef PLAYER_H */
