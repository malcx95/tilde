#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include "progressbar.hpp"
#include "constants.hpp"

const unsigned int NUM_POWERUP_TYPES = 4;
enum PowerupType { IMMUNITY, FASTER, FIRE, STEALING };

const float POWERUP_TIME = 5.0;
const unsigned int POWERUP_WIDTH = 6;
const unsigned int MAX_NUM_POWERUPS = 1000;

const unsigned int PROGRESSBAR_WIDTH = 30;
const unsigned int PROGRESSBAR_HEIGHT = 5;

const float SPEED_INCREASE = 2.;

const double ANIMATION_SPEED = .01;
const double ANIMATION_AMPLITUDE = 5.;

struct Powerup {

    Powerup(PowerupType type, sf::Vector2f position, PowerupTextures powerup_textures);

    PowerupType type;

    sf::Clock clock;

    bool active;

    Progressbar bar;

    bool should_deactivate() const;

    void activate();

    void update_progress();

    void update_animation(float dt);

    unsigned long animation_counter;

    sf::Sprite sprite;

};

#endif /* ifndef POWERUP_H */
