#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "shader.h"
#include "window.h"
#include "matrix.h"


GLuint createTriangle (void) 
{
    // GLfloat vertices[] = {
    //      0.5f,  0.5f, -5.0f,  0.0f, 1.0f, 0.0f, // top right
    //      0.5f, -0.5f, -5.0f,  0.0f, 1.0f, 1.0f,// bottom right
    //     -0.5f, -0.5f, -5.0f,  1.0f, 0.0f, 0.0f,// bottom left
    //     -0.5f,  0.5f, -5.0f,  1.0f, 0.0f, 1.0f,// top left 
    // };
    GLfloat vertices[] = {
         0.5f,  0.0f, -5.5f,  0.0f, 1.0f, 0.0f, // top right
         0.5f, -0.0f, -4.5f,  0.0f, 1.0f, 1.0f,// bottom right
        -0.5f, -1.0f, -4.5f,  1.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  -0.5f, -5.5f,  1.0f, 0.0f, 1.0f,// top left 
    };
    GLuint indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
}

int main (void) 
{

    GLFWwindow* window = createWindow("OpenGL");
    if (window == NULL)
        return -1;
    

    GLuint triangle = createTriangle();
    GLuint shader = loadShaders("shaders/basic.vert", "shaders/basic.frag");

    glUseProgram(shader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(5.0f);

    mat4 matrix;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        getPerspective(window, matrix);
        setMat4(shader, "projection", matrix);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(triangle);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glfwTerminate();
    return 0;
}