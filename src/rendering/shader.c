#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

inline static char*
readFile(const char* path)
{
    FILE* file = fopen(path, "rb");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file %s.\n", path);
        fclose(file);
        return NULL;
    }
    else
    {
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* data = (char*)calloc(size, sizeof(char));
        fread(data, 1, size, file);

        fclose(file);
        return data;
    }
}

inline static GLuint
loadShader(const char* path, GLenum type)
{
    GLuint shader = glCreateShader(type);
    char* content = readFile(path);

    printf("Compiling shader %s.\n", path);
    glShaderSource(shader, 1, (const char**)&content, NULL);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader failed to compile.\n%s\n", infoLog);
        exit(-1);
    }

    free(content);
    return shader;
}

GLuint
loadShaders(const char* vertPath, const char* fragPath)
{
    GLuint program = glCreateProgram();
    GLuint vertShader = loadShader(vertPath, GL_VERTEX_SHADER);
    GLuint fragShader = loadShader(fragPath, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Shader linking failed.\n%s\n", infoLog);
        exit(-1);
    }

    return program;
}

void
setMat4(GLuint program, const char* name, float* matrix)
{
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE,
        matrix);
}

void
setVec3(GLuint program, const char* name, float* vector)
{
    glUseProgram(program);
    glUniform3fv(glGetUniformLocation(program, name), 1, vector);
}
