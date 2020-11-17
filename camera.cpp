

#include "camera.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

// static constexpr glm::vec3 world_up(0.0f, 1.0f, 0.0f);

Camera::Camera(const glm::vec3& pos_in, float yaw_in, float pitch_in)
{
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    position = pos_in;
    yaw = yaw_in;
    pitch = pitch_in;
    update();
}

Camera::Camera(float posX, float posY, float posZ, float yaw_in, float pitch_in)
{
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    position = glm::vec3(posX, posY, posZ);
    yaw = yaw_in;
    pitch = pitch_in;
    update();
}

void Camera::moveForward(float distance)
{
    position += distance * front;
    update();
}

void Camera::moveBackward(float distance)
{
    position -= distance * front;
    update();
}
    
void Camera::moveLeft(float distance)
{
    position -= distance * right;
    update();
}
    
void Camera::moveRight(float distance)
{
    position += distance * right;
    update();
}

void Camera::moveUp(float distance)
{
    // std::cout << "distance:" << distance << " up:" << glm::to_string(up) << std::endl;
    position += distance * up;
    update();
}

void Camera::moveDown(float distance)
{
    position -= distance * up;
    update();
}

void Camera::rotate(float pitch_change, float yaw_change)
{
    yaw += yaw_change;
    pitch += pitch_change;
    
    if (pitch > 89) pitch = 89;
    if (pitch < -89) pitch = -89;
    
    update();
}

void Camera::update()
{
    glm::vec3 front_tmp;
    front_tmp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_tmp.y = sin(glm::radians(pitch));
    front_tmp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front_tmp);
    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
    
    view_matrix = glm::lookAt(position, position + front, up);
    
    // std::cout << glm::to_string(view_matrix) << std::endl;
}


// Good:
// mat4x4((1.000000, -0.000000, 0.000000, 0.000000), (0.000000, 0.707107, 0.707107, 0.000000), (-0.000000, -0.707107, 0.707107, 0.000000), (0.000000, -0.000000, -4.242641, 1.000000))
// Bad:
// mat4x4((nan, nan, 0.000000, 0.000000), (nan, nan, 0.707107, 0.000000), (nan, nan, 0.707107, 0.000000), (nan, nan, -4.242641, 1.000000))
// mat4x4((1.000000, -0.000000, 0.000000, 0.000000), (0.000000, 0.707107, 0.707107, 0.000000), (-0.000000, -0.707107, 0.707107, 0.000000), (0.000000, -0.000000, -4.242641, 1.000000))
