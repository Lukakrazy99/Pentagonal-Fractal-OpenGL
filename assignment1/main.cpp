//
//  main.cpp
//  
//
//  Created by Luís Amândio on 10/10/2024. 
//  Copyright © 2024 Luís Amândio. All rights reserved.
//

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>

#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// GLM header file
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// shaders header file
#include <common/shader.hpp>

// Vertex array object (VAO)
GLuint VertexArrayID;

// Vertex buffer object (VBO)
GLuint vertexbuffer;

// color buffer object (CBO)
GLuint colorbuffer;

// GLSL program from the shaders
GLuint programID;

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
//change the total of number of vertices
#define NUMBER_OF_VERTICES 5
//change the total of inside lines of each quandragle
#define DEEP_NUMBER 20
#define T 0.15
#define LINEBUFFERSIZE  ((NUMBER_OF_VERTICES * 6) + (DEEP_NUMBER * NUMBER_OF_VERTICES * 6))


//--------------------------------------------------------------------------------
void transferDataToGPUMemory(void)
{
    // VAO
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders(
        "C:/Users/luis_/Desktop/Uni/3Ano/1Semestre/CG/ogl-master/assignment1/SimpleVertexShader.vertexshader",
        "C:/Users/luis_/Desktop/Uni/3Ano/1Semestre/CG/ogl-master/assignment1/SimpleFragmentShader.fragmentshader");


    float pi = glm::pi<float>();
    srand((unsigned)time(0));

    GLfloat xpos = 0.0f;
    GLfloat ypos = 0.0f;
    GLdouble angle = 0.0;
    GLint radius = 15.0f;

    static GLfloat guardarPontos[NUMBER_OF_VERTICES][8];

    GLfloat g_vertex_buffer_data[NUMBER_OF_VERTICES * 3];
    GLfloat g_color_buffer_data[NUMBER_OF_VERTICES * 3];

    static GLfloat line_vertex_buffer_data[LINEBUFFERSIZE];
    static GLfloat line_color_buffer_data[LINEBUFFERSIZE];

    for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
        angle = (2.0f * pi * i / NUMBER_OF_VERTICES);
        g_vertex_buffer_data[i * 3] = xpos + radius * cos(angle); 
        g_vertex_buffer_data[(i * 3) + 1] = ypos + radius * sin(angle); 
        g_vertex_buffer_data[(i * 3) + 2] = 0.0f; 

        g_color_buffer_data[i * 3] = 0.9f;
        g_color_buffer_data[(i * 3) + 1] = 0.7f;
        g_color_buffer_data[(i * 3) + 2] = 0.0f;

        guardarPontos[i][6] = xpos + radius * cos(angle);
        guardarPontos[i][7] = ypos + radius * sin(angle);
    }


    for (int i = 0; i < NUMBER_OF_VERTICES; i++) {

        int nextVertexIndex = (i + 1) % NUMBER_OF_VERTICES;
        double xAverage = (g_vertex_buffer_data[i * 3] + g_vertex_buffer_data[nextVertexIndex * 3]) / 2.0;
        double tolerance = 1e-5;  
        double ySum = g_vertex_buffer_data[i * 3 + 1] + g_vertex_buffer_data[nextVertexIndex * 3 + 1];

        if (fabs(ySum) < tolerance) {
            ySum = 0.0;
        }

        double yAverage = ySum / 2.0;
        line_vertex_buffer_data[i * 6] = xAverage;
        line_vertex_buffer_data[i * 6 + 1] = yAverage;
        line_vertex_buffer_data[i * 6 + 2] = 0.0f;


        line_vertex_buffer_data[i * 6 + 3] = 0.0f;
        line_vertex_buffer_data[i * 6 + 4] = 0.0f;
        line_vertex_buffer_data[i * 6 + 5] = 0.0f;


        line_color_buffer_data[i * 6] = 0.9f;
        line_color_buffer_data[i * 6 + 1] = 0.7f;
        line_color_buffer_data[i * 6 + 2] = 0.0f;

        line_color_buffer_data[i * 6 + 3] = 1.0f;
        line_color_buffer_data[i * 6 + 4] = 1.0f;
        line_color_buffer_data[i * 6 + 5] = 1.0f;

        guardarPontos[i][0] = xAverage;
        guardarPontos[i][1] = yAverage;
        guardarPontos[i][2] = 0;
        guardarPontos[i][3] = 0;
        guardarPontos[i + 1][4] = xAverage;
        guardarPontos[i + 1][5] = yAverage;
        if (i == 4) {
            guardarPontos[0][4] = xAverage;
            guardarPontos[0][5] = yAverage;
        }

    }
    int acc = 0;
    for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
        double x = 0, y = 0, x1 = 0, y1 = 0;
        int j = 0, acc1 = 0;

        while (true) {
            if (acc1 == DEEP_NUMBER) {
                break;
            }
            if (j == 6) {
                x = guardarPontos[i][6];
                y = guardarPontos[i][7];
                x1 = guardarPontos[i][0];
                y1 = guardarPontos[i][1];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6] = guardarPontos[i][j - 2];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 1] = guardarPontos[i][j - 1];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 2] = 0.0f;

            }
            else if (j == 0) {
                x = guardarPontos[i][j + 0];
                y = guardarPontos[i][j + 1];
                x1 = guardarPontos[i][j + 2];
                y1 = guardarPontos[i][j + 3];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6] = guardarPontos[i][6];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 1] = guardarPontos[i][7];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 2] = 0.0f;
            }
            else {
                x = guardarPontos[i][j + 0];
                y = guardarPontos[i][j + 1];
                x1 = guardarPontos[i][j + 2];
                y1 = guardarPontos[i][j + 3];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6] = guardarPontos[i][j - 2];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 1] = guardarPontos[i][j - 1];
                line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 2] = 0.0f;

            }

            double totalX = (1 - T) * x + T * x1;
            double totalY = (1 - T) * y + T * y1;

            guardarPontos[i][j + 0] = totalX;
            guardarPontos[i][j + 1] = totalY;

            line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 3] = totalX;
            line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 4] = totalY;
            line_vertex_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 5] = 0.0f;

            line_color_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6] = 0.9f;
            line_color_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 1] = 0.7f;
            line_color_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 2] = 0.0f;

            line_color_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 3] = 1.0f;
            line_color_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 4] = 1.0f;
            line_color_buffer_data[(acc * 6) + NUMBER_OF_VERTICES * 6 + 5] = 1.0f;

            if (j == 6) {
                j = -2;
            }

            acc1 += 1;
            j += 2;
            acc += 1;
        }
   }
    // Combine both vertex data (original shape + lines)
    const int totalVertices = NUMBER_OF_VERTICES * DEEP_NUMBER * 6;
    GLfloat combined_vertex_buffer_data[totalVertices * 3];

    memcpy(combined_vertex_buffer_data, g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
    memcpy(&combined_vertex_buffer_data[NUMBER_OF_VERTICES * 3], line_vertex_buffer_data, sizeof(line_vertex_buffer_data));

    // Move combined vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(combined_vertex_buffer_data), NULL, GL_STATIC_DRAW);



    // Use glBufferSubData to fill in the buffer data
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), sizeof(line_vertex_buffer_data), line_vertex_buffer_data);


    // Combine color data (circle + lines)
    GLfloat combined_color_buffer_data[totalVertices * 3];
    memcpy(combined_color_buffer_data, g_color_buffer_data, sizeof(g_color_buffer_data));
    memcpy(&combined_color_buffer_data[NUMBER_OF_VERTICES * 3], line_color_buffer_data, sizeof(line_color_buffer_data));

    // Move combined color data to video memory; specifically to CBO called colorbuffer
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(combined_color_buffer_data), NULL, GL_STATIC_DRAW);

    // Use glBufferSubData to fill in the color buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_color_buffer_data), g_color_buffer_data);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), sizeof(line_color_buffer_data), line_color_buffer_data);
}

//--------------------------------------------------------------------------------
void cleanupDataFromGPU()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
}

//--------------------------------------------------------------------------------
void draw(void)
{

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    glm::mat4 mvp = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f);
    unsigned int matrix = glGetUniformLocation(programID, "mvp");
    glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glEnable(GL_PROGRAM_POINT_SIZE);

    // Draw the circle using the first NUMBER_OF_VERTICES
    glDrawArrays(GL_LINE_LOOP, 0, NUMBER_OF_VERTICES);

    // Draw the line (the last 2 vertices in the buffer)
    glDrawArrays(GL_LINES, NUMBER_OF_VERTICES, LINEBUFFERSIZE);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

//--------------------------------------------------------------------------------
int main(void)
{
    // Initialize the library
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Exercicio 6", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Põe a cor do ecrã a preto
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // transfer my data (vertices, colors, and shaders) to GPU side
    transferDataToGPUMemory();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Cleanup VAO, VBOs, and shaders from GPU
    cleanupDataFromGPU();

    glfwTerminate();
    return 0;
}
