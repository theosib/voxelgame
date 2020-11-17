#ifndef _INCLUDED_GAME_WINDOW
#define _INCLUDED_GAME_WINDOW

#include "window.hpp"
#include "gamecamera.hpp"
#include "shader.hpp"

class GameWindow : public Window {
private:
    GameCamera *camera;
    
public:
    GameWindow() {}
    virtual ~GameWindow();
    
    virtual void process_input();
    virtual void key_callback(int key, int scancode, int action, int mods);
    virtual void mouse_callback(double xpos, double ypos);
    
    void setCamera(GameCamera *cam) {
        camera = cam;
    }
};

#endif