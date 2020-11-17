

#include "gamecamera.hpp"
#include <iostream>

const float SPEED       =  0.1f;
const float SENSITIVITY =  0.1f;


// GameCamera::GameCamera() : camera(0, 3, 3, -90, -45) {
GameCamera::GameCamera() : camera(0, 0.5, 110, -90, 0) {
    movement_speed = SPEED;
    mouse_sensitivity = SENSITIVITY;
}

void GameCamera::moveForward(float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    camera.moveForward(velocity);
}

void GameCamera::moveBackward(float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    camera.moveBackward(velocity);
}

void GameCamera::moveLeft(float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    camera.moveLeft(velocity);
}

void GameCamera::moveRight(float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    camera.moveRight(velocity);
}

void GameCamera::moveUp(float deltaTime)
{
    // std::cout << "deltaTime:" << deltaTime << std::endl;
    float velocity = movement_speed * deltaTime;
    camera.moveUp(velocity);
}

void GameCamera::moveDown(float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    camera.moveDown(velocity);
}


void GameCamera::mouseMovement(float xoffset, float yoffset)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;
    
    camera.rotate(yoffset, xoffset);
}
