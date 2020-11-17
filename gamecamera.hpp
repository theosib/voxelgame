
#include "camera.hpp"

class GameCamera {
public:
    Camera camera;
    float movement_speed;
    float mouse_sensitivity;
    
    
    GameCamera();
    
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    
    void mouseMovement(float xoffset, float yoffset);
    
    const glm::mat4& getViewMatrix() {
        return camera.getViewMatrix();
    }
};