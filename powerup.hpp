#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include "progressbar.hpp"

const unsigned int NUM_POWERUP_TYPES = 3;
enum PowerupType { IMMUNITY, FASTER, FIRE };

const float POWERUP_TIME = 5.0;
const unsigned int POWERUP_WIDTH = 6;
const unsigned int MAX_NUM_POWERUPS = 10;

const unsigned int PROGRESSBAR_WIDTH = 30;
const unsigned int PROGRESSBAR_HEIGHT = 5;

const float SPEED_INCREASE = 2.;

const double ANIMATION_SPEED = 0.01;
const double ANIMATION_AMPLITUDE = .04;

struct Powerup {

    Powerup(PowerupType type, sf::Vector2f position);

    PowerupType type;

    sf::CircleShape shape;

    sf::Clock clock;

    bool active;

    Progressbar bar;

    bool should_deactivate() const;

    void activate();

    void update_progress();

    void update_animation();

    unsigned long animation_counter;

};

#endif /* ifndef POWERUP_H */
