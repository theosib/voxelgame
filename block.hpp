#ifndef _INCLUDED_BLOCK_HPP
#define _INCLUDED_BLOCK_HPP

#include "position.hpp"
#include "facing.hpp"
#include <string>
#include <vector>
#include "cubedata.hpp"
#include "texture.hpp"

class Block;

class BlockShape {
public:
    BlockShape() {}
    virtual ~BlockShape();
    
    virtual Texture *getTexture() = 0;
    virtual bool faceIsSolid(int face) = 0;    
    virtual int numVertices(int show_faces) = 0;
    virtual void getVertices(int show_faces, std::vector<float>& vertices_out, BlockPos pos) = 0;
    virtual void getTexCoords(int show_faces, std::vector<float>& texcoords_out) = 0;
    virtual void getNormals(int show_faces, std::vector<float>& normals_out) = 0;
    virtual Block *newInstance() = 0;
    virtual void print() = 0;
};

class BasicBlockShape : public BlockShape {
private:
    Texture *texture;
    std::vector<float> vertices[facing::NUM_FACES+1];
    std::vector<float> texcoords[facing::NUM_FACES+1];
    std::vector<float> normals[facing::NUM_FACES+1];
    int num_vertices;
    unsigned char solid_faces;
    unsigned char int_tex_coords;
    
    void parseDataLine(const std::string& line, std::vector<float>& values);
    void parseBlockConfig(const std::string& name);
    void generateAutoNormals(int face);
    void addVertices(std::vector<float>& values, int key, int face);
    void parseSolidFaces(const char *line);
    
public:
    void loadShape(const std::string& name);
    
    BasicBlockShape() : solid_faces(0), num_vertices(0), texture(0), int_tex_coords(0) {}
    BasicBlockShape(const std::string& name) : solid_faces(0), num_vertices(0), texture(0), int_tex_coords(0) {
        loadShape(name);
    }
    virtual ~BasicBlockShape();
    
    virtual Texture *getTexture();
    virtual bool faceIsSolid(int face);
    virtual int numVertices(int show_faces);
    virtual void getVertices(int show_faces, std::vector<float>& vertices_out, BlockPos pos);
    virtual void getTexCoords(int show_faces, std::vector<float>& texcoords_out);
    virtual void getNormals(int show_faces, std::vector<float>& normals_out);
    virtual Block *newInstance();
    virtual void print();
};


class Block {
public:
    Block() {}
    virtual ~Block();
    
    virtual bool faceIsVisible(int face) = 0;
    virtual bool faceIsSolid(int face) = 0;
    virtual void updateFaceVisibility(int my_face, Block *neighbor) = 0;
    
    virtual int numVertices() = 0;
    virtual void getVertices(std::vector<float>& vertices_out, BlockPos pos) = 0;
    virtual void getTexCoords(std::vector<float>& texcoords_out) = 0;
    virtual void getNormals(std::vector<float>& normals_out) = 0;
    
    virtual BlockShape *getShape() = 0;
};

class BasicBlock : public Block {
private:
    BlockShape *shape;
    unsigned char show_faces;
    
    void setShowFace(int face, bool val) {
        show_faces &= ~facing::bitmask(face);
        show_faces |= facing::bitmask(face, val);
    }
    
public:
    BasicBlock(BlockShape *s) : shape(s), show_faces(0) {}
    virtual ~BasicBlock();
    
    virtual bool faceIsVisible(int face);
    virtual bool faceIsSolid(int face);
    virtual void updateFaceVisibility(int my_face, Block *neighbor);
    
    virtual int numVertices();
    virtual void getVertices(std::vector<float>& vertices_out, BlockPos pos);
    virtual void getTexCoords(std::vector<float>& texcoords_out);
    virtual void getNormals(std::vector<float>& normals_out);
    
    virtual BlockShape *getShape();
};

class BlockLibrary {
public:
    static BlockLibrary instance;
    
private:
    std::unordered_map<std::string, BlockShape *> shape_library;
    
    BlockShape* loadShape(const std::string& name);
    
public:
    BlockLibrary() {}
    ~BlockLibrary();
    
    BlockShape* getShape(const std::string& name) {
        auto i = shape_library.find(name);
        if (i != shape_library.end()) return i->second;
        return loadShape(name);
    }
    
    Block* newBlock(const std::string& name) {
        BlockShape *shape = getShape(name);
        if (!shape) return 0;
        return shape->newInstance();
    }
};


#endif