#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include "facing.hpp"
#include "gamewindow.hpp"
#include "cubedata.hpp"
#include "texture.hpp"
#include "world.hpp"
#include <stdlib.h>

GameWindow window;
Shader ourShader("vertex.glsl", "fragment.glsl");
GameCamera camera;

void makeSphere()
{
    for (int x=-101; x<=101; x++) {
        for (int y=-101; y<=101; y++) {
            for (int z=-101; z<=101; z++) {
                double r = sqrt(x*x + y*y + z*z);
                if (r <= 30 && r >= 10) {
                    BlockPos pos(x, y, z);
                    // Block *block = new Block(pos);
                    Block *block;
                    
                    int which = random() % 4;
                    switch (which) {
                    case 0:
                        block = BlockLibrary::instance.newBlock("cobblestone");
                        break;
                    case 1:
                        block = BlockLibrary::instance.newBlock("wood");
                        break;
                    case 2:
                        block = BlockLibrary::instance.newBlock("steel");
                        break;
                    case 3:
                        block = BlockLibrary::instance.newBlock("dirt");
                        break;
                    }
                    
                    World::instance.setBlock(pos, block);
                }
            }
        }
    }
}

int main()
{
    unsigned int VAO;

    makeSphere();
    
    // Block *block = BlockLibrary::instance.newBlock("cobblestone");
    // World::instance.setBlock(BlockPos(0,0,0), block);
    // block = BlockLibrary::instance.newBlock("cobblestone");
    // World::instance.setBlock(BlockPos(-1,0,0), block);
    
    // block->getShape()->print();
    
    // exit(0);
    
    window.create();
    window.setCamera(&camera);
    window.setFOV(glm::radians(45.0f));
    ourShader.compile();
        
    std::vector<MeshRenderer *> meshes;
    for (auto ti=TextureLibrary::instance.getMap().begin(); ti!=TextureLibrary::instance.getMap().end(); ++ti) {
        Texture *tex = ti->second;
        std::cout << "Computing for texture " << tex->getName() << std::endl;
        MeshRenderer *mr = new MeshRenderer(tex, &ourShader);
        meshes.push_back(mr);
        mr->compute_arrays();
        mr->load_buffers();
    }

    while (!window.shouldClose()) {
        glm::mat4 projection = window.getProjectionMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", camera.getViewMatrix());
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (auto i=meshes.begin(); i!=meshes.end(); ++i) {
            MeshRenderer *mr = *i;
            mr->draw();
        }
        
        window.next_frame();
    }
    
    return 0;
}
