
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\shader/shader.hpp>
#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\model/objload.hpp>
#include <random>
#define STB_IMAGE_IMPLEMENTATION 
#include <C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\libs/stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <fstream>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

vec3 cameraPos = vec3(21.0f, 1.0f, 13.5f);
vec3 cameraFront = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.005f;

std::vector<vec3> grassVertices;
std::vector<vec2> grassUVs;
std::vector<vec3> grassNormals;

bool loadGrassModel() {
    bool res = loadOBJ("model/torus.obj", grassVertices, grassUVs, grassNormals);
    if (!res) {
        std::cerr << "Failed to load the grass model." << std::endl;
        return false;
    }
    return true;
}

// Variables pour la gestion de la souris
mat4 Model = mat4(1.0f);
mat4 Projection = perspective(radians(90.0f), 1000.0f / 600.0f, 0.1f, 100.0f);
mat4 View = lookAt(
     cameraPos, // Move back slightly
     cameraFront,  // Look at center
     cameraUp);

double diffX, diffY, Xpos, Ypos;

int n = 500;  // Number of vertices per side
float d = 1.0f;  // Distance between vertices

int textureScale = 50;
std::vector<std::vector<float>> heightMap;


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

STRVertex vertices[] = {
    // Base (blue)
    {{ 0.3f, 3.0f,  0.3f}, {0.0f, 0.0f, 1.0f}, {1.0f * textureScale, 1.0f * textureScale}},
    {{-0.3f, 3.0f,  0.3f}, {0.0f, 0.0f, 1.0f}, {0.0f * textureScale, 1.0f * textureScale}},
    {{-0.3f, 3.0f,  0.3f}, {0.0f, 0.0f, 1.0f}, {0.0f * textureScale, 0.0f * textureScale}},

    {{-0.3f, 3.0f, -0.3f}, {0.0f, 0.0f, 1.0f}, {0.0f * textureScale, 0.0f * textureScale}},
    {{ 0.3f, 3.0f, -0.3f}, {0.0f, 0.0f, 1.0f}, {1.0f * textureScale, 0.0f * textureScale}},
    {{ 0.3f, 3.0f,  0.3f}, {0.0f, 0.0f, 1.0f}, {1.0f * textureScale, 1.0f * textureScale}},

    // Other faces
    {{ 0.0f, 3.3f,  0.0f}, {1.0f, 0.0f, 0.0f}, {0.5f * textureScale, 0.5f * textureScale}},
    {{ 0.3f, 3.0f,  0.3f}, {1.0f, 0.0f, 0.0f}, {1.0f * textureScale, 0.0f * textureScale}},
    {{-0.3f, 3.0f,  0.3f}, {1.0f, 0.0f, 0.0f}, {0.0f * textureScale, 0.0f * textureScale}},

    {{ 0.0f, 3.3f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f * textureScale, 1.0f * textureScale}},
    {{-0.3f, 3.0f,  0.3f}, {0.0f, 1.0f, 0.0f}, {1.0f * textureScale, 1.0f * textureScale}},
    {{-0.3f, 3.0f, -0.3f}, {0.0f, 1.0f, 0.0f}, {1.0f * textureScale, 0.0f * textureScale}},

    {{ 0.0f, 3.3f,  0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f * textureScale, 0.0f * textureScale}},
    {{-0.3f, 3.0f, -0.3f}, {0.0f, 0.0f, 1.0f}, {0.0f * textureScale, 0.0f * textureScale}},
    {{ 0.3f, 3.0f, -0.3f}, {0.0f, 0.0f, 1.0f}, {0.5f * textureScale, 0.5f * textureScale}},

    {{ 0.0f, 3.3f,  0.0f}, {1.0f, 1.0f, 0.0f}, {0.5f * textureScale, 0.5f * textureScale}},
    {{ 0.3f, 3.0f, -0.3f}, {1.0f, 1.0f, 0.0f}, {1.0f * textureScale, 0.0f * textureScale}},
    {{ 0.3f, 3.0f,  0.3f}, {1.0f, 1.0f, 0.0f}, {1.0f * textureScale, 1.0f * textureScale}}
};

#ifndef GLAD_GL_EXT_texture_filter_anisotropic
#define GLAD_GL_EXT_texture_filter_anisotropic 1
#endif

void InitTexture(GLuint& textureID, const char* filePath) {
    // Load the texture image using stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Enable anisotropic filtering if supported
        if (GLAD_GL_EXT_texture_filter_anisotropic) {
            GLfloat maxAniso = 0.5f;
            #ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
            #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
            #endif

            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
            #ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
            #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
            #endif

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
        }

        stbi_image_free(data);
    }
    else {
        std::cout << "Failed to load texture: " << filePath << std::endl;
    }
}

#include "C:\Users\BAH\Documents\TP SIM\ConsoleApplication1\ConsoleApplication1\libs/PerlinNoise.hpp" // Include Perlin noise header

void GeneratePlane(std::vector<STRVertex>& vertices, std::vector<unsigned int>& indices, int n, float d) {
    vertices.clear();
    indices.clear();
    heightMap.resize(n, std::vector<float>(n)); // Resize the heightMap

    // Perlin noise setup
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{ seed };

    // Noise scaling parameters
    float noiseScale = 0.05f;  // Controls frequency of noise
    float heightScale = 2.5f; // Controls amplitude of height variation

    // Generate vertices
    for (int z = 0; z < n; ++z) {
        for (int x = 0; x < n; ++x) {
            float xPos = (x - n / 2) * d;
            float zPos = (z - n / 2) * d;

            // More sophisticated noise generation
            float noise = perlin.octave2D_01(xPos * noiseScale, zPos * noiseScale, 4);
            float yPos = (noise - 0.5f) * 2.0f * heightScale;

            vertices.push_back({
                vec3(xPos, yPos, zPos),
                vec3(0.0f, 1.0f, 0.0f),
                vec2(textureScale * x / (float)(n - 1), textureScale * z / (float)(n - 1))
                });

            heightMap[z][x] = yPos; // Store the height
        }
    }

    // Index generation (unchanged)
    for (int z = 0; z < n - 1; ++z) {
        for (int x = 0; x < n - 1; ++x) {
            int topLeft = z * n + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * n + x;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

float GetHeightAtPosition(float x, float z) {
    int n = heightMap.size();
    float halfN = n / 2.0f;

    // Convert world coordinates to heightMap indices
    int ix = static_cast<int>((x / d) + halfN);
    int iz = static_cast<int>((z / d) + halfN);

    if (ix >= 0 && ix < n && iz >= 0 && iz < n) {
        return heightMap[iz][ix];
    }
    return 1.0f; // Default ground height if out of bounds
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

    // Calculate new front vector
    cameraFront.x = cos(radians(yaw)) * cos(radians(pitch));
    cameraFront.y = sin(radians(pitch));
    cameraFront.z = sin(radians(yaw)) * cos(radians(pitch));
    cameraFront = normalize(cameraFront);

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

#include <stack>

struct Cell {
    int x, z;
    bool visited;
    bool walls[4]; // left, right, top, bottom

    Cell(int x, int z) : x(x), z(z), visited(false) {
        walls[0] = walls[1] = walls[2] = walls[3] = true;
    }
};

std::vector<std::vector<Cell>> maze;
int mazeSize = 30; // Define the size of the maze

float goalX;
float goalZ;

void generateMaze(int size) {
    maze.clear();
    maze.resize(size, std::vector<Cell>(size, Cell(0, 0)));

    // Initialize maze cells with their positions
    for (int z = 0; z < size; ++z) {
        for (int x = 0; x < size; ++x) {
            maze[z][x] = Cell(x, z);
        }
    }

    std::stack<Cell*> stack;
    Cell* current = &maze[0][0];
    current->visited = true;

    while (true) {
        std::vector<Cell*> neighbors;

        // Find unvisited neighbors
        if (current->x > 0 && !maze[current->z][current->x - 1].visited) // left
            neighbors.push_back(&maze[current->z][current->x - 1]);
        if (current->x < size - 1 && !maze[current->z][current->x + 1].visited) // right
            neighbors.push_back(&maze[current->z][current->x + 1]);
        if (current->z > 0 && !maze[current->z - 1][current->x].visited) // top
            neighbors.push_back(&maze[current->z - 1][current->x]);
        if (current->z < size - 1 && !maze[current->z + 1][current->x].visited) // bottom
            neighbors.push_back(&maze[current->z + 1][current->x]);

        if (!neighbors.empty()) {
            // Pick a random unvisited neighbor
            Cell* next = neighbors[rand() % neighbors.size()];
            stack.push(current);

            // Remove walls between current and next cells
            if (next->x == current->x) {
                if (next->z > current->z) { // bottom neighbor
                    current->walls[2] = false;
                    next->walls[0] = false;
                } else { // top neighbor
                    current->walls[0] = false;
                    next->walls[2] = false;
                }
            } else if (next->z == current->z) {
                if (next->x > current->x) { // right neighbor
                    current->walls[1] = false;
                    next->walls[3] = false;
                } else { // left neighbor
                    current->walls[3] = false;
                    next->walls[1] = false;
                }
            }

            // Mark the next cell as visited and move
            next->visited = true;
            current = next;
        } else if (!stack.empty()) {
            // Backtrack to the previous cell
            current = stack.top();
            stack.pop();
        } else {
            // Break when the stack is empty (maze is complete)
            break;
        }
    }

    // Final step: Open the exit
    maze[size - 1][size - 1].walls[1] = false; // Remove right wall of the last cell
}

void createMazePlanes(std::vector<STRVertex>& vertices, std::vector<unsigned int>& indices, int mazeSize, float planeSize) {
    vertices.clear();
    indices.clear();
    unsigned int index = 0;

    float wallHeight = 3.0f;
    float halfPlaneSize = planeSize * 2 / 2.0f;

    for (int z = 0; z < mazeSize; ++z) {
        for (int x = 0; x < mazeSize; ++x) {
            Cell& cell = maze[z][x];

			cout << "Cell (" << x << ", " << z << "): " << cell.walls[0] << cell.walls[1] << cell.walls[2] << cell.walls[3] << endl;

            float xPos = (x - mazeSize / 2) * planeSize;
            float zPos = (z - mazeSize / 2) * planeSize;
            float yPos = GetHeightAtPosition(xPos, zPos) - 0.5f; // Fix y position using the plane as reference

            if (cell.walls[0]) { // top
                vertices.push_back({ vec3(xPos, yPos, zPos), vec3(1, 0, 0), vec2(0, 0) });
                vertices.push_back({ vec3(xPos + planeSize, yPos, zPos), vec3(1, 0, 0), vec2(1, 0) });
                vertices.push_back({ vec3(xPos + planeSize, yPos + wallHeight, zPos), vec3(1, 0, 0), vec2(1, 1) });
                vertices.push_back({ vec3(xPos, yPos + wallHeight, zPos), vec3(1, 0, 0), vec2(0, 1) });
                indices.push_back(index); indices.push_back(index + 1); indices.push_back(index + 2);
                indices.push_back(index); indices.push_back(index + 2); indices.push_back(index + 3);
                index += 4;
            }
            if (cell.walls[1]) { // right
                vertices.push_back({ vec3(xPos + planeSize, yPos, zPos), vec3(0, 1, 0), vec2(0, 0) });
                vertices.push_back({ vec3(xPos + planeSize, yPos, zPos + planeSize), vec3(0, 1, 0), vec2(1, 0) });
                vertices.push_back({ vec3(xPos + planeSize, yPos + wallHeight, zPos + planeSize), vec3(0, 1, 0), vec2(1, 1) });
                vertices.push_back({ vec3(xPos + planeSize, yPos + wallHeight, zPos), vec3(0, 1, 0), vec2(0, 1) });
                indices.push_back(index); indices.push_back(index + 1); indices.push_back(index + 2);
                indices.push_back(index); indices.push_back(index + 2); indices.push_back(index + 3);
                index += 4;
            }
            if (cell.walls[2]) { // bottom
                vertices.push_back({ vec3(xPos, yPos, zPos + planeSize), vec3(0, 0, 1), vec2(0, 0) });
                vertices.push_back({ vec3(xPos + planeSize, yPos, zPos + planeSize), vec3(0, 0, 1), vec2(1, 0) });
                vertices.push_back({ vec3(xPos + planeSize, yPos + wallHeight, zPos + planeSize), vec3(0, 0, 1), vec2(1, 1) });
                vertices.push_back({ vec3(xPos, yPos + wallHeight, zPos + planeSize), vec3(0, 0, 1), vec2(0, 1) });
                indices.push_back(index); indices.push_back(index + 1); indices.push_back(index + 2);
                indices.push_back(index); indices.push_back(index + 2); indices.push_back(index + 3);
                index += 4;
            }
            if (cell.walls[3]) { // left
                vertices.push_back({ vec3(xPos, yPos, zPos), vec3(1, 1, 0), vec2(0, 0) });
                vertices.push_back({ vec3(xPos, yPos, zPos + planeSize), vec3(1, 1, 0), vec2(1, 0) });
                vertices.push_back({ vec3(xPos, yPos + wallHeight, zPos + planeSize), vec3(1, 1, 0), vec2(1, 1) });
                vertices.push_back({ vec3(xPos, yPos + wallHeight, zPos), vec3(1, 1, 0), vec2(0, 1) });
                indices.push_back(index); indices.push_back(index + 1); indices.push_back(index + 2);
                indices.push_back(index); indices.push_back(index + 2); indices.push_back(index + 3);
                index += 4;
            }
        }
    }
    // Goal cell
    goalX = (mazeSize / 2) * planeSize;
    goalZ = (mazeSize / 2) * planeSize;

    cout << "Goal position: (" << goalX << ", " << goalZ << ")" << endl;
}

bool isCollidingWithWall(const vec3& newPosition, const std::vector<std::vector<Cell>>& maze, float planeSize) {
    float halfPlaneSize = planeSize / 2.0f;
    float wallThickness = 0.2f; // Adjust as needed for wall thickness

    for (const auto& row : maze) {
        for (const auto& cell : row) {
            float xPos = (cell.x - maze.size() / 2.0f) * planeSize;
            float zPos = (cell.z - maze.size() / 2.0f) * planeSize;

            // Check top wall
            if (cell.walls[0]) {
                if (newPosition.x >= xPos - wallThickness && newPosition.x <= xPos + planeSize + wallThickness &&
                    newPosition.z >= zPos - wallThickness && newPosition.z <= zPos + wallThickness) {
                    return true;
                }
            }
            // Check right wall
            if (cell.walls[1]) {
                if (newPosition.x >= xPos + planeSize - wallThickness && newPosition.x <= xPos + planeSize + wallThickness &&
                    newPosition.z >= zPos - wallThickness && newPosition.z <= zPos + planeSize + wallThickness) {
                    return true;
                }
            }
            // Check bottom wall
            if (cell.walls[2]) {
                if (newPosition.x >= xPos - wallThickness && newPosition.x <= xPos + planeSize + wallThickness &&
                    newPosition.z >= zPos + planeSize - wallThickness && newPosition.z <= zPos + planeSize + wallThickness) {
                    return true;
                }
            }
            // Check left wall
            if (cell.walls[3]) {
                if (newPosition.x >= xPos - wallThickness && newPosition.x <= xPos + wallThickness &&
                    newPosition.z >= zPos - wallThickness && newPosition.z <= zPos + planeSize + wallThickness) {
                    return true;
                }
            }
        }
    }
    return false;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentSpeed = cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        currentSpeed = cameraSpeed * 2.0f;

    vec3 newPos = cameraPos;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        newPos += currentSpeed * vec3(cameraFront.x, 0.0f, cameraFront.z);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        newPos -= currentSpeed * vec3(cameraFront.x, 0.0f, cameraFront.z);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        newPos -= normalize(cross(vec3(cameraFront.x, 0.0f, cameraFront.z), cameraUp)) * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        newPos += normalize(cross(vec3(cameraFront.x, 0.0f, cameraFront.z), cameraUp)) * currentSpeed;

    // Apply gravity
    newPos.y -= 0.1f; // Adjust gravity strength as needed

    // Check collision with ground
    float groundHeight = GetHeightAtPosition(newPos.x, newPos.z);
    if (newPos.y > groundHeight + 1.0f) { // Adding a small offset to keep the camera above the ground
        // Check collision with walls
        //if (!isCollidingWithWall(newPos, maze, d * 2)) {
            cameraPos = newPos;
		//}
    }
    else {
        cameraPos = vec3(newPos.x, groundHeight + 1.0f, newPos.z);
    }

    // Check if the player has reached the goal
    if (abs(cameraPos.x - goalX) <= 1.0f && abs(cameraPos.z - goalZ) <= 1.0f) {
        cout << "You win!" << endl;
    }


    // Jump when space is pressed
    static bool isJumping = false;
    static float jumpSpeed = 0.0f;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping) {
        isJumping = true;
        jumpSpeed = 0.25f; // Initial jump speed
    }

    if (isJumping) {
        cameraPos.y += jumpSpeed;
        jumpSpeed -= 0.001f; // Gravity effect

        if (cameraPos.y <= groundHeight + 1.0f) {
            cameraPos.y = groundHeight + 1.0f;
            isJumping = false;
            jumpSpeed = 0.0f;
        }
    }
}

std::vector<vec3> grassInstancePositions;

void generateGrassInstances(const std::vector<STRVertex>& planeVertices) {
   // for (const auto& vertex : planeVertices) {
        // Randomly place grass based on some condition, here we use a simple probability
       // if (rand() % 100 < 5) { // 10% chance to place grass
			vec3 pos = vec3(0,0,0);
			pos.y = GetHeightAtPosition(pos.x, pos.z) + 1.0; // Adjust y position based on the height map
            grassInstancePositions.push_back(pos);
        //}
    //}
}

GLuint grassVAO, grassVBO, grassUVBO, grassNormalBO;

void initGrassBuffers() {
    glGenVertexArrays(1, &grassVAO);
    glGenBuffers(1, &grassVBO);
    glGenBuffers(1, &grassUVBO);
    glGenBuffers(1, &grassNormalBO);

    glBindVertexArray(grassVAO);

    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, grassVertices.size() * sizeof(vec3), &grassVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, grassUVBO);
    glBufferData(GL_ARRAY_BUFFER, grassUVs.size() * sizeof(vec2), &grassUVs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, grassNormalBO);
    glBufferData(GL_ARRAY_BUFFER, grassNormals.size() * sizeof(vec3), &grassNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void renderGrassInstances(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    glBindVertexArray(grassVAO);

    for (const auto& pos : grassInstancePositions) {
        mat4 model = translate(mat4(1.0f), pos);
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, grassVertices.size());
    }

    glBindVertexArray(0);
}

GLuint pyramidVAO, pyramidVBO;

void initPyramidBuffers() {
    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);

    glBindVertexArray(pyramidVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, couleur));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, texture));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

std::vector<vec3> pyramidInstancePositions;

void generatePyramidInstances(const std::vector<STRVertex>& mazeVertices) {
    for (const auto& vertex : mazeVertices) {
        // Randomly place pyramid based on some condition, here we use a simple probability
        if (rand() % 100 < 5) { // 5% chance to place pyramid
            vec3 pos = vertex.position;
            pos.y = GetHeightAtPosition(pos.x, pos.z) + 1.0; // Adjust y position based on the height map
            pyramidInstancePositions.push_back(pos);
        }
    }
}

void renderPyramidInstances(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    glBindVertexArray(pyramidVAO);

    for (const auto& pos : pyramidInstancePositions) {
        mat4 model = translate(mat4(1.0f), pos);
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
    }

    glBindVertexArray(0);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	//initGrassBuffers();
    std::vector<STRVertex> planeVertices;
    std::vector<unsigned int> planeIndices;
    std::vector<STRVertex> mazeVertices;
    std::vector<unsigned int> mazeIndices;

    GeneratePlane(planeVertices, planeIndices, n, d);
    generateMaze(mazeSize);
	createMazePlanes(mazeVertices, mazeIndices, mazeSize, d*2);

    // Generate pyramid instances
    generatePyramidInstances(mazeVertices);

    // Initialize pyramid buffers
    initPyramidBuffers();

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(sizeof(GL_FLOAT) * 3));
    
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

	// Create VAO, VBO, and EBO for the plane
    GLuint planeVAO, planeVBO, planeEBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeEBO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(STRVertex), planeVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, planeIndices.size() * sizeof(unsigned int), planeIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, couleur));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, texture));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    GLuint mazeVAO, mazeVBO, mazeEBO;
    glGenVertexArrays(1, &mazeVAO);
    glGenBuffers(1, &mazeVBO);
    glGenBuffers(1, &mazeEBO);

    glBindVertexArray(mazeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mazeVBO);
    glBufferData(GL_ARRAY_BUFFER, mazeVertices.size() * sizeof(STRVertex), mazeVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mazeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mazeIndices.size() * sizeof(unsigned int), mazeIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, couleur));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, texture));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    GLuint ShaderProgram = LoadShaders("shader/SimpleVertexShader2.vertexshader", "shader/SimpleFragmentShader2.fragmentshader");
    GLuint skyboxShader = LoadShaders("shader/skybox.vertexshader", "shader/skybox.fragmentshader");

    GLuint TextureID;
    GLuint text;

    GLuint textureID;
    const char* wallTexturePath = "textures/Wall.jpg";
    InitTexture(textureID, wallTexturePath);

    const char* grassTexturePath = "textures/Grass.jpg";
    InitTexture(text, grassTexturePath);

    TextureID = glGetUniformLocation(ShaderProgram, "ourTexture");

    // Set shader uniforms
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
    glm::vec3 viewPos = cameraPos; // Assuming cameraPos is the position of your camera
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);
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
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Cycles through all the textures and attaches them to the cubemap object
    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            //stbi_set_flip_vertically_on_load(false);
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
		processInput(window);
        View = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        mat4 MVP = Projection * View * Model;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

        glUseProgram(ShaderProgram);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(ShaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(ShaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
        glUniform3fv(glGetUniformLocation(ShaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(ShaderProgram, "ambientColor"), 1, glm::value_ptr(ambientColor));

        // Render the maze
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, textureID); // Bind the wall texture
        glBindVertexArray(mazeVAO);
        glDrawElements(GL_TRIANGLES, mazeIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //// Render the plane
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, text); // Bind the grass texture
        glBindVertexArray(planeVAO);
        glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Render pyramid instances
        renderPyramidInstances(ShaderProgram);

        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShader);

        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
        glm::mat4 skyboxView = glm::mat4(glm::mat3(View)); 
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
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



