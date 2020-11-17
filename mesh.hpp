#ifndef _INCLUDED_MESH_HPP
#define _INCLUDED_MESH_HPP

#include <vector>
#include "texture.hpp"
#include "shader.hpp"

struct MeshData {
    std::vector<float> vertices, texcoords, normals;
    int total_vertices;
    
    void clear();
};

struct MeshBuffer {
    int attribute_number;
    unsigned int VBO;
    int num_components;
    
    MeshBuffer(int an, int nc) : attribute_number(an), num_components(nc), VBO(0) {}
    void load(unsigned int VAO, const std::vector<float>& list);
};

class MeshRenderer {
private:
    unsigned int VAO;
    Texture *tex;
    Shader *shader;
    MeshData data;
    MeshBuffer vertex_buffer, texcoord_buffer, normals_buffer;
    
public:
    MeshRenderer(Texture *t, Shader *s) : tex(t), shader(s), VAO(0),
        vertex_buffer(0, 3), texcoord_buffer(2, 2), normals_buffer(1, 3) {}
    
    void compute_arrays();
    void load_buffers();
    void draw();
};


#endif