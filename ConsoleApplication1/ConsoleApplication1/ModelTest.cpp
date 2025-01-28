/*

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\shader/shader.hpp>
#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\model/objload.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace glm;

mat4 Model = mat4(1.0f);

// Variables pour la gestion de la souris
double diffX, diffY, Xpos, Ypos;
float scaleFactor = 1.0f;  // Facteur d'�chelle de l'objet

// Callback pour ajuster la taille de la fen�tre lorsque celle-ci est redimensionn�e
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback pour la gestion des touches du clavier
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (key)
        {
        case GLFW_KEY_R:
            glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
            break;
        case GLFW_KEY_V:
            glClearColor(0.0f, 1.0f, 0.0f, 0.5f);
            break;
        case GLFW_KEY_B:
            glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:
            glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        }
    else if (action == GLFW_RELEASE)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    }
}

// Callback pour la gestion de la position du curseur pour la rotation du mod�le
static void cursor_position(GLFWwindow* window, double x, double y)
{
    double diffX = Xpos - x;
    double diffY = y - Ypos;

    Model = rotate(Model, radians(float(diffX)), vec3(0.0f, 0.0f, 1.0f)); // Z-axis
    Model = rotate(Model, radians(float(diffY)), vec3(1.0f, 0.0f, 0.0f)); // X-axis
}

// Callback pour la gestion des boutons de la souris
void mouse_button(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Model = scale(Model, vec3(1.1, 1.1, 1.1));
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        Model = scale(Model, vec3(0.9, 0.9, 0.9));
    }
}


int main() {
    std::vector<vec3> position;
    std::vector<vec2> texture;
    std::vector<vec3> normals;

    loadOBJ("model/torus.obj", position, texture, normals);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Torus", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position);
    glfwSetMouseButtonCallback(window, mouse_button);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Could not initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint VAO, VBOPosition, VBOColor;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBOPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBOPosition);
    glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(vec3), &position[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

    glGenBuffers(1, &VBOColor);
    glBindBuffer(GL_ARRAY_BUFFER, VBOColor);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint ShaderProgram = LoadShaders("shader/SimpleVertexShader3.vertexshader", "shader/SimpleFragmentShader3.fragmentshader");

    GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");
    mat4 Projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    mat4 View = lookAt(vec3(3, 2, 5), vec3(0, 0, 0), vec3(0, 1, 0));

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        glfwGetCursorPos(window, &Xpos, &Ypos);



        mat4 MVP = Projection * View * Model;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ShaderProgram);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, position.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBOPosition);
    glDeleteBuffers(1, &VBOColor);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
*/