#pragma once

#include <SDL2/SDL.h>

#include <unordered_map>

namespace Forge {
enum class Key {
    UNKNOWN,
    W,
    A,
    S,
    D,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    // Actions
    ENTER,
    SPACE,
    ESCAPE,
    LeftShift,
    LeftCtrl,
    RightShift,
    RightCtrl,
    // Numbers for potential future use in text input handling
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9
};

enum class MouseButton { LEFT, RIGHT, MIDDLE };

class Input {
   public:
    // Call one per frame for polling events
    void beginFrame();

    // Call with each SDL Event from the event loop
    void processEvent(const SDL_Event& event);

    // Key Query
    bool isKeyPressed(Key key) const;   // true for one frame when the key is first pressed
    bool isKeyHeldDown(Key key) const;      // true for every frame the key is held down
    bool isKeyReleased(Key key) const;  // true for one frame when the key is released

    // Mouse Button Query
    bool isMouseButtonPressed(MouseButton button) const;
    bool isMouseButtonHeldDown(MouseButton button) const;
    bool isMouseButtonReleased(MouseButton button) const;

    int getMouseX() const { return mouseX; }
    int getMouseY() const { return mouseY; }

   private:
    std::unordered_map<Key, bool> currentKeys;
    std::unordered_map<Key, bool> previousKeys;
    std::unordered_map<MouseButton, bool> currentMouseButtons;
    std::unordered_map<MouseButton, bool> previousMouseButtons;

    int mouseX = 0;
    int mouseY = 0;

    static Key sdlKeyToForgeKey(SDL_Keycode sdlKey);
};
}  // namespace Forge