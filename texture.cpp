
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "stb/stb_image.h"
#include <iostream>

#include "texture.hpp"

TextureLibrary TextureLibrary::instance;


static std::string base_dir("/Users/millerti/tinker/voxelgame/textures/");

unsigned int Texture::openGL_load_texture()
{
    unsigned int texID;
    glGenTextures(1, &texID); // XXX check for failure
    glBindTexture(GL_TEXTURE_2D, texID); 
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // XXX check for failure
    
    stbi_image_free(data);
    data = 0;
    
    return texID;
}

void Texture::load_image_file(const std::string& base_dir, const std::string& name)
{
    std::string full_path(base_dir + name + ".png");
    int nrChannels;
    
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(full_path.c_str(), &width, &height, &nrChannels, STBI_rgb);
    if (!data) {
        // XXX Push error up to UI
        std::cerr << "Failed to load image\n";
        return;
    }
    std::cout << name << " width=" << width << " height=" << height << " nc=" << nrChannels << std::endl;
    
    inverse_width = 1.0 / width;
    inverse_height = 1.0 / height;
    // texID = openGL_load_texture(data, width, height);
    // stbi_image_free(data);
}


Texture::Texture(const std::string& base_dir, const std::string& name) : tex_name(name)
{
    refcnt = 0;
    texID = 0;
    width = 0;
    height = 0;
    data = 0;
    inverse_width = 0;
    inverse_height = 0;
    
    load_image_file(base_dir, name);
}

Texture::~Texture()
{
    if (texID) glDeleteTextures(1, &texID);
    if (data) stbi_image_free(data);
}

void Texture::use(int texture_unit) {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, getID());
}


void Texture::unlink()
{
    refcnt--;
    
    // Schedule delete
    // if (refcnt == 0) {
    //     texture_library.delete(name);
    // }
}



// TextureLibrary::TextureLibrary() {}

TextureLibrary::~TextureLibrary()
{
    for (auto i = texture_map.begin(); i != texture_map.end(); ++i) {
        delete i->second;
    }
}

Texture* TextureLibrary::load_texture(const std::string& name)
{
    Texture *t = new Texture(base_dir, name);
    texture_map[name] = t;
    return t;
}

Texture *TextureLibrary::lookup(const std::string& name)
{
    auto i = texture_map.find(name);
    if (i != texture_map.end()) return i->second;
    return load_texture(name);
}
    
void TextureLibrary::free(const std::string& name)
{
    auto i = texture_map.find(name);
    if (i == texture_map.end()) return;
    delete i->second;
    texture_map.erase(i);
}

