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
KeyConfig PLAYER_KEYS[] = {
    KeyConfig { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right },
    KeyConfig { sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D },
    KeyConfig { sf::Keyboard::I, sf::Keyboard::K, sf::Keyboard::J, sf::Keyboard::L },
    KeyConfig { sf::Keyboard::T, sf::Keyboard::G, sf::Keyboard::F, sf::Keyboard::H }
};
#endif

const int MAX_ITEMS = 10;

struct Item {
    bool alive;
    sf::CircleShape shape;
    int carriedBy; // -1 if not carried
};

void spawn_item(std::vector<Item>& items) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].alive) {
            sf::CircleShape shape(10, 3);
            shape.setOrigin(10, 10);
            shape.setPosition(WIDTH / 2, HEIGHT / 2);
            shape.setFillColor(sf::Color(200, 255, 255));

            items[i].alive = true;
            items[i].carriedBy = -1;
            items[i].shape = shape;

            break;
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "~");

    sf::Font font;
    font.loadFromFile("arial.ttf");

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
    std::vector<Item> items(MAX_ITEMS);
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].alive = false;
        items[i].carriedBy = -1;
    }

    int player_scores[] = { 0, 0, 0, 0 };

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
            spawn_item(items);
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

            bool already_carries = false;
            for (int j = 0; j < MAX_ITEMS; j++) {
                if (items[j].alive && items[j].carriedBy == i) {
                    already_carries = true;
                    break;
                }
            }

            if (!already_carries) {
                auto boundingBox = playerShapes[i].getGlobalBounds();
                for (int j = 0; j < MAX_ITEMS; j++) {
                    // TODO: add stun when stealing
                    if (items[j].alive && boundingBox.intersects(items[j].shape.getGlobalBounds())) {
                        items[j].carriedBy = i;
                        break;
                    }
                }
            }
        }

        for (size_t i = 0; i < items.size(); i++) {
            int carrier = items[i].carriedBy;
            if (items[i].alive && carrier >= 0) {
                items[i].shape.setPosition(playerShapes[carrier].getPosition());

                auto boundingBox = houses[carrier].getGlobalBounds();
                if (boundingBox.intersects(items[i].shape.getGlobalBounds())) {
                    player_scores[carrier] += 10;
                    items[i].alive = false;

                    if (player_scores[carrier] >= 100) {
                        window.close();

                        // TODO: display player color instead and keep window open
                        std::cout << "player " << carrier << " won the game" << std::endl;
                    }
                }
            }
        }

        // Drawing
        window.clear(sf::Color::Black);
        for (int i = 0; i < 4; i++) {
            window.draw(houses[i]);

            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(player_scores[i]));
            text.setCharacterSize(50);
            text.setFillColor(sf::Color(255, 255, 255));
            text.setPosition(houses[i].getPosition());
            window.draw(text);
        }
        for (auto p : playerShapes) {
            window.draw(p);
        }
        for (auto item : items) {
            if (item.alive) {
                window.draw(item.shape);
            }
        }

        window.display();
    }
    return 0;
}
