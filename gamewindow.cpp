#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "gamewindow.hpp"


GameWindow::~GameWindow() {}

void GameWindow::process_input()
{
    GLFWwindow *window = (GLFWwindow *)window_ptr;
    
    bool w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    
    float deltaTime = 0;
    float now = glfwGetTime();
    if (!w && !s && !a && !d && !shift && !space) {
        last_key_movement = -1;
    } else {
        if (last_key_movement < 0) last_key_movement = now;
        deltaTime = now - last_key_movement;
    }
    
    if (w) camera->moveForward(deltaTime);
    if (s) camera->moveBackward(deltaTime);
    if (a) camera->moveLeft(deltaTime);
    if (d) camera->moveRight(deltaTime);
    if (shift) camera->moveDown(deltaTime);
    if (space) camera->moveUp(deltaTime);
}

void GameWindow::key_callback(int key, int scancode, int action, int mods)
{
    GLFWwindow *window = (GLFWwindow *)window_ptr;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        grab_cursor = !grab_cursor;
        glfwSetInputMode(window, GLFW_CURSOR, grab_cursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void GameWindow::mouse_callback(double xpos, double ypos)
{
    GLFWwindow *window = (GLFWwindow *)window_ptr;
    
    float xoffset = xpos - last_mouse_x;
    float yoffset = last_mouse_y - ypos;
    
    last_mouse_x = xpos;
    last_mouse_y = ypos;
    
    if (!grab_cursor) return;
    camera->mouseMovement(xoffset, yoffset);
    
    // std::cout << "x:" << xpos << " y:" << ypos << std::endl;
}
