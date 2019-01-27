#ifndef CONSTANTS
#define CONSTANTS

#include <SFML/Graphics.hpp>
#include "config.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const unsigned int WINDOW_MARGIN = 40;
const float POWERUP_DESPAWN_TIMER = 20.0;
const float ITEM_DROP_TIMER = 1.0;

struct KeyConfig {
    sf::Keyboard::Key up;
    sf::Keyboard::Key down;
    sf::Keyboard::Key left;
    sf::Keyboard::Key right;
    sf::Keyboard::Key drop;
};

struct PowerupTextures {
    sf::Texture* speed;
    sf::Texture* immunity;
    sf::Texture* fire;
    sf::Texture* stealing;
};

#endif /* ifndef CONSTANTS */

