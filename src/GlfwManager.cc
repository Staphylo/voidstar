#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GlfwManager.hh>

#define get_manager_ptr(Window) \
    reinterpret_cast<GlfwManager*>(glfwGetWindowUserPointer(Window))

// records how far the y axis has been scrolled
static void
OnScroll(GLFWwindow* window, double deltaX, double deltaY) {
    auto* mouse = get_manager_ptr(window)->getMouse();
    mouse->scrollY += deltaY;
    mouse->scrollX += deltaX;
}

static void
OnError(int errorCode __unused, const char* msg) {
    throw std::runtime_error(msg);
}

void
GlfwManager::init()
{
    glfwSetErrorCallback(OnError);
    if (!glfwInit())
        throw std::runtime_error("!glfwInit");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWmonitor* monitor = NULL;
    if (args_->fullscreen) {
        monitor = glfwGetPrimaryMonitor();
    }

    window_ = glfwCreateWindow(args_->width, args_->height, "points", monitor, NULL);
    if (!window_)
        throw std::runtime_error("!glfwCreateWindow. Can your hardware handle OpenGL 3.2?");

    glfwSetWindowUserPointer(window_, this);

    // GLFW settings
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(window_, 0, 0);
    glfwSetScrollCallback(window_, OnScroll);
    glfwMakeContextCurrent(window_);

    mouse_ = new GlfwMouse();
    events_ = new GlfwEvents();

    glInit();
}

void
GlfwManager::glInit()
{
    glewExperimental = GL_TRUE; //stops glew from crashing on OSX :-/
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("!glewInit");

    // GLEW throws some errors, so discard all the errors so far
    //while (glGetError() != GL_NO_ERROR) {}
    glProcessErrors();

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    if (!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
}

void
GlfwManager::glProcessErrors(bool quiet)
{
    while (true) {
        GLenum error = glGetError();
        if (error == GL_NO_ERROR)
            break;
        if (!quiet)
            std::cerr << "OpenGL Error " << error << std::endl;
    }
}

void
GlfwManager::run()
{
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window_)) {
        // process pending events
        glfwPollEvents();

        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        float elapsedTime = thisTime - lastTime;
        bool redraw = scene_->update(elapsedTime);
        lastTime = thisTime;

        if (redraw) {
            scene_->render();
            glfwSwapBuffers(window_);
        }

        glProcessErrors();

        if (events_->keyPressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window_, GL_TRUE);
        if (events_->keyPressed('F'))
            toggleFullscreen();

        if (events_->keyPressed('L'))
            loadNextFile();
        else if (events_->keyPressed('H'))
            loadPrevFile();
        if (events_->keyPressed('J'))
            loadPrevAlgo();
        else if (events->keyPressed('K'))
            loadNextAlgo();
    }
    glfwTerminate();
}

bool
GlfwEvents::keyPressed(int key) {
    auto value = glfwGetKey(GlfwManager::instance()->window(), key);
    if (value == GLFW_KEY_UNKNOWN)
        throw std::runtime_error("Unknown key");
    return value == GLFW_PRESS;
}

void
GlfwMouse::getCursorPos() {
    glfwGetCursorPos(GlfwManager::instance()->window(), &x, &y);
}

void
GlfwMouse::setCursorPos() {
    glfwSetCursorPos(GlfwManager::instance()->window(), x, y);
}
