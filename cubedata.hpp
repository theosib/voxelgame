#ifndef _INCLUDED_CUBEDATA_HPP
#define _INCLUDED_CUBEDATA_HPP

#include <vector>

class CubeData {
public:
    static int numVertices(unsigned int faces);
    static void getVertices(unsigned int faces, int X, int Y, int Z, std::vector<float>& vertices);
    static void getNormals(unsigned int faces, std::vector<float>& normals);
    static void getTexCoords(unsigned int faces, std::vector<float>& texcoords);
};

#endif