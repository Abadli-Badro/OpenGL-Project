
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\shader/shader.hpp>
#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\model/objload.hpp>
#define STB_IMAGE_IMPLEMENTATION 
#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\libs/stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <fstream>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

// Variables pour la gestion de la souris
mat4 Model = mat4(1.0f);
mat4 Projection = perspective(radians(90.0f), 1000.0f / 600.0f, 0.1f, 100.0f);
mat4 View = lookAt(vec3(0, 0, 0), // Camera position at center
    vec3(-0.5, 0, -1), // Looking forward
    vec3(0, 1, 0)); // Up vector
double diffX, diffY, Xpos, Ypos;
float scaleFactor = 1.0f;  // Facteur d'�chelle de l'objet

float skyboxVertices[] =
{
    //   Coordinates
    -1.0f, -1.0f,  1.0f,//        7--------6
     1.0f, -1.0f,  1.0f,//       /|       /|
     1.0f, -1.0f, -1.0f,//      4--------5 |
    -1.0f, -1.0f, -1.0f,//      | |      | |
    -1.0f,  1.0f,  1.0f,//      | 3------|-2
     1.0f,  1.0f,  1.0f,//      |/       |/
     1.0f,  1.0f, -1.0f,//      0--------1
    -1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
    // Right
    1, 2, 6,
    6, 5, 1,
    // Left
    0, 4, 7,
    7, 3, 0,
    // Top
    4, 5, 6,
    6, 7, 4,
    // Bottom
    0, 3, 2,
    2, 1, 0,
    // Back
    0, 1, 5,
    5, 4, 0,
    // Front
    3, 7, 6,
    6, 2, 3
};


struct STRVertex
{
    vec3 position;
    vec3 couleur;
    vec2 texture;
};

void InitTexture(GLuint& text) {
    char data[128 * 128 * 3];
    std::ifstream VertexTexture("textures/herbe.raw", std::ios::binary);
    for (int i = 0; i < 128 * 128 * 3; ++i) {
        VertexTexture.read(reinterpret_cast<char*>(&data[i]), 1);
    }
    VertexTexture.close();
    glGenTextures(1, &text);
    glBindTexture(GL_TEXTURE_2D, text);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

static void cursor_position(GLFWwindow* window, double x, double y)
{
    // Add sensitivity factor
    float sensitivity = 0.1f;
    double diffX = (x - Xpos) * sensitivity;
    double diffY = (Ypos - y) * sensitivity; // Inverted Y-axis

    // Update yaw and pitch
    static float yaw = -90.0f; // Initialize to -90 degrees to look forward
    static float pitch = 0.0f;

    yaw += diffX;
    pitch += diffY;

    // Constrain the pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calculate the new front vector
    vec3 front;
    front.x = cos(radians(yaw)) * cos(radians(pitch));
    front.y = sin(radians(pitch));
    front.z = sin(radians(yaw)) * cos(radians(pitch));
    front = normalize(front);

    // Update the view matrix
    View = lookAt(vec3(0, 0, 0), vec3(0, 0, 0) + front, vec3(0, 1, 0));

    // Update previous position
    Xpos = x;
    Ypos = y;
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

    if (!glfwInit()) {
        printf("Could not initialize glfw.\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1040, 680, "Shaders", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position);
    glfwSetMouseButtonCallback(window, mouse_button);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Could not initialize GLAD\n");
        return -1;
    }


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    STRVertex vertices[] = {
        // Base of the pyramid (blue)
        {{ 1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},  // top-right
        {{-1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // top-left
        {{-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // bottom-left

        {{-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // bottom-left
        {{ 1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // bottom-right
        {{ 1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},  // top-right

        // Other faces (different colors, same texture mapping)
        {{ 0.0f, 1.0f,  0.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, 0.5f}},  // Red face 1
        {{ 1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // Red face 2
        {{-1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Red face 3

        {{ 0.0f, 1.0f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},  // Green face 1
        {{-1.0f, 0.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // Green face 2
        {{-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // Green face 3

        {{ 0.0f, 1.0f,  0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // Blue face 1
        {{-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // Blue face 2
        {{ 1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}},  // Blue face 3

        {{ 0.0f, 1.0f,  0.0f}, {1.0f, 1.0f, 0.0f}, {0.5f, 0.5f}},  // Yellow face 1
        {{ 1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // Yellow face 2
        {{ 1.0f, 0.0f,  1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // Yellow face 3
    };


    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(sizeof(GL_FLOAT) * 3));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, couleur));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, texture));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create VAO, VBO, and EBO for the skybox
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    GLuint ShaderProgram = LoadShaders("shader/SimpleVertexShader2.vertexshader", "shader/SimpleFragmentShader2.fragmentshader");
    GLuint skyboxShader = LoadShaders("shader/skybox.vertexshader", "shader/skybox.fragmentshader");


    GLuint TextureID;
    GLuint text;
    InitTexture(text);

    TextureID = glGetUniformLocation(ShaderProgram, "ourTexture");
    // Take care of all the light related things
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);

    GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");

    std::string parentDir = "C:/Users/BAH/Documents/TP SIM/ConsoleApplication1/ConsoleApplication1/textures";
    std::string facesCubemap[6] =
    {
        parentDir + "/right.jpg",
        parentDir + "/left.jpg",
        parentDir + "/top.jpg",
        parentDir + "/bottom.jpg",
        parentDir + "/front.jpg",
        parentDir + "/back.jpg"
    };

    // Creates the cubemap texture object
    unsigned int cubemapTexture;
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // These are very important to prevent seams
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Cycles through all the textures and attaches them to the cubemap object
    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
            stbi_image_free(data);
        }
    }

    while (!glfwWindowShouldClose(window)) {
        glfwGetCursorPos(window, &Xpos, &Ypos);

        mat4 MVP = Projection * View * Model;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        //glUseProgram(ShaderProgram);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShader);

        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, text);
        //glUniform1i(TextureID, 0);

        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 18);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
