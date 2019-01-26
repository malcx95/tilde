#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>

const unsigned int NUM_POWERUP_TYPES = 2;
enum PowerupType { IMMUNITY, FASTER };

const float POWERUP_TIME = 5.0;
const unsigned int POWERUP_WIDTH = 6;
const unsigned int MAX_NUM_POWERUPS = 10;

struct Powerup {

    Powerup(PowerupType type, sf::Vector2f position);

    PowerupType type;

    sf::CircleShape shape;

    sf::Clock clock;

    bool active;

    bool should_deactivate() const;

    void activate();

};

#endif /* ifndef POWERUP_H */
