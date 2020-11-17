#include "block.hpp"
#include "world.hpp"
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "compat.hpp"

BlockLibrary BlockLibrary::instance;

BlockLibrary::~BlockLibrary() {}
BlockShape::~BlockShape() {}
Block::~Block() {}
BasicBlockShape::~BasicBlockShape() {}
BasicBlock::~BasicBlock() {}

static std::string base_dir("/Users/millerti/tinker/voxelgame/blocks/");


static bool startsWith(const char *line, const char *prefix)
{
    while (*prefix) {
        if (*prefix != *line) return false;
        prefix++;
        line++;
    }
    return true;
}

static bool startsWith(const std::string& line, const char *prefix)
{
    return startsWith(line.c_str(), prefix);
}

static bool delimiter(char c)
{
    if (c == ' ') return true;
    if (c == '\t') return true;
    return false;
}

const char *lineValue(const std::string& line)
{
    const char *p = line.c_str();
    while (*p && !delimiter(*p)) p++;
    if (!*p) return 0;
    while (*p && delimiter(*p)) p++;
    if (!*p) return 0;
    return p;
}

enum Key {
    NONE,
    TEX,
    NORMAL,
    FACE
};

static const unsigned int face_indices[] = {
    0, 1, 2, 0, 2, 3
};

void BasicBlockShape::parseDataLine(const std::string& line, std::vector<float>& values)
{
    const char *p = line.c_str();    
    while (*p) {
        while (*p && isspace(*p)) p++;
        
        float val = atof(p);
        values.push_back(val);
        
        while (*p && !isspace(*p)) p++;
    }
}

void BasicBlockShape::addVertices(std::vector<float>& values, int key, int face)
{
    if (key == TEX && int_tex_coords) {
        for (int i=0; i<values.size(); i+=2) {
            values[i] /= texture->getWidth();
            values[i+1] /= texture->getHeight();
        }
    }
    
    std::vector<float> corners;
    int group_size = (key == TEX) ? 2 : 3;
    for (int i=0; i<6; i++) {
        int offset = group_size * face_indices[i];
        for (int j=0; j<group_size; j++) {
            corners.push_back(values[j+offset]);
        }
    }
    
    switch (key) {
    case TEX:
        texcoords[face].insert(texcoords[face].end(), corners.begin(), corners.end());
        break;
    case FACE:
        vertices[face].insert(vertices[face].end(), corners.begin(), corners.end());
        num_vertices += 6;
        break;
    case NORMAL:
        normals[face].insert(normals[face].end(), corners.begin(), corners.end());
        break;
    }
}

void BasicBlockShape::generateAutoNormals(int face)
{
    std::vector<float>& vert(vertices[face]);
    std::vector<float>& norm(normals[face]);
    const float *vp = vert.data();
    int vs = vert.size();
    for (int i=0; i<vs; i+=18) {
        glm::vec3 a = glm::make_vec3(vp+i);
        glm::vec3 b = glm::make_vec3(vp+i+3);
        glm::vec3 c = glm::make_vec3(vp+i+6);
        glm::vec3 cb = c - b;
        glm::vec3 ab = a - b;
        glm::vec3 x = glm::cross(cb, ab);
        float mag = glm::length(x);
        x /= mag;
        const float *np = glm::value_ptr(x);
        for (int j=0; j<18; j+=3) {
            norm.push_back(np[0]);
            norm.push_back(np[1]);
            norm.push_back(np[2]);
        }
    }
}


void BasicBlockShape::parseSolidFaces(const char *p)
{
    solid_faces = 0;
    
    while (*p) {
        while (*p && isspace(*p)) p++;
        
        int face = facing::faceFromName(p);
        solid_faces |= facing::bitmask(face);
        
        while (*p && !isspace(*p)) p++;
    }
}


void BasicBlockShape::parseBlockConfig(const std::string& name)
{
    std::string fname = base_dir + name + ".dat";
    std::ifstream infile(fname);
    if (!infile.is_open()) {
        std::cout << "Unable to open " << name << std::endl;
        return;
    }
    
    std::string line;
    int face = -1, key = NONE;
    std::vector<float> values;
    
    while (std::getline(infile, line)) {
        // std::cout << line << std::endl;
        
        if (line[0]==' ' || line[0] == '\t') {
            parseDataLine(line, values);
        } else {
            if (values.size()>0) {
                addVertices(values, key, face);
                values.clear();
            }
            
            if (startsWith(line, "tex:")) {
                key = TEX;
                face = facing::faceFromName(line.c_str()+4);
            } else if (startsWith(line, "face:")) {
                key = FACE;
                face = facing::faceFromName(line.c_str()+5);
            } else if (startsWith(line, "normal:")) {
                key = NORMAL;
                face = facing::faceFromName(line.c_str()+7);
            } else if (startsWith(line, "include:")) {
                const char *v = lineValue(line);
                parseBlockConfig(v);
            } else if (startsWith(line, "texture:")) {
                const char *v = lineValue(line);
                texture = TextureLibrary::instance.lookup(v);
            } else if (startsWith(line, "texture-scale:")) {
                const char *v = lineValue(line);
                if (startsWith(v, "float")) {
                    int_tex_coords = false;
                } else if (startsWith(v, "int")) {
                    int_tex_coords = true;
                }
            } else if (startsWith(line, "normals:")) {
                const char *v = lineValue(line);
                if (startsWith(v, "auto")) {
                    for (int i=0; i<=facing::NUM_FACES; i++) {
                        generateAutoNormals(i);
                    }
                }
            } else if (startsWith(line, "solid-faces:")) {
                const char *v = lineValue(line);
                parseSolidFaces(v);
            }
        }
    }
    
    if (values.size()>0) {
        addVertices(values, key, face);
    }    
}


Texture *BasicBlockShape::getTexture()
{
    return texture;
}

void BasicBlockShape::loadShape(const std::string& name)
{
    parseBlockConfig(name);
}


bool BasicBlockShape::faceIsSolid(int face)
{
    return facing::hasFace(solid_faces, face);
}

int BasicBlockShape::numVertices(int show_faces)
{
    return POPCOUNT(show_faces) * 6;
}

void BasicBlockShape::getVertices(int show_faces, std::vector<float>& vertices_out, BlockPos pos)
{
    for (int face=0; face<=facing::NUM_FACES; face++) {
        if (facing::hasFace(show_faces, face)) {
            const float *coords = vertices[face].data();
            int num_coordinates = vertices[face].size();
            for (int i=0; i<num_coordinates; i+=3) {
                vertices_out.push_back(coords[i+0] + pos.X);
                vertices_out.push_back(coords[i+1] + pos.Y);
                vertices_out.push_back(coords[i+2] + pos.Z);
            }
        }
    }
}

void BasicBlockShape::getTexCoords(int show_faces, std::vector<float>& texcoords_out)
{
    for (int face=0; face<=facing::NUM_FACES; face++) {
        if (facing::hasFace(show_faces, face)) {
            texcoords_out.insert(texcoords_out.end(), texcoords[face].begin(), texcoords[face].end());
        }
    }
}

void BasicBlockShape::getNormals(int show_faces, std::vector<float>& normals_out)
{
    for (int face=0; face<=facing::NUM_FACES; face++) {
        if (facing::hasFace(show_faces, face)) {
            normals_out.insert(normals_out.end(), normals[face].begin(), normals[face].end());
        }
    }
}


void BasicBlockShape::print()
{
    std::cout << "BlockShape: nvertices=" << num_vertices << std::endl;
    for (int face=0; face<=facing::NUM_FACES; face++) {
        std::cout << "Vertices: face=" << face << "\n";
        for (int i=0; i<vertices[face].size(); i+=3) {
            std::cout << "    ";
            std::cout << vertices[face][i] << " ";
            std::cout << vertices[face][i+1] << " ";
            std::cout << vertices[face][i+2] << std::endl;
        }
    }
    for (int face=0; face<=facing::NUM_FACES; face++) {
        std::cout << "Texcoords: face=" << face << "\n";
        for (int i=0; i<texcoords[face].size(); i+=2) {
            std::cout << "    ";
            std::cout << texcoords[face][i] << " ";
            std::cout << texcoords[face][i+1] << std::endl;
        }
    }
    for (int face=0; face<=facing::NUM_FACES; face++) {
        std::cout << "Normals: face=" << face << "\n";
        for (int i=0; i<normals[face].size(); i+=3) {
            std::cout << "    ";
            std::cout << normals[face][i] << " ";
            std::cout << normals[face][i+1] << " ";
            std::cout << normals[face][i+2] << std::endl;
        }
    }
}


Block *BasicBlockShape::newInstance()
{
    return new BasicBlock(this);
}

BlockShape* BasicBlock::getShape()
{
    return shape;
}


bool BasicBlock::faceIsVisible(int face)
{
    return facing::hasFace(show_faces, face);
}


bool BasicBlock::faceIsSolid(int face)
{
    return shape->faceIsSolid(face);
}

void BasicBlock::updateFaceVisibility(int my_face, Block *neighbor)
{
    bool neighbor_solid = false;
    if (neighbor) {
        int neighbor_face = facing::oppositeFace(my_face);
        neighbor_solid = neighbor->faceIsSolid(neighbor_face);
    }
    setShowFace(my_face, !neighbor_solid);
    
    // std::cout << getName() << " face " << my_face << " solid=" << (!neighbor_solid) << std::endl;
}

int BasicBlock::numVertices()
{
    return shape->numVertices(show_faces);
}

void BasicBlock::getVertices(std::vector<float>& vertices_out, BlockPos pos)
{
    return shape->getVertices(show_faces, vertices_out, pos);
}

void BasicBlock::getTexCoords(std::vector<float>& texcoords_out)
{
    return shape->getTexCoords(show_faces, texcoords_out);
}

void BasicBlock::getNormals(std::vector<float>& normals_out)
{
    return shape->getNormals(show_faces, normals_out);
}



BlockShape* BlockLibrary::loadShape(const std::string& name)
{
    BlockShape *shape = new BasicBlockShape(name);
    shape_library[name] = shape;
    return shape;
}


