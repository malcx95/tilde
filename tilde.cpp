#include <SFML/Graphics.hpp>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const unsigned int MARGIN = 40;

const unsigned int HOUSE_WIDTH = 200;
const unsigned int HOUSE_HEIGHT = 100;

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "~");

    sf::RectangleShape house(sf::Vector2f(HOUSE_WIDTH, HOUSE_HEIGHT));
    sf::RectangleShape houses[] = { house, house, house, house };
    houses[0].setPosition(MARGIN, MARGIN);
    houses[1].setPosition(MARGIN, HEIGHT - HOUSE_HEIGHT - MARGIN);
    houses[2].setPosition(WIDTH - HOUSE_WIDTH - MARGIN, MARGIN);
    houses[3].setPosition(
        WIDTH - HOUSE_WIDTH - MARGIN,
        HEIGHT - HOUSE_HEIGHT - MARGIN
    );

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        for (auto h : houses) {
            window.draw(h);
        }

        window.display();
    }
    return 0;
}
