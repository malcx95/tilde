#include "progressbar.hpp"

Progressbar::Progressbar(unsigned int width, unsigned int height) 
    : background{sf::Vector2f{(float)width, (float)height}},
      foreground{sf::Vector2f{(float)width, (float)height}} { }

void Progressbar::set_colors(sf::Color foreground, sf::Color background) {
    this->foreground.setFillColor(foreground);
    this->background.setFillColor(background);
}

void Progressbar::set_progress(float progress) {
    this->foreground.setScale(1. - progress, 1.);
}

void Progressbar::set_position(sf::Vector2f pos) {
    this->foreground.setPosition(pos);
    this->background.setPosition(pos);
}

void Progressbar::move(float dx, float dy) {
    this->foreground.move(dx, dy);
    this->background.move(dx, dy);
}

sf::Vector2f Progressbar::getPosition() const {
    return this->background.getPosition();
}

void Progressbar::draw(sf::RenderWindow& window) {
    window.draw(this->background);
    window.draw(this->foreground);
}

