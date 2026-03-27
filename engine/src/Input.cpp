#include "Input.h"

namespace Forge {
void Input::beginFrame() {
    // snapshot of the current state
    previousKeys = currentKeys;
    previousMouseButtons = currentMouseButtons;
}

void Input::processEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        Key key = sdlKeyToForgeKey(event.key.keysym.sym);
        currentKeys[key] = true;
    } else if (event.type == SDL_KEYUP) {
        Key key = sdlKeyToForgeKey(event.key.keysym.sym);
        currentKeys[key] = false;
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            currentMouseButtons[MouseButton::LEFT] = true;
        } else if (event.button.button == SDL_BUTTON_MIDDLE) {
            currentMouseButtons[MouseButton::MIDDLE] = true;
        } else if (event.button.button == SDL_BUTTON_RIGHT) {
            currentMouseButtons[MouseButton::RIGHT] = true;
        }

    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            currentMouseButtons[MouseButton::LEFT] = false;
        } else if (event.button.button == SDL_BUTTON_MIDDLE) {
            currentMouseButtons[MouseButton::MIDDLE] = false;
        } else if (event.button.button == SDL_BUTTON_RIGHT) {
            currentMouseButtons[MouseButton::RIGHT] = false;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        mouseX = event.motion.x;
        mouseY = event.motion.y;
    }
}

bool Input::isKeyPressed(Key key) const {
    // True only on the frame the key is first pressed
    auto currentState = currentKeys.find(key);
    auto previousState = previousKeys.find(key);
    bool currentDown = currentState != currentKeys.end() && currentState->second;
    bool previousDown = previousState != previousKeys.end() && previousState->second;
    return currentDown && !previousDown;
}

bool Input::isKeyHeldDown(Key key) const {
    // True for every frame the key is held down
    auto currentState = currentKeys.find(key);
    return currentState != currentKeys.end() && currentState->second;
}

bool Input::isKeyReleased(Key key) const {
    // True only on the frame the key is released
    auto currentState = currentKeys.find(key);
    auto previousState = previousKeys.find(key);
    bool currentDown = currentState != currentKeys.end() && currentState->second;
    bool previousDown = previousState != previousKeys.end() && previousState->second;
    return !currentDown && previousDown;
}

bool Input::isMouseButtonPressed(MouseButton button) const {
    auto currentState = currentMouseButtons.find(button);
    auto previousState = previousMouseButtons.find(button);
    bool currentDown = currentState != currentMouseButtons.end() && currentState->second;
    bool previousDown = previousState != previousMouseButtons.end() && previousState->second;
    return currentDown && !previousDown;
}

bool Input::isMouseButtonHeldDown(MouseButton button) const {
    auto currentState = currentMouseButtons.find(button);
    return currentState != currentMouseButtons.end() && currentState->second;
}

bool Input::isMouseButtonReleased(MouseButton button) const {
    auto currentState = currentMouseButtons.find(button);
    auto previousState = previousMouseButtons.find(button);
    bool currentDown = currentState != currentMouseButtons.end() && currentState->second;
    bool previousDown = previousState != previousMouseButtons.end() && previousState->second;
    return !currentDown && previousDown;
}
Key Input::sdlKeyToForgeKey(SDL_Keycode sdlKey) {
    static std::unordered_map<SDL_Keycode, Key> keyMap = {{SDLK_w, Key::W},
                                                          {SDLK_a, Key::A},
                                                          {SDLK_s, Key::S},
                                                          {SDLK_d, Key::D},
                                                          {SDLK_UP, Key::UP},
                                                          {SDLK_DOWN, Key::DOWN},
                                                          {SDLK_LEFT, Key::LEFT},
                                                          {SDLK_RIGHT, Key::RIGHT},
                                                          {SDLK_RETURN, Key::ENTER},
                                                          {SDLK_SPACE, Key::SPACE},
                                                          {SDLK_ESCAPE, Key::ESCAPE},
                                                          {SDLK_LSHIFT, Key::LeftShift},
                                                          {SDLK_RSHIFT, Key::RightShift},
                                                          {SDLK_LCTRL, Key::LeftCtrl},
                                                          {SDLK_RCTRL, Key::RightCtrl},
                                                          {SDLK_0, Key::Num0},
                                                          {SDLK_1, Key::Num1},
                                                          {SDLK_2, Key::Num2},
                                                          {SDLK_3, Key::Num3},
                                                          {SDLK_4, Key::Num4},
                                                          {SDLK_5, Key::Num5},
                                                          {SDLK_6, Key::Num6},
                                                          {SDLK_7, Key::Num7},
                                                          {SDLK_8, Key::Num8},
                                                          {SDLK_9, Key::Num9}};

    auto it = keyMap.find(sdlKey);
    if (it != keyMap.end()) {
        return it->second;
    }
    return Key::UNKNOWN;
}
}  // namespace Forge