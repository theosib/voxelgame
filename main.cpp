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
// CubeData cubedata;
GameCamera camera;

#if 0
unsigned int load_vertices(const std::vector<float>& vertices)
{
    // std::cout << "vertices words=" << vertices.size() << " bytes=" << (vertices.size() * sizeof(float)) << std::endl;
    const int which_buffer = 0;
    static unsigned int VBO = 0;
    if (!VBO) glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(which_buffer, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // Actually bind the VBO to the VAO
    glEnableVertexAttribArray(which_buffer);  
    return VBO;
}

unsigned int load_normals(const std::vector<float>& normals)
{
    // std::cout << "normals words=" << normals.size() << " bytes=" << (normals.size() * sizeof(float)) << std::endl;
    const int which_buffer = 1;
    static unsigned int VBO = 0;
    if (!VBO) glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(which_buffer, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // Actually bind the VBO to the VAO
    glEnableVertexAttribArray(which_buffer);  
    return VBO;
}

unsigned int load_texture(const std::vector<float>& texcoords)
{
    // std::cout << "texture words=" << texcoords.size() << " bytes=" << (texcoords.size() * sizeof(float)) << std::endl;
    const int which_buffer = 2;
    static unsigned int VBO = 0;
    if (!VBO) glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(float), texcoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(which_buffer, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // Actually bind the VBO to the VAO
    glEnableVertexAttribArray(which_buffer);  
    return VBO;
}

unsigned int create_vertex_array(const std::vector<float>& vertices, const std::vector<float>& texcoords, const std::vector<float>& normals)
{
    static unsigned int VAO = 0;
    if (!VAO) glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    load_vertices(vertices);
    load_texture(texcoords);
    load_normals(normals);
    glBindVertexArray(0);
    return VAO;
}

void draw(unsigned int VAO, Shader& shader, Texture *tex, int num_vertices)
{
    glBindVertexArray(VAO);
    shader.use();
    tex->use(0);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}

std::vector<float> vertices, texcoords, normals;
int total_vertices;

void block_callback(BlockPos pos, Block *block)
{
    // std::cout << block->getName() << std::endl;
    block->getVertices(vertices, pos);
    block->getTexCoords(texcoords);
    block->getNormals(normals);
    total_vertices += block->numVertices();
}
#endif

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

    // Block *block;
    // block = new Block(BlockPos(0,0,0));
    // World::instance.setBlock(block->getPos(), block);
    // block = new Block(BlockPos(-1,0,0));
    // World::instance.setBlock(block->getPos(), block);
    // block = new Block(BlockPos(2,0,0));
    // World::instance.setBlock(block->getPos(), block);
    
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
    
    // int faces = facing::ALL_FACES & ~bitmask(facing::DOWN);
    // std::cout << "Faces: " << faces << std::endl;
    // cubedata.getVertices(faces, vertices);
    // cubedata.getTexCoords(faces, texcoords);
    
    
    // std::cout << "Iterating blocks:\n";
    // World::instance.iterate_blocks(block_callback);
    // std::cout << "Vertices size to " << vertices.size() << std::endl;
    
        
    // VAO = create_vertex_array(vertices, texcoords, normals);
    // Texture *tex = TextureLibrary::instance.lookup("cobblestone");
    
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
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
#if 0
        for (auto ti=TextureLibrary::instance.getMap().begin(); ti!=TextureLibrary::instance.getMap().end(); ++ti) {
            Texture *tex = ti->second;
            
            vertices.clear();
            texcoords.clear();
            normals.clear();
            total_vertices = 0;
            World::instance.iterate_blocks(block_callback, tex);
            
            if (total_vertices) {
                VAO = create_vertex_array(vertices, texcoords, normals);
                draw(VAO, ourShader, tex, total_vertices);
            }
        }
#endif
        for (auto i=meshes.begin(); i!=meshes.end(); ++i) {
            MeshRenderer *mr = *i;
            mr->draw();
        }
        
        // glm::mat4 projection = glm::perspective(glm::radians(45.0f), window.getAspectRatio(), 0.1f, 100.0f);
        // draw(VAO, ourShader, tex, cubedata.numVertices(faces));
        // draw(VAO, ourShader, tex, total_vertices);
        
        window.next_frame();
    }
    
    
    
    return 0;
}