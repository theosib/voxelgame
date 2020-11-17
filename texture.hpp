

#ifndef _INCLUDED_TEXTURE_REPO
#define _INCLUDED_TEXTURE_REPO

/*
Global texture repository
*/

#include <string>
#include <unordered_map>

class Texture {
private:
    int refcnt;
    unsigned int texID;
    int width, height;
    double inverse_width, inverse_height;
    std::string tex_name;
    unsigned char *data;
    
    void load_image_file(const std::string& base_dir, const std::string& name);
    unsigned int openGL_load_texture();
    
public:
    Texture(const std::string& base_dir, const std::string& name);
    ~Texture();
    
    void link() { refcnt++; }
    void unlink();
    int getWidth() { return width; }
    int getHeight() { return height; }
    float getTexX(int px) { return px * inverse_width; }
    float getTexY(int py) { return py * inverse_height; }

    int getID() { 
        if (!texID) texID = openGL_load_texture();
        return texID; 
    }
    
    const std::string& getName() { return tex_name; }
    
    void use(int texture_unit);
};

class TextureLibrary {
public:
    static TextureLibrary instance;
    
private:
    std::unordered_map<std::string, Texture *> texture_map;
    Texture *load_texture(const std::string& name);
    
public:
    TextureLibrary() {}
    ~TextureLibrary();
    
    Texture *lookup(const std::string& name);
    void free(const std::string& name);
    
    std::unordered_map<std::string, Texture *>& getMap() {
        return texture_map;
    }
};


#endif
