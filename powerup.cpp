#include "powerup.hpp"
#include <math.h>
#include "constants.hpp"
#include "player.hpp"
#include <iostream>

Powerup::Powerup(PowerupType type, sf::Vector2f position, PowerupTextures powerup_textures)
    : type{type}, bar{PROGRESSBAR_WIDTH, PROGRESSBAR_HEIGHT} {
    this->sprite.setPosition(position);
    this->active = false;
    this->animation_counter = 0;
    this->sprite.setOrigin(POWERUP_WIDTH, POWERUP_WIDTH);
    sf::Color c;
    switch (type) {
        case PowerupType::FASTER:
            this->sprite.setTexture(*powerup_textures.speed);
            c = sf::Color{255, 50, 50};
            break;
        case PowerupType::IMMUNITY:
            this->sprite.setTexture(*powerup_textures.immunity);
            c = sf::Color{255, 0, 255};
            break;
        case PowerupType::FIRE:
            this->sprite.setTexture(*powerup_textures.fire);
            c = sf::Color{255, 200, 50};
            break;
        case PowerupType::STEALING:
            this->sprite.setTexture(*powerup_textures.stealing);
            c = sf::Color{40, 240, 180};
            break;
    }
    this->bar.set_colors(c, sf::Color{50, 50, 50});
}

void Powerup::activate() {
    this->clock.restart();
    this->active = true;
}

void Powerup::update_animation(float dt) {
    this->animation_counter++;
    double dy = ANIMATION_AMPLITUDE*sin((double)this->animation_counter*ANIMATION_SPEED);
    auto curr_x = this->sprite.getPosition().x;
    auto curr_y = this->sprite.getPosition().y;
    this->sprite.setPosition(curr_x, curr_y + dy*dt);
}

void Powerup::update_progress() {
    this->bar.set_progress(this->clock.getElapsedTime().asSeconds() / POWERUP_TIME);
}


bool Powerup::should_deactivate() const {
    return this->active && this->clock.getElapsedTime().asSeconds() >= POWERUP_TIME;
}

