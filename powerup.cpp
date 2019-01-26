#include "powerup.hpp"
#include <math.h>
#include "constants.hpp"
#include "player.hpp"
#include <iostream>

Powerup::Powerup(PowerupType type, sf::Vector2f position)
    : type{type}, shape{sf::CircleShape{POWERUP_WIDTH, 8}}, 
      bar{PROGRESSBAR_WIDTH, PROGRESSBAR_HEIGHT} {
    this->shape.setPosition(position);
    this->active = false;
    this->shape.setOrigin(POWERUP_WIDTH, POWERUP_WIDTH);
    this->animation_counter = 0;
    sf::Color c;
    switch (type) {
        case PowerupType::FASTER:
            c = sf::Color{255, 50, 50};
            break;
        case PowerupType::IMMUNITY:
            c = sf::Color{255, 0, 255};
            break;
    }
    this->shape.setFillColor(c);
    this->bar.set_colors(c, sf::Color{50, 50, 50});
}

void Powerup::activate() {
    this->clock.restart();
    this->active = true;
}

void Powerup::update_animation() {
    this->animation_counter++;
    double dy = ANIMATION_AMPLITUDE*sin((double)this->animation_counter*ANIMATION_SPEED);
    auto curr_x = this->shape.getPosition().x;
    auto curr_y = this->shape.getPosition().y;
    this->shape.setPosition(curr_x, curr_y + dy);
}

void Powerup::update_progress() {
    this->bar.set_progress(this->clock.getElapsedTime().asSeconds() / POWERUP_TIME);
}


bool Powerup::should_deactivate() const {
    return this->active && this->clock.getElapsedTime().asSeconds() >= POWERUP_TIME;
}

