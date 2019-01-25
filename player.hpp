#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <SFML/Graphics.hpp>

const unsigned int HOUSE_WIDTH = 200;
const unsigned int HOUSE_HEIGHT = 100;

const unsigned int PLAYER_RADIUS = 10;

struct Player {

    Player(unsigned int index, 
            sf::Color color,
            sf::Vector2f house_pos);

    unsigned int index;

    int score;

    bool stunned;

    sf::CircleShape shape;

    sf::RectangleShape house;

};

#endif /* ifndef PLAYER_H */

