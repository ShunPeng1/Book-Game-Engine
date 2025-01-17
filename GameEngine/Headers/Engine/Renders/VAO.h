#ifndef VAO_H
#define VAO_H

#include <QOpenGLExtraFunctions>

class VAO : protected QOpenGLExtraFunctions {
public:
    VAO(const void* data, GLsizeiptr size);
    ~VAO();

    void bind();
    void unbind();

private:
    GLuint mID;
};

#endif // VAO_H