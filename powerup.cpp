#include "powerup.hpp"
#include <math.h>
#include "constants.hpp"
#include "player.hpp"

Powerup::Powerup(PowerupType type, sf::Vector2f position)
    : type{type}, shape{sf::CircleShape{POWERUP_WIDTH, 8}} {
    this->shape.setPosition(position);
    this->active = false;
    switch (type) {
        case PowerupType::FASTER:
            this->shape.setFillColor(sf::Color{255, 50, 50});
            break;
        case PowerupType::IMMUNITY:
            this->shape.setFillColor(sf::Color{255, 0, 255});
            break;
    }
}

void Powerup::activate() {
    this->clock.restart();
    this->active = true;
}

bool Powerup::should_deactivate() const {
    return this->active && this->clock.getElapsedTime().asSeconds() >= POWERUP_TIME;
}

