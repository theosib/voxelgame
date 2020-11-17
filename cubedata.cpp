

#include "cubedata.hpp"
#include "compat.hpp"
#include "facing.hpp"
#include <iostream>

// DOWN, UP, NORTH, SOUTH, WEST, EAST

// cube corners
static const float corner_coordinates[6][18] = {
    // bottom
    {
         0,  0,  1,
         0,  0,  0,
         1,  0,  0,
         0,  0,  1,
         1,  0,  0,
         1,  0,  1,
    },
    // top
    {
         0,  1,  0,
         0,  1,  1,
         1,  1,  1,
         0,  1,  0,
         1,  1,  1,
         1,  1,  0,
    },
    // north
    {
         1,  1,  0,
         1,  0,  0,
         0,  0,  0,
         1,  1,  0,
         0,  0,  0,
         0,  1,  0,
    },
    // south
    {
         0,  1,  1,
         0,  0,  1,
         1,  0,  1,
         0,  1,  1,
         1,  0,  1,
         1,  1,  1,
    },
    // west
    {
         0,  1,  0,
         0,  0,  0,
         0,  0,  1,
         0,  1,  0,
         0,  0,  1,
         0,  1,  1,
    },
    // east
    {
         1,  1,  1,
         1,  0,  1,
         1,  0,  0,
         1,  1,  1,
         1,  0,  0,
         1,  1,  0,
    },
};
static constexpr int num_coordinates = 18;

static const float corner_normals[6][18] = {
    // bottom
    {
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
    },
    // top
    {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
    },
    // north
    {
        0, 0, -1,
        0, 0, -1,
        0, 0, -1,
        0, 0, -1,
        0, 0, -1,
        0, 0, -1,
    },
    // south
    {
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
    },
    // west
    {
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
    },
    // east
    {
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
    },
};



static const float texture_coordinates[6][12] = {
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1,},
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1,},
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1,},
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1,},
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1,},
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1,},
};
static constexpr int num_texcoords = 12;

int CubeData::numVertices(unsigned int faces)
{
    return POPCOUNT(faces) * 6;
}

void CubeData::getVertices(unsigned int faces, int X, int Y, int Z, std::vector<float>& vertices)
{
    for (int face=0; face<facing::NUM_FACES; face++) {
        if (facing::hasFace(faces, face)) {
            const float *coords = corner_coordinates[face];
            for (int i=0; i<num_coordinates; i+=3) {
                vertices.push_back(coords[i+0] + X);
                vertices.push_back(coords[i+1] + Y);
                vertices.push_back(coords[i+2] + Z);
            }
            // vertices.insert(std::end(vertices), std::begin(corner_coordinates[face]), std::end(corner_coordinates[face]));
        }
    }
}

void CubeData::getNormals(unsigned int faces, std::vector<float>& normals)
{
    for (int face=0; face<facing::NUM_FACES; face++) {
        if (facing::hasFace(faces, face)) {
            normals.insert(std::end(normals), std::begin(corner_normals[face]), std::end(corner_normals[face]));
        }
    }
}

void CubeData::getTexCoords(unsigned int faces, std::vector<float>& texcoords)
{
    for (int face=0; face<facing::NUM_FACES; face++) {
        if (facing::hasFace(faces, face)) {
            texcoords.insert(std::end(texcoords), std::begin(texture_coordinates[face]), std::end(texture_coordinates[face]));
        }
    }
}

// const float* CubeData::getColors()
// {
//     return 0;
// }
