// Assignment 03: Lighting and shading

/*References
  Trackball: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/

#include "utils.h"

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cmath>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

// Globals
int screen_width = 640, screen_height = 640;
GLint vModel_uniform, vView_uniform, vProjection_uniform;
GLint lpos_world_uniform, eye_normal_uniform; // introduce the light position and eye normal variables
// GLint vColor_uniform;
glm::mat4 modelT, viewT, projectionT; // The model, view and projection transformations

double oldX, oldY, currentX, currentY;
bool isDragging = false;

void createsphereObject(unsigned int &, unsigned int &);

void setupModelTransformation(unsigned int &);
void setupViewTransformation(unsigned int &);
void setupProjectionTransformation(unsigned int &);
glm::vec3 getTrackBallVector(double x, double y);

int main(int, char **)
{
    // Setup window
    GLFWwindow *window = setupWindow(screen_width, screen_height);
    ImGuiIO &io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");

    // Get handle to light position variable in shader
    lpos_world_uniform = glGetUniformLocation(shaderProgram, "lpos_world");
    if (lpos_world_uniform == -1)
    {
        fprintf(stderr, "Could not bind location: lpos_world\n");
    }

    // Get handle to eye normal variable in shader
    eye_normal_uniform = glGetUniformLocation(shaderProgram, "eye_normal");
    if (eye_normal_uniform == -1)
    {
        fprintf(stderr, "Could not bind location: eye_normal. Specular Lighting Switched Off.\n");
    }

    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram);

    createsphereObject(shaderProgram, VAO);

    oldX = oldY = currentX = currentY = 0.0;
    int prevLeftButtonState = GLFW_RELEASE;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Get current mouse position
        int leftButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_RELEASE)
        {
            isDragging = true;
            currentX = oldX = x;
            currentY = oldY = y;
        }
        else if (leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_PRESS)
        {
            currentX = x;
            currentY = y;
        }
        else if (leftButtonState == GLFW_RELEASE && prevLeftButtonState == GLFW_PRESS)
        {
            isDragging = false;
        }

        // Rotate based on mouse drag movement
        prevLeftButtonState = leftButtonState;
        if (isDragging && (currentX != oldX || currentY != oldY))
        {
            glm::vec3 va = getTrackBallVector(oldX, oldY);
            glm::vec3 vb = getTrackBallVector(currentX, currentY);

            float angle = acos(std::min(1.0f, glm::dot(va, vb)));
            glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
            glm::mat3 camera2object = glm::inverse(glm::mat3(viewT * modelT));
            glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
            modelT = glm::rotate(modelT, angle, axis_in_object_coord);
            glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));

            oldX = currentX;
            oldY = currentY;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);

        {
            ImGui::Begin("Information");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);

        glUniform3f(lpos_world_uniform, -50.0, 500.0, 30.0); // declare the coordinates of the light source

        glUniform3f(eye_normal_uniform, 40.0, -40.0, 40.0); // declare the coordinates of the eye

        glDrawArrays(GL_TRIANGLES, 0, 15000);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    cleanup(window);

    return 0;
}

void createsphereObject(unsigned int &program, unsigned int &sphere_VAO) // create the sphere object of 50x50 vertices
{
    glUseProgram(program);

    // Bind shader variables
    int vVertex_attrib = glGetAttribLocation(program, "vVertex"); // bind the vertex shader variable to the program
    if (vVertex_attrib == -1)
    {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }

    int vNormal_attrib = glGetAttribLocation(program, "vNormal"); // bind the normal shader variable to the program
    if (vNormal_attrib == -1)
    {
        std::cout << "Could not bind location: vNormal\n";
    }

    float x, y, z;
    float pi = 3.14 / 50;
    vector<float> phi; // declare the phi vector(holds values from 0 to pi)

    for (int i = 0; i < 50; i++)
    {
        phi.push_back(pi * i);
    }

    float pii = 6.28 / 50;
    vector<float> theta; // declare the theta vector(holds values from 0 to 2*pi)

    for (int i = 0; i < 50; i++)
    {
        theta.push_back(pii * i);
    }

    // theta and phi vectors hold the values for the angle value in radians for the sphere

    GLfloat sphere_vertices[7500]; // stores the 3 coordinates(x,y,z) of each vertex
    GLfloat vertex_normals[7500];  // stor the 3 coordinates(x,y,z) of each normalized normal
    int a = 0;
    int b = 1;
    int c = 2;
    int p = 0;
    float r = 10.0; // radius of the sphere

    // declare the tangent vectors wrtu and wrtv which we need to find the normal vectors
    glm::vec3 wrtu; // u vector will hold the partial differentiation of the sphere equation wrt phi
    glm::vec3 wrtv; // v vector will hold the partial differentiation of the sphere equation wrt theta
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {                                                         // components of equation of sphere
            sphere_vertices[a] = r * sin(phi[i]) * cos(theta[j]); // x component
            sphere_vertices[b] = r * sin(phi[i]) * sin(theta[j]); // y component
            sphere_vertices[c] = r * cos(phi[i]);                 // z component

            wrtu = {r * cos(phi[i]) * cos(theta[j]), r * cos(phi[i]) * sin(theta[j]), -r * sin(phi[i])};
            wrtv = {-r * sin(phi[i]) * sin(theta[j]), r * sin(phi[i]) * cos(theta[j]), 0};
            glm::vec3 temp = glm::cross(wrtu, wrtv); // cross product of the tangents
            temp = glm::normalize(temp);             // normalizing the normals
            vertex_normals[a] = temp.x;              // x component of normal
            vertex_normals[b] = temp.y;              // y component of normal
            vertex_normals[c] = temp.z;              // z component of normal

            a = a + 3;
            b = b + 3;
            c = c + 3;
            p++;
        }
    }

    GLuint sphere_indices[15000]; // container to store the indices to build the sphere
    GLuint normal_indices[15000]; // container to store the indices of normal
    int k = 0;
    for (int i = 0; i <= 2400; i += 50)
    {
        for (int j = 0; j < 49; j++)
        {
            sphere_indices[k] = i + j;
            sphere_indices[k + 1] = i + j + 50;
            sphere_indices[k + 2] = (i + j + 51);
            sphere_indices[k + 3] = (i + j);
            sphere_indices[k + 4] = (i + j + 51);
            sphere_indices[k + 5] = (i + j + 1);

            normal_indices[k] = i + j;
            normal_indices[k + 1] = i + j + 50;
            normal_indices[k + 2] = (i + j + 51);
            normal_indices[k + 3] = (i + j);
            normal_indices[k + 4] = (i + j + 51);
            normal_indices[k + 5] = (i + j + 1);

            k += 6;
        }
    }
    // the above loop ensures that the 50x50 sheet interconnects but it leaves behind the edge cases
    // to handle those we have the lopp given below
    vector<int> ind;
    for (int i = 0; i < 49; i++)
    { // loop to seal up and bottom
        ind.push_back(2450 + i);
        ind.push_back(i);
        ind.push_back(i + 1);
        ind.push_back(2450 + i);
        ind.push_back(i + 1);
        ind.push_back(2451 + i);
    }

    for (int i = 0; i < 2450; i += 50)
    { // loop to seal left and right
        ind.push_back(i + 49);
        ind.push_back(i + 99);
        ind.push_back(i + 50);
        ind.push_back(i + 49);
        ind.push_back(i + 50);
        ind.push_back(i);
    }

    for (int i = 14406; i < 15000; i++) // storing the above calculated indices in the respective containers
    {
        sphere_indices[i] = ind[i - 14406];
        normal_indices[i] = ind[i - 14406];
    }

    // Generate VAO object
    glGenVertexArrays(1, &sphere_VAO);
    glBindVertexArray(sphere_VAO);

    // Create VBOs for the VAO
    // Position information (data + format)
    //  int nVertices = (6*2)*3; //(6 faces) * (2 triangles each) * (3 vertices each)
    int nVertices = 14994;
    int nvertex_indices = 15000;
    GLfloat *expanded_vertices = new GLfloat[nVertices * 3];
    GLfloat *expanded_normals = new GLfloat[nVertices * 3];
    for (int i = 0; i < nVertices; i++)
    { // generating expanded normals/vertices for normals/vertices
        expanded_vertices[i * 3] = sphere_vertices[sphere_indices[i] * 3];
        expanded_vertices[i * 3 + 1] = sphere_vertices[sphere_indices[i] * 3 + 1];
        expanded_vertices[i * 3 + 2] = sphere_vertices[sphere_indices[i] * 3 + 2];

        expanded_normals[i * 3] = vertex_normals[normal_indices[i] * 3];
        expanded_normals[i * 3 + 1] = vertex_normals[normal_indices[i] * 3 + 1];
        expanded_normals[i * 3 + 2] = vertex_normals[normal_indices[i] * 3 + 2];
    }
    // buffers
    GLuint vertex_VBO; // vertex buffer
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete[] expanded_vertices;

    GLuint normal_VBO; // Normal Buffer
    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nvertex_indices * 3, expanded_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vNormal_attrib);
    glVertexAttribPointer(vNormal_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // delete[] vertex_normals;
    delete[] expanded_normals;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind the VAO to disable changes outside this function.
}

void setupModelTransformation(unsigned int &program)
{
    // Modelling transformations (Model -> World coordinates)
    modelT = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0)); // Model coordinates are the world coordinates

    // Pass on the modelling matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if (vModel_uniform == -1)
    {
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}

void setupViewTransformation(unsigned int &program)
{
    // Viewing transformations (World -> Camera coordinates
    // Camera at (0, 0, 100) looking down the negative Z-axis in a right handed coordinate system
    viewT = glm::lookAt(glm::vec3(40.0, -40.0, 40.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    // Pass-on the viewing matrix to the vertex shader
    glUseProgram(program);
    vView_uniform = glGetUniformLocation(program, "vView");
    if (vView_uniform == -1)
    {
        fprintf(stderr, "Could not bind location: vView\n");
        exit(0);
    }
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
}

void setupProjectionTransformation(unsigned int &program)
{
    // Projection transformation
    projectionT = glm::perspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 0.1f, 1000.0f);

    // Pass on the projection matrix to the vertex shader
    glUseProgram(program);
    vProjection_uniform = glGetUniformLocation(program, "vProjection");
    if (vProjection_uniform == -1)
    {
        fprintf(stderr, "Could not bind location: vProjection\n");
        exit(0);
    }
    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

glm::vec3 getTrackBallVector(double x, double y)
{
    glm::vec3 p = glm::vec3(2.0 * x / screen_width - 1.0, 2.0 * y / screen_height - 1.0, 0.0); // Normalize to [-1, +1]
    p.y = -p.y;                                                                                // Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

    float mag2 = p.x * p.x + p.y * p.y;
    if (mag2 <= 1.0f)
        p.z = sqrtf(1.0f - mag2);
    else
        p = glm::normalize(p); // Nearest point, close to the sides of the trackball
    return p;
}
