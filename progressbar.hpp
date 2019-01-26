#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <SFML/Graphics.hpp>

class Progressbar {

public:

    Progressbar(unsigned int width, unsigned int height);

    void set_colors(sf::Color foreground, sf::Color background);
    void set_progress(float progress);
    void set_position(sf::Vector2f pos);
    void move(float dx, float dy);
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window);

private:

    sf::RectangleShape background;
    sf::RectangleShape foreground;

};

#endif /* ifndef PROGRESSBAR_H */
