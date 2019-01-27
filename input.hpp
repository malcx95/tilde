#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>
#include <map>

namespace input {

    enum class Action { NONE, UP, DOWN, LEFT, RIGHT, DROP };

    class InputHandler {
    public:
        virtual float get_value(const Action ac) = 0;
        virtual bool is_connected() const = 0;
        virtual ~InputHandler() = default;

        bool in_use = false;
    };


    class KeyboardInputHandler : public InputHandler {
    public:
        KeyboardInputHandler(
                sf::Keyboard::Key up,
                sf::Keyboard::Key down,
                sf::Keyboard::Key left,
                sf::Keyboard::Key right,
                sf::Keyboard::Key drop
                );

        virtual float get_value(const Action ac);

        virtual bool is_connected() const;

    private:

        std::map<Action, sf::Keyboard::Key> action_map;
    };




    class ControllerInputHandler : public InputHandler {
    public:
        ControllerInputHandler(int id);

        virtual float get_value(const Action ac);
        virtual bool is_connected() const;
    private:
        int id;
    };
}

#endif /* ifndef INPUT_H */
