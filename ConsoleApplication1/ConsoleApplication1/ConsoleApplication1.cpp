

//#include <iostream>
//#include <vector>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\shader/shader.hpp>
//#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\model/objload.hpp>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <fstream>
//
//using namespace glm;
//using namespace std;
//
//// Variables pour la gestion de la souris
//mat4 Model = mat4(1.0f);
//double diffX, diffY, Xpos, Ypos;
//float scaleFactor = 1.0f;  // Facteur d'�chelle de l'objet
//
//struct STRVertex
//{
//    vec3 position;
//    vec3 couleur;
//    vec2 texture;
//    int textureIndex;  // New field to specify which texture to use
//};
//
//vector<GLuint> LoadTextures(const vector<string>& textureFiles) {
//    vector<GLuint> textures;
//    for (const auto& filename : textureFiles) {
//        GLuint textureID;
//        char data[128 * 128 * 3];
//
//        ifstream textureFile(filename, ios::binary);
//        if (!textureFile) {
//            cerr << "Failed to load texture: " << filename << endl;
//            continue;
//        }
//
//        textureFile.read(data, 128 * 128 * 3);
//        textureFile.close();
//
//        glGenTextures(1, &textureID);
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//        textures.push_back(textureID);
//    }
//    return textures;
//}
//
//static void cursor_position(GLFWwindow* window, double x, double y)
//{
//    double diffX = Xpos - x;
//    double diffY = y - Ypos;
//
//    Model = rotate(Model, radians(float(diffX)), vec3(0.0f, 0.0f, 1.0f)); // Z-axis
//    Model = rotate(Model, radians(float(diffY)), vec3(1.0f, 0.0f, 0.0f)); // X-axis
//}
//
//// Callback pour la gestion des boutons de la souris
//void mouse_button(GLFWwindow* window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//        Model = scale(Model, vec3(1.1, 1.1, 1.1));
//    }
//    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
//        Model = scale(Model, vec3(0.9, 0.9, 0.9));
//    }
//}
//int main() {
//
//    if (!glfwInit()) {
//        printf("Could not initialize glfw.\n");
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(1040, 680, "Shaders", NULL, NULL);
//    if (!window) {
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetCursorPosCallback(window, cursor_position);
//    glfwSetMouseButtonCallback(window, mouse_button);
//
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        printf("Could not initialize GLAD\n");
//        return -1;
//    }
//
//
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//
//
//    STRVertex vertices[] = {
//        // Base of the pyramid (using first texture - index 0)
//        {{ 1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0},  // top-right
//        {{-1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, 0},  // top-left
//        {{-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, 0},  // bottom-left
//        {{-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, 0},  // bottom-left
//        {{ 1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, 0},  // bottom-right
//        {{ 1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0},  // top-right
//
//        // Side faces using second texture (index 1)
//        {{ 0.0f, 1.0f,  0.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, 0.5f}, 1},  // Red face 1
//        {{ 1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 1},  // Red face 2
//        {{-1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 1},  // Red face 3
//
//        {{ 0.0f, 1.0f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, 1},  // Green face 1
//        {{-1.0f, 0.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, 1},  // Green face 2
//        {{-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 1},  // Green face 3
//
//        {{ 0.0f, 1.0f,  0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, 1},  // Blue face 1
//        {{-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, 1},  // Blue face 2
//        {{ 1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}, 1},  // Blue face 3
//
//        {{ 0.0f, 1.0f,  0.0f}, {1.0f, 1.0f, 0.0f}, {0.5f, 0.5f}, 1},  // Yellow face 1
//        {{ 1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 1},  // Yellow face 2
//        {{ 1.0f, 0.0f,  1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, 1},  // Yellow face 3
//    };
//
//
//    GLuint VAO, VBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
//    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(sizeof(GL_FLOAT) * 3));
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, couleur));
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, texture));
//    glVertexAttribIPointer(3, 1, GL_INT, sizeof(STRVertex), (void*)offsetof(STRVertex, textureIndex));
//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glEnableVertexAttribArray(2);
//    glEnableVertexAttribArray(3);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//
//    GLuint ShaderProgram = LoadShaders("shader/SimpleVertexShader2.vertexshader", "shader/SimpleFragmentShader2.fragmentshader");
//
//    GLuint TextureID;
//    GLuint text;
//
//    // Load multiple textures
//    vector<GLuint> textures = LoadTextures({
//        "textures/damier.raw",
//        "textures/herbe.raw",  // Add more texture files
//        });
//    cout << "Loaded textures: " << textures.size() << endl;
//    for (size_t i = 0; i < textures.size(); ++i) {
//        cout << "Texture " << i << " ID: " << textures[i] << endl;
//    }
//    // Get texture uniform location
//    GLuint textureArrayID = glGetUniformLocation(ShaderProgram, "textures");
//
//
//    mat4 Projection = perspective(radians(90.0f), 1000.0f / 600.0f, 0.1f, 100.0f);
//    mat4 View = lookAt(vec3(0.5, 1, 2), vec3(0, 0, 0), vec3(0, 1, 0));
//
//    GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");
//
//
//    while (!glfwWindowShouldClose(window)) {
//        glfwGetCursorPos(window, &Xpos, &Ypos);
//
//        mat4 MVP = Projection * View * Model;
//
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glUseProgram(ShaderProgram);
//
//        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
//        //// Activate multiple texture units
//        //for (size_t i = 0; i < textures.size(); ++i) {
//        //    glActiveTexture(GL_TEXTURE0 + i);
//        //    glBindTexture(GL_TEXTURE_2D, textures[i]);
//        //    
//        //}
//        // glUniform1iv(textureArrayID, textures.size(), (int*)textures.data());
//        //
//
//        // Activate and bind textures
//        GLint texLoc = glGetUniformLocation(ShaderProgram, "textures");
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, textures[0]);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, textures[1]);
//
//        // Set texture units
//        int textureUnits[2] = { 0, 1 };
//        glUniform1iv(texLoc, 2, textureUnits);
//
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 18);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//
//    return 0;
//}
