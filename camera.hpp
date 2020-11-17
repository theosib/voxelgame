

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {    
public:
    glm::vec3 world_up;
    glm::vec3 position;
    glm::vec3 front, up, right;
    float pitch, yaw;
    glm::mat4 view_matrix;
    
    Camera(const glm::vec3& pos_in, float yaw_in, float pitch_in);
    Camera(float posX, float posY, float posZ, float yaw_in, float pitch_in);
    
    void moveForward(float distance);
    void moveBackward(float distance);
    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    
    void rotate(float pitch_change, float yaw_change);
    
    const glm::mat4& getViewMatrix() {
        return view_matrix;
    }
    
private:
    void update();
};

