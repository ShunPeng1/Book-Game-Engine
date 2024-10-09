#include "Engine/Render/Shader.h"
#include <QFile>
#include <QTextStream>


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    initializeOpenGLFunctions();


    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        // open files
        QFile vShaderFile(vertexPath);
        QFile fShaderFile(fragmentPath);

        if (!vShaderFile.open(QIODevice::ReadOnly | QIODevice::Text) || !fShaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw std::runtime_error("Failed to open shader files");
        }

        QTextStream vShaderStream(&vShaderFile);
        QTextStream fShaderStream(&fShaderFile);

        // read file's buffer contents into strings
        vertexCode = vShaderStream.readAll().toStdString();
        fragmentCode = fShaderStream.readAll().toStdString();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    // shader Program
    mID = glCreateProgram();
    glAttachShader(mID, vertex);
    glAttachShader(mID, fragment);
    glLinkProgram(mID);
    CheckCompileErrors(mID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(mID);
}

void Shader::SetUniformBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(mID, name.c_str()), (int)value);
}

void Shader::SetUniformInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}

void Shader::SetUniformFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
}

void Shader::SetUniformVec2(const std::string& name, glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(mID, name.c_str()), value.x, value.y);
}

void Shader::SetUniformVec3(const std::string& name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(mID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetUniformVec4(const std::string& name, glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(mID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMat2(const std::string& name, glm::mat2 value)
{
	glUniformMatrix2fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniformMat3(const std::string& name, glm::mat3 value)
{
	glUniformMatrix3fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
