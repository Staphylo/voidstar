#include <bitset>


struct GlfwModifiers {
    int shift:1;
    int ctrl:1;
    int alt:1;
    int super:1;
} __attribute__((packed));

class GlfwEvents : public Events {
    friend GlfwManager;
public:
    virtual bool keyPressed(int key);
    virtual bool keyReleased(int key);
    virtual bool keyUp(int key) {return false;};
    virtual bool keyDown(int key) {return false;};

public:
    std::bitset<512> keys;
    std::bitset<512> state;
    union {
        int rawmods;
        GlfwModifiers mods;
    };
};

bool
GlfwEvents::keyPressed(int key) {
    auto value = glfwGetKey(GlfwManager::instance()->window(), key);
    if (value == GLFW_KEY_UNKNOWN)
        throw std::runtime_error("Unknown key pressed");
    return value == GLFW_PRESS;
}

bool
GlfwEvents::keyReleased(int key) {
    auto value = glfwGetKey(GlfwManager::instance()->window(), key);
    if (value == GLFW_KEY_UNKNOWN)
        throw std::runtime_error("Unknown key released");
    return value == GLFW_RELEASE;
}

