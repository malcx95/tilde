#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const unsigned int MARGIN = 40;

const unsigned int HOUSE_WIDTH = 200;
const unsigned int HOUSE_HEIGHT = 100;

struct KeyConfig {
    sf::Keyboard::Key up;
    sf::Keyboard::Key down;
    sf::Keyboard::Key left;
    sf::Keyboard::Key right;
};

// SFML doesn't support scancodes...
#define COLEMAK
#ifdef COLEMAK
KeyConfig PLAYER_KEYS[] = {
    KeyConfig { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right },
    KeyConfig { sf::Keyboard::W, sf::Keyboard::R, sf::Keyboard::A, sf::Keyboard::S },
    KeyConfig { sf::Keyboard::G, sf::Keyboard::D, sf::Keyboard::T, sf::Keyboard::H },
    KeyConfig { sf::Keyboard::U, sf::Keyboard::E, sf::Keyboard::N, sf::Keyboard::I }
};
#else
#endif

struct Item {
    sf::CircleShape shape;
    int carriedBy; // -1 if not carried
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "~");

    sf::RectangleShape house(sf::Vector2f(HOUSE_WIDTH, HOUSE_HEIGHT));
    house.setFillColor(sf::Color(100, 100, 100));
    sf::RectangleShape houses[] = { house, house, house, house };
    houses[0].setPosition(MARGIN, MARGIN);
    houses[1].setPosition(MARGIN, HEIGHT - HOUSE_HEIGHT - MARGIN);
    houses[2].setPosition(WIDTH - HOUSE_WIDTH - MARGIN, MARGIN);
    houses[3].setPosition(
        WIDTH - HOUSE_WIDTH - MARGIN,
        HEIGHT - HOUSE_HEIGHT - MARGIN
    );

    sf::CircleShape playerShape(10);
    playerShape.setOrigin(10, 10);
    sf::CircleShape playerShapes[] = {
        playerShape,
        playerShape,
        playerShape,
        playerShape
    };
    playerShapes[0].setFillColor(sf::Color(255, 100, 100));
    playerShapes[1].setFillColor(sf::Color(100, 255, 100));
    playerShapes[2].setFillColor(sf::Color(100, 100, 255));
    playerShapes[3].setFillColor(sf::Color(255, 255, 100));
    for (int i = 0; i < 4; i++) {
        auto pos = houses[i].getPosition() +
            sf::Vector2f(HOUSE_WIDTH / 2, HOUSE_HEIGHT / 2);
        playerShapes[i].setPosition(pos);
    }

    float spawnInterval = 1.0f;
    std::vector<Item> items{};

    sf::Clock deltaClock;
    sf::Clock spawnClock;
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
            sf::CircleShape shape(10, 3);
            shape.setOrigin(10, 10);
            shape.setPosition(WIDTH / 2, HEIGHT / 2);
            shape.setFillColor(sf::Color(200, 255, 255));
            items.push_back(Item { shape, -1 });
            spawnClock.restart();
        }

        const float PLAYER_SPEED = 80.f;
        for (int i = 0; i < 4; i++) {
            if (sf::Keyboard::isKeyPressed(PLAYER_KEYS[i].up)) {
                playerShapes[i].move(0.f, -PLAYER_SPEED * dt);
            }
            if (sf::Keyboard::isKeyPressed(PLAYER_KEYS[i].down)) {
                playerShapes[i].move(0.f, PLAYER_SPEED * dt);
            }
            if (sf::Keyboard::isKeyPressed(PLAYER_KEYS[i].left)) {
                playerShapes[i].move(-PLAYER_SPEED * dt, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(PLAYER_KEYS[i].right)) {
                playerShapes[i].move(PLAYER_SPEED * dt, 0.f);
            }

            auto boundingBox = playerShapes[i].getGlobalBounds();
            for (auto& item : items) {
                // TODO: add stun when stealing
                if (boundingBox.intersects(item.shape.getGlobalBounds())) {
                    item.carriedBy = i;
                }
            }
        }

        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].carriedBy > 0) {
                items[i].shape.setPosition(playerShapes[items[i].carriedBy].getPosition());
            }

            for (auto& house : houses) {
                auto boundingBox = house.getGlobalBounds();
                if (boundingBox.intersects(items[i].shape.getGlobalBounds())) {
                    // TODO: Add to player score
                }
            }
        }

        // Drawing
        window.clear(sf::Color::Black);
        for (auto h : houses) {
            window.draw(h);
        }
        for (auto p : playerShapes) {
            window.draw(p);
        }
        for (auto item : items) {
            window.draw(item.shape);
        }

        window.display();
    }
    return 0;
}
