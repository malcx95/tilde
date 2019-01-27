#include "input.hpp"

using namespace input;

KeyboardInputHandler::KeyboardInputHandler(
                sf::Keyboard::Key up,
                sf::Keyboard::Key down,
                sf::Keyboard::Key left,
                sf::Keyboard::Key right) {
    this->action_map = {
        {Action::UP, up},
        {Action::DOWN, down},
        {Action::LEFT, left},
        {Action::RIGHT, right}
    };
}

float KeyboardInputHandler::get_value(const Action ac) {
    return sf::Keyboard::isKeyPressed(this->action_map[ac]);
}

bool KeyboardInputHandler::is_connected() const {
    return true;
}

ControllerInputHandler::ControllerInputHandler(int id) :
    id{id}
{}

float ControllerInputHandler::get_value(const Action ac) {
    auto axis_x = sf::Joystick::getAxisPosition(id, sf::Joystick::X) / 100;
    auto axis_y = sf::Joystick::getAxisPosition(id, sf::Joystick::Y) / 100;

    switch (ac) {
        case Action::UP:
            if (axis_y < 0) {
                return -axis_y;
            }
            break;
        case Action::DOWN:
            if (axis_y > 0) {
                return axis_y;
            }
            break;
        case Action::LEFT:
            if (axis_x < 0) {
                return -axis_x;
            }
            break;
        case Action::RIGHT:
            if (axis_x > 0) {
                return axis_x;
            }
            break;
        case Action::NONE:
            return 0;
            break;
    }
    return 0;
}

bool ControllerInputHandler::is_connected() const {
    return sf::Joystick::isConnected(this->id);
}
