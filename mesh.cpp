
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "mesh.hpp"
#include "world.hpp"
#include <iostream>

void MeshData::clear()
{
    vertices.clear();
    texcoords.clear();
    normals.clear();
    total_vertices = 0;
}

void MeshBuffer::load(unsigned int VAO, const std::vector<float>& list)
{
    glBindVertexArray(VAO);
    if (!VBO) glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, list.size() * sizeof(float), list.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(attribute_number, num_components, GL_FLOAT, GL_FALSE, 0, (void*)0); // Actually bind the VBO to the VAO
    glEnableVertexAttribArray(attribute_number);
    glBindVertexArray(0);
}

void MeshRenderer::compute_arrays()
{
    data.clear();
    World::instance.iterate_blocks(&data, tex);
    
    // std::cout << "Texture " << tex->getName() << " tv=" << data.total_vertices << " sv=" << data.vertices.size() <<
    //     " st=" << data.texcoords.size() << " sn=" << data.normals.size() << std::endl;
}

void MeshRenderer::load_buffers()
{
    if (!VAO) glGenVertexArrays(1, &VAO);
    vertex_buffer.load(VAO, data.vertices);
    texcoord_buffer.load(VAO, data.texcoords);
    normals_buffer.load(VAO, data.normals);
}

void MeshRenderer::draw()
{
    glBindVertexArray(VAO);
    shader->use();
    tex->use(0);
    glDrawArrays(GL_TRIANGLES, 0, data.total_vertices);
}
