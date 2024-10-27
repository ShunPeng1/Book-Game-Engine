#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QOpenGLExtraFunctions>

#include "Vertex.h"
#include "Texture.h"
#include "ShaderProgram.h"

class Mesh : public QOpenGLExtraFunctions {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, GLenum drawMode);
    virtual void init();
    virtual void draw(ShaderProgram& shader);
protected:
    //  render data
    unsigned int mVAO, mVBO, mEBO;
    GLenum mDrawMode; // Member variable to store the drawing mode

    void setupMesh();
};

#endif // MESH_H