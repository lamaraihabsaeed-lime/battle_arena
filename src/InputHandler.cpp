#include "InputHandler.h"

InputHandler::InputHandler() {
}

void InputHandler::keyDown(int k) {
    keys.insert(k);
    justPressed.insert(k);
}

void InputHandler::keyUp(int k) {
    keys.erase(k);
    justPressed.erase(k);
}

bool InputHandler::isPressed(int k) const {
    return keys.count(k) > 0;
}

bool InputHandler::wasJustPressed(int k) const {
    return justPressed.count(k) > 0;
}

void InputHandler::update() {
    justPressed.clear();
}

void InputHandler::reset() {
    keys.clear();
    justPressed.clear();
}
