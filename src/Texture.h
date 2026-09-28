#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/freeglut.h>

class Texture {
public:
    static GLuint loadTexture(const std::string& path);
};

#endif // TEXTURE_H
