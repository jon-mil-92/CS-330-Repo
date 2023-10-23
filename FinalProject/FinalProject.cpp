#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"      // Image loading Utility functions
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#include <iostream>         // Console output
#include <cstdlib>          // Exit status
#include <vector>           // Mesh building

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Mesh builder inclusions
#include "CylinderMeshBuilder.h"
#include "SphereMeshBuilder.h"
#include "CuboidMeshBuilder.h"
#include "PlaneMeshBuilder.h"

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// The following variables can only be used in this file
namespace
{
    const char* const WINDOW_TITLE = "Jonathan Miller - Final Project"; // Macro for window title

    // Program window parameters
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

    // Camera variables
    glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 0.0f); // Perspective camera position
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraPosOrtho = glm::vec3(0.0f, -3.0f, 0.0f); // Orthographic camera position
    bool perspective = true; // Start the view with a perspective projection
    bool projectionKeyDown = false; // The key to toggle the projection type is not currently held down
    float orthoTop = (float)WINDOW_HEIGHT / 100.0f;
    float orthoRight = orthoTop * ASPECT_RATIO;

    // Delta time variables
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    // Mouse variables
    float mouseLastX = WINDOW_WIDTH / 2.0f, mouseLastY = WINDOW_HEIGHT / 2.0f; // Mouse originally at the center of the screen
    bool firstMouse = true; // First time receiving mouse input
    float pitch = 0.0f, yaw = -90.0f; // Initial camera orientation
    float moveSpeed = 2.5f; // Initial camera move speed

    // Battery mesh parameters
    const float BATTERY_CASE_RADIUS = 0.1f;
    const float BATTERY_CASE_HEIGHT = 0.7f;
    const float BATTERY_TERMINAL_RADIUS = 0.035f;
    const float BATTERY_TERMINAL_HEIGHT = 0.02f;
    const float BATTERY_SPECULAR_INTENSITY = 0.7f;
    const int CYLINDER_SLICES = 60; // The roundness of the cylinder

    // Amp mesh parameters
    const float AMP_WIDTH = 0.33;
    const float AMP_HEIGHT = 0.08;
    const float AMP_LENGTH = 0.66;
    const float VOLUME_KNOB_RADIUS = 0.04f;
    const float VOLUME_KNOB_HEIGHT = 0.025f;
    const float AMP_SPECULAR_INTENSITY = 0.65;

    // Marble mesh parameters
    const int SPHERE_SEGMENTS = 64;
    const float MARBLE_SPECULAR_INTENSITY = 0.9f;

    // Phone box mesh parameters
    const float PHONE_BOX_WIDTH = 0.28;
    const float PHONE_BOX_HEIGHT = 0.19;
    const float PHONE_BOX_LENGTH = 0.52;
    const float PHONE_BOX_SPECULAR_INTENSITY = 0.4f;

    // Table mesh parameters
    const float TABLE_LENGTH = 1.0f;
    const float TABLE_WIDTH = 1.0f;
    const float TABLE_SPECULAR_INTENSITY = 0.25;

    // Window light mesh parameter
    const float WINDOW_MESH_LENGTH = 1.0f;
    const float WINDOW_MESH_WIDTH = 1.0f;
    const float WINDOW_MESH_SCALE = 20.0f;

    // Back window variables
    glm::vec3 gLightPosBack(0.0f, 15.0f, -50.0f); // Back window
    glm::vec3 gLightColorBack(0.95f, 0.90f, 0.80f); // Bright sunlight color
    float gLightIntenBack = 0.9f; // 90 percent light intensity

    // Left window variables
    glm::vec3 gLightPosLeft(-40.0f, 15.0f, -5.0f); // Left window
    glm::vec3 gLightColorLeft(0.95f, 0.90f, 0.80f); // Bright sunlight color
    float gLightIntenLeft = 0.9f; // 90 percent light intensity

    // Right window variables
    glm::vec3 gLightPosRight(40.0f, 15.0f, -5.0f); // Right window
    glm::vec3 gLightColorRight(0.95f, 0.90f, 0.80f); // Bright sunlight color
    float gLightIntenRight = 0.9f; // 90 percent light intensity

    // Lighting variables
    float gAmbientLightStrength = 0.12f; // Brighten unlit areas
    float gSpecularHighlightSize = WINDOW_MESH_SCALE;
    float gSpecularIntensity;

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;

    // No decal texture identifier
    GLuint gNoDecal = -1;

    // Shader programs
    GLuint gObjectProgramId;
    GLuint gLampProgramId;

    // Mesh builders
    CylinderMeshBuilder cylinderMeshBuilder;
    SphereMeshBuilder sphereMeshBuilder;
    CuboidMeshBuilder cuboidMeshBuilder;
    PlaneMeshBuilder planeMeshBuilder;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        bool enabled = true;
        GLuint vao;         // Handle for the vertex array object
        GLuint vbo;         // Handle for the vertex buffer object
        GLuint nVertices;   // Number of vertices of the mesh
    };

    // Stores the GL data relative to a given mesh
    struct GLMeshIndexed
    {
        bool enabled = true;
        GLuint vao;         // Handle for the vertex array object
        GLuint vbos[2];     // Handles for the vertex buffer objects
        GLuint nIndices;    // Number of indices of the mesh
    };

    // Placeholder meshes
    // ------------------

    GLMesh gMesh;
    GLMeshIndexed gMeshIndexed;

    // Battery Meshes
    // --------------

    // Triangle mesh data for the battery
    GLMesh gMeshBatteryCaseSide;
    GLMesh gMeshBatteryCaseTop;
    GLMesh gMeshBatteryCaseBottom;
    GLMesh gMeshBatteryTerminalSide;
    GLMesh gMeshBatteryTerminalTop;

    // Texture IDs for the battery
    GLuint gTextureBatteryCaseSide;
    GLuint gTextureBatteryCaseSideDecal;
    GLuint gTextureBatteryCaseTop;
    GLuint gTextureBatteryCaseBottom;
    GLuint gTextureBatteryTerminalSide;
    GLuint gTextureBatteryTerminalTop;

    // Texture scales for the battery
    glm::vec2 gUVScaleBatteryCaseSide(1.0f, 1.0f);
    glm::vec2 gUVScaleBatteryCaseTop(1.0f, 1.0f);
    glm::vec2 gUVScaleBatteryCaseBottom(1.0f, 1.0f);
    glm::vec2 gUVScaleBatteryTerminalSide(1.0f, 1.0f);
    glm::vec2 gUVScaleBatteryTerminalTop(1.0f, 1.0f);

    // Amp Meshes
    // --------------

    // Triangle mesh data for the amp
    GLMesh gMeshAmp;
    GLMesh gMeshAmpSide;
    GLMesh gMeshAmpSideBack;
    GLMesh gMeshAmpSideFront;
    GLMesh gMeshVolumeKnobSide;
    GLMesh gMeshVolumeKnobFront;

    // Texture IDs for the amp
    GLuint gTextureAmp;
    GLuint gTextureAmpSide;
    GLuint gTextureAmpSideFace;
    GLuint gTextureVolumeKnobSide;
    GLuint gTextureVolumeKnobFront;

    // Texture scales for the amp
    glm::vec2 gUVScaleAmp(1.0f, 1.0f);
    glm::vec2 gUVScaleAmpSide(1.0f, 1.0f);
    glm::vec2 gUVScaleVolumeKnobSide(5.0f, 1.0f); // Texture tiling 5x1
    glm::vec2 gUVScaleVolumeKnobFront(1.0f, 1.0f);

    // Marble Mesh
    // -----------

    GLMeshIndexed gMeshMarble; // Triangle mesh data for the marble
    GLuint gTextureMarble; // Texture ID for the marble
    glm::vec2 gUVScaleMarble(1.0f, 1.0f); // Texture scale for the marble

    // Phone Box Mesh
    // ----------

    GLMesh gMeshPhoneBox; // Triangle mesh data for the phone box
    GLuint gTexturePhoneBox; // Texture ID for the phone box
    glm::vec2 gUVScalePhoneBox(1.0f, 1.0f); // Texture scale for the phone box

    // Table Mesh
    // ----------

    GLMesh gMeshTable; // Triangle mesh data for the table surface
    GLuint gTextureTable; // Texture ID for the table surface
    glm::vec2 gUVScaleTable(2.0f, 2.0f); // Texture scale for the table surface - Texture tiling 2x2

    // Window Light Mesh
    // -----------------

    GLMesh gMeshWindow; // Triangle mesh data for the windows
}

// ------------------------------------------------------------------------------------------------------------------------
// User-defined function prototypes
// ------------------------------------------------------------------------------------------------------------------------

// Input processing functions
// --------------------------
void UProcessInput(GLFWwindow* window);
void UMouseOffWindow(GLFWwindow* window);
void UMouseCallback(GLFWwindow* window, double xpos, double ypos);
void UScrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset);

// Mesh creation functions
// -----------------------
void UCreateMesh(GLMesh& gMesh, GLMeshIndexed& gMeshIndexed, vector<GLfloat>& vertices, vector<GLushort>& indices);
void UCreateBatteryMeshes();
void UCreateAmpMeshes();
void UCreateCylinderSideMesh(GLMesh& gMesh, int slices, float height, float radius);
void UCreateCylinderFaceMesh(GLMesh& gMesh, bool isTopFace, int slices, float radius);
void UCreateSphereMesh(GLMeshIndexed& gMeshIndexed, unsigned int segments);
void UCreateCuboidMesh(GLMesh& gMesh, float width, float height, float length);
void UCreatePlaneMesh(GLMesh& gMesh, float length, float width);

// Draw functions
// --------------
void URender();
void UDrawObjectMesh(GLMesh& gMesh, GLMeshIndexed& gMeshIndexed, GLuint& gTexture, GLuint& gTextureDecal, glm::vec2& gUVScale,
    float posX, float posY, float posZ, float rotAngle, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ);
void UDrawLightMesh(GLMesh& gMesh, glm::vec3 lightPos, glm::vec3 lightColor, float lightIntensity,
    float rotAngle, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ);
void UDrawBattery(float x, float y, float z);
void UDrawAmp(float x, float y, float z);
void UDrawMarble(float x, float y, float z);
void UDrawPhoneBox(float x, float y, float z);
void UDrawTable(float x, float y, float z);

// Setup functions
// ---------------
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
int ULoadTextures();
bool UCreateTexture(const char* filename, GLuint& gTexture, int textureWrapType);

// Destruction functions
// ---------------------
void UDestroyShaderProgram(GLuint programId);
void UDestroyMesh(GLMesh& gMesh, GLMeshIndexed& gMeshIndexed);
void UDestroyTexture(GLuint gTexture);

// ------------------------------------------------------------------------------------------------------------------------
// Shader programs
// ------------------------------------------------------------------------------------------------------------------------

/* Lamp Shader Source Code*/
const GLchar* lampVertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

    // Global variables for the transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
    }
);

/* Lamp Fragment Shader Source Code*/
const GLchar* lampFragmentShaderSource = GLSL(440,
    out vec4 fragmentColor; // For outgoing lamp color to the GPU

    uniform vec3 lightColor; // Make the lamp have the same color as the light

    void main()
    {
        fragmentColor = vec4(lightColor, 1.0); // Set color to the light color with alpha 1.0
    }
);

/* Object Shader Source Code*/
const GLchar* objectVertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
    layout(location = 1) in vec3 normal; // VAP position 1 for normals
    layout(location = 2) in vec2 textureCoordinate;

    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
    out vec2 vertexTextureCoordinate;

    // Global variables for the transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

        vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

        vertexNormal = mat3(transpose(inverse(model))) * normal; // Get normal vectors in world space only and exclude normal translation properties
        vertexTextureCoordinate = textureCoordinate;
    }
);

/* Object Fragment Shader Source Code*/
const GLchar* objectFragmentShaderSource = GLSL(440,
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position
    in vec2 vertexTextureCoordinate; // For incoming texture coordinates

    out vec4 fragmentColor; // For outgoing object color to the GPU

    // Camera position
    uniform vec3 viewPosition;

    // Texture variables
    uniform sampler2D uTexture;
    uniform sampler2D uTextureDecal;
    uniform vec2 uvScale;
    uniform bool useDecal;

    // Back window variables
    uniform vec3 lightPosBack;
    uniform vec3 lightColorBack;
    uniform float lightIntenBack;

    // Left window variables
    uniform vec3 lightPosLeft;
    uniform vec3 lightColorLeft;
    uniform float lightIntenLeft;

    // Right window variables
    uniform vec3 lightPosRight;
    uniform vec3 lightColorRight;
    uniform float lightIntenRight;

    // Lighting variables
    uniform float ambStren;
    uniform float specInten;
    uniform float specSize;

    // Point light function prototype
    vec3 CalcPointLight(vec3 lightPos, vec3 lightColor, float intensity);

    void main()
    {
        // Calculate overall lighting from the three windows
        vec3 result = CalcPointLight(lightPosBack, lightColorBack, lightIntenBack) +
            CalcPointLight(lightPosLeft, lightColorLeft, lightIntenLeft) +
            CalcPointLight(lightPosRight, lightColorRight, lightIntenRight);

        fragmentColor = vec4(result, 1.0); // Send lighting results to GPU
    }

    // Calculates the color when using a point light
    vec3 CalcPointLight(vec3 lightPos, vec3 lightColor, float intensity)
    {
        /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

        // Calculate Ambient lighting
        float ambientStrength = ambStren; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // Calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // Calculate Specular lighting
        float specularIntensity = specInten; // Set specular light strength
        float highlightSize = specSize; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm); // Calculate reflection vector

        // Calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // Texture holds the color to be used for all three components
        vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

        if (useDecal)
        {
            vec4 decalTextureColor = texture(uTextureDecal, vertexTextureCoordinate * uvScale);

            if (decalTextureColor.a > 0.4)
                textureColor = decalTextureColor;
        }

        // Apply light intensity to the phong model
        ambient *= intensity;
        diffuse *= intensity;
        specular *= intensity;

        // Calculate and return the phong result
        return ((ambient + diffuse + specular) * textureColor.xyz);
    }
);

// ------------------------------------------------------------------------------------------------------------------------
// Main program function
// ------------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Create the application window
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the object shader program
    if (!UCreateShaderProgram(objectVertexShaderSource, objectFragmentShaderSource, gObjectProgramId))
        return EXIT_FAILURE;

    // Create the lamp shader program
    if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, gLampProgramId))
        return EXIT_FAILURE;

    // Load all textures from their corresponding files
    int exitStatus = ULoadTextures();

    // If one of the textures does not load, exit the program with exit status 1
    if (exitStatus == EXIT_FAILURE)
        return EXIT_FAILURE;

    // Tell OpenGL for each sampler which texture unit it belongs to
    glUseProgram(gObjectProgramId);

    // Set the main texture as texture unit 0
    glUniform1i(glGetUniformLocation(gObjectProgramId, "uTexture"), 0);

    // Set the decal texture as texture unit 1
    glUniform1i(glGetUniformLocation(gObjectProgramId, "uTextureDecal"), 1);

    // Disable placeholder meshes for the draw object function calls
    gMesh.enabled = false;
    gMeshIndexed.enabled = false;

    // Create object meshes
    UCreateBatteryMeshes();
    UCreateAmpMeshes();
    UCreateSphereMesh(gMeshMarble, SPHERE_SEGMENTS);
    UCreateCuboidMesh(gMeshPhoneBox, PHONE_BOX_WIDTH, PHONE_BOX_HEIGHT, PHONE_BOX_LENGTH);
    UCreatePlaneMesh(gMeshTable, TABLE_LENGTH, TABLE_WIDTH);
    UCreatePlaneMesh(gMeshWindow, WINDOW_MESH_LENGTH, WINDOW_MESH_WIDTH);

    // Render loop
    while (!glfwWindowShouldClose(gWindow))
    {
        // Process keyboard events
        UProcessInput(gWindow);

        // Reset firstMouse if the mouse cursor leaves the content area of the window
        UMouseOffWindow(gWindow);

        // Render this frame
        URender();

        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(gMeshBatteryCaseSide, gMeshIndexed);
    UDestroyMesh(gMeshBatteryCaseTop, gMeshIndexed);
    UDestroyMesh(gMeshBatteryCaseBottom, gMeshIndexed);
    UDestroyMesh(gMeshBatteryTerminalSide, gMeshIndexed);
    UDestroyMesh(gMeshBatteryTerminalTop, gMeshIndexed);
    UDestroyMesh(gMeshAmp, gMeshIndexed);
    UDestroyMesh(gMeshAmpSide, gMeshIndexed);
    UDestroyMesh(gMeshAmpSideBack, gMeshIndexed);
    UDestroyMesh(gMeshAmpSideFront, gMeshIndexed);
    UDestroyMesh(gMesh, gMeshMarble);
    UDestroyMesh(gMeshPhoneBox, gMeshIndexed);
    UDestroyMesh(gMeshTable, gMeshIndexed);

    // Release texture data
    UDestroyTexture(gTextureBatteryCaseSide);
    UDestroyTexture(gTextureBatteryCaseSideDecal);
    UDestroyTexture(gTextureBatteryCaseTop);
    UDestroyTexture(gTextureBatteryCaseBottom);
    UDestroyTexture(gTextureBatteryTerminalSide);
    UDestroyTexture(gTextureBatteryTerminalTop);
    UDestroyTexture(gTextureAmp);
    UDestroyTexture(gTextureAmpSide);
    UDestroyTexture(gTextureAmpSideFace);
    UDestroyTexture(gTextureMarble);
    UDestroyTexture(gTexturePhoneBox);
    UDestroyTexture(gTextureTable);

    // Release shader programs
    UDestroyShaderProgram(gObjectProgramId);
    UDestroyShaderProgram(gLampProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// ------------------------------------------------------------------------------------------------------------------------
// Input processing functions
// ------------------------------------------------------------------------------------------------------------------------

// Process keyboard input
void UProcessInput(GLFWwindow* window)
{
    // Updata delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Enable a consistent camera speed with delta time.
    float cameraSpeed = moveSpeed * deltaTime;

    // Move camera forward while using perspective projection; zoom camera in while using orthographic projection
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (perspective)
            cameraPos += cameraSpeed * cameraFront;
        else
        {
            if (orthoTop - cameraSpeed > 0) // Don't flip the image when zooming in all the way while using orthographic projection
                orthoTop -= cameraSpeed, orthoRight -= cameraSpeed * ASPECT_RATIO; // Maintain aspect ratio while zooming in
        }
    }

    // Move camera backward while using perspective projection; zoom camera out while using orthographic perspective
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        if (perspective)
            cameraPos -= cameraSpeed * cameraFront;
        else
            orthoTop += cameraSpeed, orthoRight += cameraSpeed * ASPECT_RATIO; // Maintain aspect ratio while zooming out
    }

    // Move camera left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (perspective)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        else
            cameraPosOrtho.x += cameraSpeed;
    }

    // Move camera right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (perspective)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        else
            cameraPosOrtho.x -= cameraSpeed;
    }

    // Move camera down
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (perspective)
            cameraPos -= cameraUp * cameraSpeed;
        else
            cameraPosOrtho.y += cameraSpeed;
    }

    // Move camera up
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        if (perspective)
            cameraPos += cameraUp * cameraSpeed;
        else
            cameraPosOrtho.y -= cameraSpeed;
    }

    // Toggle the projection type
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        // Only change the projection type once per key press
        if (!projectionKeyDown)
        {
            perspective = !perspective;
            firstMouse = true; // Avoid sudden camera jumps after re-entering the 3D perspective
            projectionKeyDown = true;
        }
    }

    // Once the key to change the projection type is released, it is no longer held down
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
        projectionKeyDown = false;

    // Close the OpenGL window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Reset the firstMouse variable if the mouse cursor leaves the content area of the window
void UMouseOffWindow(GLFWwindow* window) {
    // Get mouse postition relative to the content area of the window
    double mousePosX;
    double mousePosY;
    glfwGetCursorPos(window, &mousePosX, &mousePosY);

    // Reset firstMouse if the cursor leaves the content area of the window
    if (mousePosX < 0.0f || mousePosX >(float)WINDOW_WIDTH || mousePosY < 0 || mousePosY >(float)WINDOW_HEIGHT)
    {
        firstMouse = true;
    }
}

// GLFW callback for mouse movement events
void UMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Only process mouse events while using the 3D perspective projection
    if (perspective)
    {
        // Avoid sudden camera jumps
        if (firstMouse)
        {
            mouseLastX = xpos;
            mouseLastY = ypos;
            firstMouse = false;
        }

        // Calculate the X and Y offset values for the mouse position
        float xoffset = xpos - mouseLastX;
        float yoffset = mouseLastY - ypos;

        // Update the previous mouse position
        mouseLastX = xpos;
        mouseLastY = ypos;

        // Apply the look sensitivy multiplier to the mouse position offsets
        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // Update the yaw and pitch based on the mouse position offsets
        yaw += xoffset;
        pitch += yoffset;

        // Constrain the camera pitch to prevent LookAt flip
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Update where the camera is looking based on the new direction
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }
}

// GLFW callback for scroll wheel events
void UScrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Adjust the camera movement speed if the offset would not make it negative
    if (moveSpeed + (float)yoffset > 0.0f)
        moveSpeed += (float)yoffset;
    else
        moveSpeed = 0; // Do not allow a negative movement speed
}

// ------------------------------------------------------------------------------------------------------------------------
// Mesh creation functions
// ------------------------------------------------------------------------------------------------------------------------

// Create a mesh with the given vertices and indices
void UCreateMesh(GLMesh& gMesh, GLMeshIndexed& gMeshIndexed, vector<GLfloat>& vertices, vector<GLushort>& indices)
{
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    // Initialize the number of mesh vertices
    gMesh.nVertices = vertices.size();

    // Initialize the number of mesh indices
    gMeshIndexed.nIndices = indices.size();

    if (gMesh.enabled == true)
    {
        // Create and activate the vertex array object
        glGenVertexArrays(1, &gMesh.vao);
        glBindVertexArray(gMesh.vao);

        // Create, activate, and send buffer for the vertex data
        glGenBuffers(1, &gMesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    }
    else if (gMeshIndexed.enabled == true)
    {
        // Create and activate the vertex array object
        glGenVertexArrays(1, &gMeshIndexed.vao);
        glBindVertexArray(gMeshIndexed.vao);

        // Create, activate, and send buffers for the vertex data and indices
        glGenBuffers(2, gMeshIndexed.vbos);
        glBindBuffer(GL_ARRAY_BUFFER, gMeshIndexed.vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gMeshIndexed.vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
    }

    // Stride between vertex coordinates is 8 (X, Y, Z, nX, nY, nZ, tX, tY)
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // Create vertex attribute pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex)); // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal))); // Texture Coordinate
    glEnableVertexAttribArray(2);
}

// Create the meshes for all parts of a battery
void UCreateBatteryMeshes()
{
    // Create the mesh for the side of the battery case
    UCreateCylinderSideMesh(gMeshBatteryCaseSide, CYLINDER_SLICES, BATTERY_CASE_RADIUS, BATTERY_CASE_HEIGHT);

    // Create the mesh for the top of the battery case
    UCreateCylinderFaceMesh(gMeshBatteryCaseTop, true, CYLINDER_SLICES, BATTERY_CASE_RADIUS);

    // Create the mesh for the bottom of the battery case
    UCreateCylinderFaceMesh(gMeshBatteryCaseBottom, false, CYLINDER_SLICES, BATTERY_CASE_RADIUS);

    // Create the mesh for the side of the battery terminal
    UCreateCylinderSideMesh(gMeshBatteryTerminalSide, CYLINDER_SLICES, BATTERY_TERMINAL_RADIUS, BATTERY_TERMINAL_HEIGHT);

    // Create the mesh for the top of the battery terminal
    UCreateCylinderFaceMesh(gMeshBatteryTerminalTop, true, CYLINDER_SLICES, BATTERY_TERMINAL_RADIUS);
}

// Create the meshes for all parts of the amp
void UCreateAmpMeshes()
{
    // Create the mesh for the body of the amp
    UCreateCuboidMesh(gMeshAmp, AMP_WIDTH, AMP_HEIGHT, AMP_LENGTH);

    // Create the mesh for the sides of the amp body for rounded sides
    UCreateCylinderSideMesh(gMeshAmpSide, CYLINDER_SLICES, VOLUME_KNOB_RADIUS, AMP_LENGTH);

    // Create the mesh for the front face of the rounded side of the amp
    UCreateCylinderFaceMesh(gMeshAmpSideFront, true, CYLINDER_SLICES, VOLUME_KNOB_RADIUS);

    // Create the mesh for the back face of the rounded side of the amp
    UCreateCylinderFaceMesh(gMeshAmpSideBack, false, CYLINDER_SLICES, VOLUME_KNOB_RADIUS);

    // Create the mesh for the side of the volume knob for the amp
    UCreateCylinderSideMesh(gMeshVolumeKnobSide, CYLINDER_SLICES, VOLUME_KNOB_RADIUS, VOLUME_KNOB_HEIGHT);

    // Create the mesh for the front of the volume knob for the amp
    UCreateCylinderFaceMesh(gMeshVolumeKnobFront, true, CYLINDER_SLICES, VOLUME_KNOB_RADIUS);
}

// Create a mesh for a cylinder side
void UCreateCylinderSideMesh(GLMesh& gMesh, int slices, float radius, float height)
{
    // Create a vector to hold the vertices for the cylinder side mesh
    vector<GLfloat> vertices;
    vector<GLushort> indices; // Not used for a cylinder mesh

    // Build the vertices vector for the cylinder side mesh
    cylinderMeshBuilder.buildSideMesh(vertices, slices, radius, height);

    // Create the cylinder side mesh
    UCreateMesh(gMesh, gMeshIndexed, vertices, indices);
}

// Create a mesh for a cylinder face
void UCreateCylinderFaceMesh(GLMesh& gMesh, bool isTopFace, int slices, float radius)
{
    // Create a vector to hold the vertices for the cylinder face mesh
    vector<GLfloat> vertices;
    vector<GLushort> indices; // Not used for a cylinder mesh

    // Build the vertices vector for the cylinder face mesh
    cylinderMeshBuilder.buildFaceMesh(vertices, isTopFace, slices, radius);

    // Create the cylinder face mesh
    UCreateMesh(gMesh, gMeshIndexed, vertices, indices);
}

// Create a mesh for a sphere
void UCreateSphereMesh(GLMeshIndexed& gMeshIndexed, unsigned int segments)
{
    // Create vectors to hold the vertices and indices for the sphere mesh
    vector<GLfloat> vertices;
    vector<GLushort> indices;

    // Build the vertices and indices vectors for the sphere mesh
    sphereMeshBuilder.buildMesh(vertices, indices, segments);

    // Create the sphere mesh
    UCreateMesh(gMesh, gMeshIndexed, vertices, indices);
}

// Create a mesh for a cuboid
void UCreateCuboidMesh(GLMesh& gMesh, float width, float height, float length)
{
    // Create a vector to hold the vertices for the cuboid mesh
    vector<GLfloat> vertices;
    vector<GLushort> indices; // Not used for a cuboid mesh

    // Build the vertices vector for the cuboid mesh
    cuboidMeshBuilder.buildMesh(vertices, width, height, length);

    // Create the cuboid mesh
    UCreateMesh(gMesh, gMeshIndexed, vertices, indices);
}

// Create a mesh for a plane
void UCreatePlaneMesh(GLMesh& gMesh, float length, float width)
{
    // Create a vector to hold the vertices for the plane mesh
    vector<GLfloat> vertices;
    vector<GLushort> indices; // Not used for a plane mesh

    // Build the vertices vector for the plane mesh
    planeMeshBuilder.buildMesh(vertices, length, width);

    // Create the plane mesh
    UCreateMesh(gMesh, gMeshIndexed, vertices, indices);
}

// ------------------------------------------------------------------------------------------------------------------------
// Draw functions
// ------------------------------------------------------------------------------------------------------------------------

// Render a frame with all of the objects
void URender()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Enable alpha blending for decal textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // Clear the frame and z buffers
    glClearColor(0.20f, 0.50f, 0.64f, 1.0f); // Dark blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the objects
    UDrawBattery(3.0f, 0.0f, -11.5f);
    UDrawBattery(4.0f, 0.0f, -11.5f);
    UDrawAmp(0.3f, 0.0f, -8.5f);
    UDrawMarble(1.1f, 0.0f, -7.5f);
    UDrawPhoneBox(-4.0f, 0.0f, -7.0f);
    UDrawTable(0.0f, -0.0001f, -10.0f);

    // Draw the light source meshes
    UDrawLightMesh(gMeshWindow, gLightPosBack, gLightColorBack, gLightIntenBack,
        90.0f, 1.0f, 0.0f, 0.0f, WINDOW_MESH_SCALE, WINDOW_MESH_SCALE, WINDOW_MESH_SCALE); // Back window

    UDrawLightMesh(gMeshWindow, gLightPosLeft, gLightColorLeft, gLightIntenLeft,
        90.0f, 0.0f, 0.0f, 1.0f, WINDOW_MESH_SCALE, WINDOW_MESH_SCALE, WINDOW_MESH_SCALE); // Left window

    UDrawLightMesh(gMeshWindow, gLightPosRight, gLightColorRight, gLightIntenRight,
        90.0f, 0.0f, 0.0f, 1.0f, WINDOW_MESH_SCALE, WINDOW_MESH_SCALE, WINDOW_MESH_SCALE); // Right window

    // Swap buffers and poll IO events
    glfwSwapBuffers(gWindow);
}

// Draw a mesh with the given textures, texture scale, coordinates, rotation angle, axis rotation scalars, and size scalars
void UDrawObjectMesh(GLMesh& gMesh, GLMeshIndexed& gMeshIndexed, GLuint& gTexture, GLuint& gTextureDecal, glm::vec2& gUVScale,
    float posX, float posY, float posZ, float rotAngle, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ)
{
    // Set the shader to be used
    glUseProgram(gObjectProgramId);

    // Place object at the given coordinates
    glm::mat4 translation = glm::translate(glm::vec3(posX, posY, posZ));

    // Rotates the mesh by a given number of degrees along the specified axes
    glm::mat4 rotation = glm::rotate(glm::radians(rotAngle), glm::vec3(rotX, rotY, rotZ));

    // Scales the object by a given matrix scalar
    glm::mat4 scale = glm::scale(glm::vec3(scaleX, scaleY, scaleZ));

    // Model matrix transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Define the view matrix
    glm::mat4 view;

    // Define the projection matrix
    glm::mat4 projection;

    if (perspective) // If the user is in 3D mode
    {
        // Move the camera in 3D space
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Creates a perspective projection
        projection = glm::perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    else // If the user is in 2D mode
    {
        // Move the camera in 2D space
        view = glm::translate(glm::vec3(cameraPosOrtho.x, cameraPosOrtho.y, 0.0f));

        // Creates an orthogonal projection
        projection = glm::ortho(-orthoRight, orthoRight, -orthoTop, orthoTop, 0.1f, 100.0f);
    }

    // Set transform matrix uniforms
    GLint modelLoc = glGetUniformLocation(gObjectProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gObjectProgramId, "view");
    GLint projLoc = glGetUniformLocation(gObjectProgramId, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set camera position uniform
    GLint viewPositionLoc = glGetUniformLocation(gObjectProgramId, "viewPosition");
    glUniform3f(viewPositionLoc, cameraPos.x, cameraPos.y, cameraPos.z);

    // Set texture scale uniform
    GLint UVScaleLoc = glGetUniformLocation(gObjectProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // Set back window uniforms
    GLint lightPosBackLoc = glGetUniformLocation(gObjectProgramId, "lightPosBack");
    GLint lightColorBackLoc = glGetUniformLocation(gObjectProgramId, "lightColorBack");
    GLint lightIntenBackLoc = glGetUniformLocation(gObjectProgramId, "lightIntenBack");
    glUniform3f(lightPosBackLoc, gLightPosBack.x, gLightPosBack.y, gLightPosBack.z);
    glUniform3f(lightColorBackLoc, gLightColorBack.r, gLightColorBack.g, gLightColorBack.b);
    glUniform1f(lightIntenBackLoc, gLightIntenBack);

    // Set left window uniforms
    GLint lightPosLeftLoc = glGetUniformLocation(gObjectProgramId, "lightPosLeft");
    GLint lightColorLeftLoc = glGetUniformLocation(gObjectProgramId, "lightColorLeft");
    GLint lightIntenLeftLoc = glGetUniformLocation(gObjectProgramId, "lightIntenLeft");
    glUniform3f(lightPosLeftLoc, gLightPosLeft.x, gLightPosLeft.y, gLightPosLeft.z);
    glUniform3f(lightColorLeftLoc, gLightColorLeft.r, gLightColorLeft.g, gLightColorLeft.b);
    glUniform1f(lightIntenLeftLoc, gLightIntenLeft);

    // Set right window uniforms
    GLint lightPosRightLoc = glGetUniformLocation(gObjectProgramId, "lightPosRight");
    GLint lightColorRightLoc = glGetUniformLocation(gObjectProgramId, "lightColorRight");
    GLint lightIntenRightLoc = glGetUniformLocation(gObjectProgramId, "lightIntenRight");
    glUniform3f(lightPosRightLoc, gLightPosRight.x, gLightPosRight.y, gLightPosRight.z);
    glUniform3f(lightColorRightLoc, gLightColorRight.r, gLightColorRight.g, gLightColorRight.b);
    glUniform1f(lightIntenRightLoc, gLightIntenRight);

    // Set light property uniforms
    GLint specIntenLoc = glGetUniformLocation(gObjectProgramId, "specInten");
    GLint specSizeLoc = glGetUniformLocation(gObjectProgramId, "specSize");
    GLint ambStrenLoc = glGetUniformLocation(gObjectProgramId, "ambStren");
    glUniform1f(ambStrenLoc, gAmbientLightStrength);
    glUniform1f(specIntenLoc, gSpecularIntensity);
    glUniform1f(specSizeLoc, gSpecularHighlightSize);

    // Get the use decal uniform location
    GLuint useDecalLoc = glGetUniformLocation(gObjectProgramId, "useDecal");

    // Activate and bind the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexture);

    // If a decal texture is used for the mesh, activate and bind the decal texture
    if (gTextureDecal != gNoDecal)
    {
        // Activate decal texture rendering
        glUniform1i(useDecalLoc, true);

        // Activate and bind the decal texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gTextureDecal);
    }

    if (gMesh.enabled == true)
    {
        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMesh.vao);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
    }
    else if (gMeshIndexed.enabled == true)
    {
        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshIndexed.vao);

        // Draws the triangles
        glDrawElements(GL_TRIANGLE_STRIP, gMeshIndexed.nIndices, GL_UNSIGNED_SHORT, NULL);
    }

    // Deactivate decal texture rendering
    glUniform1i(useDecalLoc, false);

    // Deactivate the VAO and shader
    glBindVertexArray(0);
    glUseProgram(0);
}

// Draw a light source mesh with the given coordinates, color, intensity, rotation angle, axis rotation scalars, and size scalars
void UDrawLightMesh(GLMesh& gMesh, glm::vec3 lightPos, glm::vec3 lightColor, float lightIntensity,
    float rotAngle, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ)
{
    // Set the shader to be used
    glUseProgram(gLampProgramId);

    // Place object at the given coordinates
    glm::mat4 translation = glm::translate(lightPos);

    // Rotates the mesh by a given number of degrees along the specified axes
    glm::mat4 rotation = glm::rotate(glm::radians(rotAngle), glm::vec3(rotX, rotY, rotZ));

    // Scales the object by a given matrix scalar
    glm::mat4 scale = glm::scale(glm::vec3(scaleX, scaleY, scaleZ));

    // Model matrix transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Define the view matrix
    glm::mat4 view;

    // Define the projection matrix
    glm::mat4 projection;

    if (perspective) // If the user is in 3D mode
    {
        // Move the camera in 3D space
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Creates a perspective projection
        projection = glm::perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    else // If the user is in 2D mode
    {
        // Move the camera in 2D space
        view = glm::translate(glm::vec3(cameraPosOrtho.x, cameraPosOrtho.y, 0.0f));

        // Creates an orthogonal projection
        projection = glm::ortho(-orthoRight, orthoRight, -orthoTop, orthoTop, 0.1f, 100.0f);
    }

    // Set transform matrix uniforms
    GLint modelLoc = glGetUniformLocation(gLampProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gLampProgramId, "view");
    GLint projLoc = glGetUniformLocation(gLampProgramId, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set light mesh color uniform with the given color and intensity
    GLint lightColorBackLoc = glGetUniformLocation(gLampProgramId, "lightColor");
    glUniform3f(lightColorBackLoc, lightColor.r * lightIntensity, lightColor.g * lightIntensity, lightColor.b * lightIntensity);

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    // Deactivate the VAO and shader
    glBindVertexArray(0);
    glUseProgram(0);
}

// Draw the battery meshes at the given coordinates
void UDrawBattery(float x, float y, float z)
{
    // Set the shininess for the battery
    gSpecularIntensity = BATTERY_SPECULAR_INTENSITY;

    // Rotate the battery side so the "Duracell" label is facing the front
    float batteryRotationDegrees = 116.0f;

    // Make the battery larger
    float batteryScale = 2.0f;

    // Draw the battery case side mesh at the given coordinates with rotation along the Y-axis and defined scale
    UDrawObjectMesh(gMeshBatteryCaseSide, gMeshIndexed, gTextureBatteryCaseSide, gTextureBatteryCaseSideDecal, gUVScaleBatteryCaseSide,
        x, y, z, batteryRotationDegrees, 0.0f, 1.0f, 0.0f, batteryScale, batteryScale, batteryScale);

    // Draw the battery case top face mesh directly above the case of the battery with rotation along the Y-axis and defined scale
    UDrawObjectMesh(gMeshBatteryCaseTop, gMeshIndexed, gTextureBatteryCaseTop, gNoDecal, gUVScaleBatteryCaseTop,
        x, y + BATTERY_CASE_HEIGHT * batteryScale, z, batteryRotationDegrees, 0.0f, 1.0f, 0.0f, batteryScale, batteryScale, batteryScale);

    // Draw the battery case bottom face mesh directly under the case of the battery with rotation along the Y-axis and defined scale
    UDrawObjectMesh(gMeshBatteryCaseBottom, gMeshIndexed, gTextureBatteryCaseBottom, gNoDecal, gUVScaleBatteryCaseBottom,
        x, y, z, batteryRotationDegrees, 0.0f, 1.0f, 0.0f, batteryScale, batteryScale, batteryScale);

    // Draw the battery terminal side mesh directly above the case of the battery with rotation along the Y-axis and defined scale
    UDrawObjectMesh(gMeshBatteryTerminalSide, gMeshIndexed, gTextureBatteryTerminalSide, gNoDecal, gUVScaleBatteryTerminalSide,
        x, y + BATTERY_CASE_HEIGHT * batteryScale, z, batteryRotationDegrees, 0.0f, 1.0f, 0.0f, batteryScale, batteryScale, batteryScale);

    // Draw the battery terminal top mesh directly above the battery terminal side with rotation along the Y-axis and defined scale
    UDrawObjectMesh(gMeshBatteryTerminalTop, gMeshIndexed, gTextureBatteryTerminalTop, gNoDecal, gUVScaleBatteryTerminalTop,
        x, y + (BATTERY_CASE_HEIGHT + BATTERY_TERMINAL_HEIGHT) * batteryScale, z, batteryRotationDegrees, 0.0f, 1.0f, 0.0f, batteryScale, batteryScale, batteryScale);
}

// Draw the amp meshes at the given coordinates
void UDrawAmp(float x, float y, float z)
{
    // Set the shininess for the amp
    gSpecularIntensity = AMP_SPECULAR_INTENSITY;

    // Make the amp larger
    float ampScale = 5.0;

    // Move the volume knob to the right on the amp's front face
    float volumeKnobOffset = AMP_WIDTH * ampScale;

    // Create a very small Z-axis offset to avoid texture Z-fighting from the faces of the amp sides and the main body
    float zOffset = 0.0001f;
    
    // Draw the amp body mesh at the given coordinates with no rotation and the defined scale
    UDrawObjectMesh(gMeshAmp, gMeshIndexed, gTextureAmp, gNoDecal, gUVScaleAmp,
        x, y, z, 0.0f, 1.0f, 1.0f, 1.0f, ampScale, ampScale, ampScale);

    // Draw the volume knob side mesh at the given coordinates with 90 degree rotation along the X-axis and the defined scale
    UDrawObjectMesh(gMeshVolumeKnobSide, gMeshIndexed, gTextureVolumeKnobSide, gNoDecal, gUVScaleVolumeKnobSide,
        x + volumeKnobOffset, y + VOLUME_KNOB_RADIUS * ampScale, z, 90.0f, 1.0f, 0.0f, 0.0f, ampScale, ampScale, ampScale);

    // Draw the volume knob front mesh directly on top of the side mesh with 90 degree rotation along the X-axis and the defined scale
    UDrawObjectMesh(gMeshVolumeKnobFront, gMeshIndexed, gTextureVolumeKnobFront, gNoDecal, gUVScaleVolumeKnobFront,
        x + volumeKnobOffset, y + VOLUME_KNOB_RADIUS * ampScale, z + VOLUME_KNOB_HEIGHT * ampScale, 90.0f, 1.0f, 0.0f, 0.0f, ampScale, ampScale, ampScale);

    // Draw the amp side mesh on the right side of body with 90 degree rotation along the X-axis and the defined scale
    UDrawObjectMesh(gMeshAmpSide, gMeshIndexed, gTextureAmpSide, gNoDecal, gUVScaleAmpSide,
        x + volumeKnobOffset, y + VOLUME_KNOB_RADIUS * ampScale, z - AMP_LENGTH * ampScale, 90.0f, 1.0f, 0.0f, 0.0f, ampScale, ampScale, ampScale);

    // Draw the amp side back face mesh at the back of the right rounded side with 90 degree rotation along the X-axis and the defined scale
    UDrawObjectMesh(gMeshAmpSideBack, gMeshIndexed, gTextureAmpSideFace, gNoDecal, gUVScaleAmpSide,
        x + volumeKnobOffset, y + VOLUME_KNOB_RADIUS * ampScale, z + zOffset - AMP_LENGTH * ampScale, 90.0f, 1.0f, 0.0f, 0.0f, ampScale, ampScale, ampScale);

    // Draw the amp side mesh on the left side of body with 90 degree rotation along the X-axis and the defined scale
    UDrawObjectMesh(gMeshAmpSide, gMeshIndexed, gTextureAmpSide, gNoDecal, gUVScaleAmpSide,
        x , y + VOLUME_KNOB_RADIUS * ampScale, z - AMP_LENGTH * ampScale, 90.0f, 1.0f, 0.0f, 0.0f, ampScale, ampScale, ampScale);

    // Draw the amp side back face mesh at the back of the left rounded side with 90 degree rotation along the X-axis and the defined scale
    UDrawObjectMesh(gMeshAmpSideBack, gMeshIndexed, gTextureAmpSideFace, gNoDecal, gUVScaleAmpSide,
        x, y + VOLUME_KNOB_RADIUS * ampScale, z + zOffset - AMP_LENGTH * ampScale, 90.0f, 1.0f, 0.0f, 0.0f, ampScale, ampScale, ampScale);

    // Draw the amp side front face mesh at the front of the left rounded side with 90 degree rotation along the X-axis and the defined scale
    UDrawObjectMesh(gMeshAmpSideFront, gMeshIndexed, gTextureAmpSideFace, gNoDecal, gUVScaleAmpSide,
        x, y + VOLUME_KNOB_RADIUS * ampScale, z - zOffset, 90.0f, 1.0f, 0.0f, 0.0f, ampScale, ampScale, ampScale);
}

// Draw the marble mesh at the given coordinates
void UDrawMarble(float x, float y, float z)
{
    // Set the shininess for the marble
    gSpecularIntensity = MARBLE_SPECULAR_INTENSITY;

    // Make the marble smaller
    float marbleScale = 0.25f;

    // Draw the marble mesh at the given coordinates with no rotation and the defined scale
    UDrawObjectMesh(gMesh, gMeshMarble, gTextureMarble, gNoDecal, gUVScaleMarble,
        x, y + marbleScale, z, 0.0f, 1.0f, 1.0f, 1.0f, marbleScale, marbleScale, marbleScale);
}

// Draw the phone box mesh at the given coordinates
void UDrawPhoneBox(float x, float y, float z)
{
    // Set the shininess for the phone box
    gSpecularIntensity = PHONE_BOX_SPECULAR_INTENSITY;

    // Make the phone box larger
    float phoneBoxScale = 11.5f;

    // Draw the phone box mesh at the given coordinates with no rotation and the defined scale
    UDrawObjectMesh(gMeshPhoneBox, gMeshIndexed, gTexturePhoneBox, gNoDecal, gUVScalePhoneBox,
        x, y, z, 0.0f, 1.0f, 1.0f, 1.0f, phoneBoxScale, phoneBoxScale, phoneBoxScale);
}

// Draw the table surface mesh at the given coordinates
void UDrawTable(float x, float y, float z)
{
    // Set the shininess for the table
    gSpecularIntensity = TABLE_SPECULAR_INTENSITY;

    // Make the table larger
    float tableScale = 10.0f;

    // Draw the table surface mesh at the given coordinates with no rotation and default scale
    UDrawObjectMesh(gMeshTable, gMeshIndexed, gTextureTable, gNoDecal, gUVScaleTable,
        x, y, z, 0.0f, 1.0f, 1.0f, 1.0f, tableScale, tableScale, tableScale);
}

// ------------------------------------------------------------------------------------------------------------------------
// Setup functions
// ------------------------------------------------------------------------------------------------------------------------

// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // GLFW: window creation
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);

    // GLFW: enable mouse and scrollwheel input
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(gWindow, UMouseCallback);
    glfwSetScrollCallback(gWindow, UScrollWheelCallback);

    // GLEW: initialize
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}

// GLFW: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Create a shader program from the given vertex shader and fragment shader sources
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader
    glCompileShader(vertexShaderId);

    // Check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

    // If the vertex shader successfully compiled
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Compile the fragment shader
    glCompileShader(fragmentShaderId);

    // Check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

    // If the fragment shader successfully compiled
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    // Link the shader program
    glLinkProgram(programId);

    // Check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    // If the shader was successfully linked
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId); // Uses the shader program

    return true;
}

// Load all textures from their corresponding files
int ULoadTextures()
{
    // Load battery case side texture
    const char* texFilenameBatteryCaseSide = "resources/textures/battery_case_side.png";
    if (!UCreateTexture(texFilenameBatteryCaseSide, gTextureBatteryCaseSide, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameBatteryCaseSide << endl;
        return EXIT_FAILURE;
    }

    // Load battery case side texture decal for the positive and negative pole indicators
    const char* texFilenameBatteryCaseSideDecal = "resources/textures/battery_case_side_decal.png";
    if (!UCreateTexture(texFilenameBatteryCaseSideDecal, gTextureBatteryCaseSideDecal, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameBatteryCaseSideDecal << endl;
        return EXIT_FAILURE;
    }

    // Load battery case top texture
    const char* texFilenameBatteryCaseTop = "resources/textures/battery_case_top.png";
    if (!UCreateTexture(texFilenameBatteryCaseTop, gTextureBatteryCaseTop, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameBatteryCaseTop << endl;
        return EXIT_FAILURE;
    }

    // Load battery case bottom texture
    const char* texFilenameBatteryCaseBottom = "resources/textures/battery_case_bottom.png";
    if (!UCreateTexture(texFilenameBatteryCaseBottom, gTextureBatteryCaseBottom, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameBatteryCaseBottom << endl;
        return EXIT_FAILURE;
    }

    // Load battery terminal side texture
    const char* texFilenameBatteryTerminalSide = "resources/textures/battery_terminal_side.png";
    if (!UCreateTexture(texFilenameBatteryTerminalSide, gTextureBatteryTerminalSide, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameBatteryTerminalSide << endl;
        return EXIT_FAILURE;
    }

    // Load battery terminal top texture
    const char* texFilenameBatteryTerminalTop = "resources/textures/battery_terminal_top.png";
    if (!UCreateTexture(texFilenameBatteryTerminalTop, gTextureBatteryTerminalTop, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameBatteryTerminalTop << endl;
        return EXIT_FAILURE;
    }

    // Load amp body texture
    const char* texFilenameAmp = "resources/textures/amp.png";
    if (!UCreateTexture(texFilenameAmp, gTextureAmp, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameAmp << endl;
        return EXIT_FAILURE;
    }

    // Load amp side texture
    const char* texFilenameAmpSide = "resources/textures/amp_side.png";
    if (!UCreateTexture(texFilenameAmpSide, gTextureAmpSide, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameAmpSide << endl;
        return EXIT_FAILURE;
    }

    // Load amp side face texture
    const char* texFilenameAmpSideFace = "resources/textures/amp_side_face.png";
    if (!UCreateTexture(texFilenameAmpSideFace, gTextureAmpSideFace, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameAmpSideFace << endl;
        return EXIT_FAILURE;
    }

    // Load volume knob side texture
    const char* texFilenameVolumeKnobSide = "resources/textures/volume_knob_side.png";
    if (!UCreateTexture(texFilenameVolumeKnobSide, gTextureVolumeKnobSide, GL_REPEAT))
    {
        cout << "Failed to load texture " << texFilenameVolumeKnobSide << endl;
        return EXIT_FAILURE;
    }

    // Load volume knob front texture
    const char* texFilenameVolumeKnobFront = "resources/textures/volume_knob_front.png";
    if (!UCreateTexture(texFilenameVolumeKnobFront, gTextureVolumeKnobFront, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameVolumeKnobFront << endl;
        return EXIT_FAILURE;
    }

    // Load marble texture
    const char* texFilenameMarble = "resources/textures/marble.png";
    if (!UCreateTexture(texFilenameMarble, gTextureMarble, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenameMarble << endl;
        return EXIT_FAILURE;
    }

    // Load phone box texture
    const char* texFilenamePhoneBox = "resources/textures/phone_box.png";
    if (!UCreateTexture(texFilenamePhoneBox, gTexturePhoneBox, GL_CLAMP_TO_EDGE))
    {
        cout << "Failed to load texture " << texFilenamePhoneBox << endl;
        return EXIT_FAILURE;
    }

    // Load table texture
    const char* texFilenameTable = "resources/textures/table_tile.png";
    if (!UCreateTexture(texFilenameTable, gTextureTable, GL_REPEAT))
    {
        cout << "Failed to load texture " << texFilenameTable << endl;
        return EXIT_FAILURE;
    }

    return 0;
}

// Generate and bind the texture
bool UCreateTexture(const char* filename, GLuint& gTexture, int textureWrapType)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

    // If the image could be loaded
    if (image)
    {
        glGenTextures(1, &gTexture);
        glBindTexture(GL_TEXTURE_2D, gTexture);

        // Set the texture wrapping parameters to "clamp to edge" to prevent texture seams between meshes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapType);

        // Set texture parameters for trilinear filtering to reduce texture shimmer
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3) // No Transparency
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4) // Transparency
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture.

        return true;
    }

    // Error loading the image
    return false;
}

// ------------------------------------------------------------------------------------------------------------------------
// Destruction functions
// ------------------------------------------------------------------------------------------------------------------------

// Destroy a specified shader
void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

// Destroy a specified mesh
void UDestroyMesh(GLMesh& gMesh, GLMeshIndexed& gMeshIndexed)
{   
    // Delete the non-indexed mesh
    glDeleteVertexArrays(1, &gMesh.vao);
    glDeleteBuffers(1, &gMesh.vbo);

    // Delete the indexed mesh
    glDeleteVertexArrays(1, &gMeshIndexed.vao);
    glDeleteBuffers(2, gMeshIndexed.vbos);
}

// Destroy a specified texture
void UDestroyTexture(GLuint gTexture)
{
    glGenTextures(1, &gTexture);
}