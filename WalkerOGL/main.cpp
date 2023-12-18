//main.cpp
//Walker McGilvary 2023

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include <direct.h>

#include "stb_image.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace glm;

//vertex struct
struct Vertex {
    vec3 pos;
    vec3 color;

    Vertex(float x, float y, float z) {
        pos = vec3(x, y, z);

        float red = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        color = vec3(red, green, blue);
    }
};

// function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


//CAMERA
vec3 cameraPos(0.0f, 0.0f, 0.0f);
const float CAMERA_SPEED = 0.01f;
Camera cam;


int main()
{

    //print current directory
    char buffer[FILENAME_MAX];
    _getcwd(buffer, FILENAME_MAX);
    std::cout << "Current working directory: " << buffer << std::endl;


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Walker's Cool Graphics", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //prepare mouse with glfw
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //load shaders
    Shader ourShader("VertexShader.glsl", "FragmentShader.glsl");


    Vertex vertices[] = {
        Vertex(0.5f, 0.5f, 0.5f),
        Vertex(-0.5f, 0.5f, -0.5f),
        Vertex(-0.5f, 0.5f, 0.5f),
        Vertex(0.5f, -0.5f, -0.5f),
        Vertex(-0.5f, -0.5f, -0.5f),
        Vertex(0.5f, 0.5f, -0.5f),
        Vertex(0.5f, -0.5f, 0.5f),
        Vertex(-0.5f, -0.5f, 0.5f)
    };
    unsigned int indices[] = {
        0,1,2,
        1,3,4,
        5,6,3,
        7,3,6,
        2,4,7,
        0,7,6,
        0,5,1,
        1,5,3,
        5,0,6,
        7,4,3,
        2,1,4,
        0,2,7
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //bind the VAO first, then bind and set buffers, then configure vertex attributes
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);



    //unbind buffer, then vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //textures
    //unsigned int texture;
    //glGenTextures(1, &texture);
    //glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    //// set the texture wrapping parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //// set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //// load image, create texture and generate mipmaps
    //int width, height, nrChannels;
    //// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    //if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data);






    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);


    //render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate the shader
        ourShader.use();

        // Model transformation: Rotate the cube around the Z-axis over time
        float t = (float)glfwGetTime();
        mat4 model(
            cos(t), 0, sin(t), 0,
            0, 1, 0, 0,
            -sin(t), 0, cos(t), 0,
            0, 0, 0, 1
        );
        model = transpose(model);

        // View transformation: Translate the scene in the reverse direction of where you want the camera to move
        mat4 view = cam.getViewMatrix();

        // Projection transformation: Create a perspective projection matrix
        float fov = radians(45.0f);
        float nearPlane = 1.0f;
        float farPlane = 100.0f;
        float aspectRatio = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
        mat4 projection(
            1 / (aspectRatio * tan(fov / 2)), 0, 0, 0,
            0, 1 / (tan(fov / 2)), 0, 0,
            0, 0, -(farPlane + nearPlane) / (farPlane - nearPlane), -2 * farPlane * nearPlane / (farPlane - nearPlane),
            0, 0, -1, 0
        );
        projection = transpose(projection); //because glm is column major :(


        // Combine the transformations
        mat4 trans = projection * view * model;

        // Pass the combined transformation matrix to the shader
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(trans));

        // Bind the vertex array object and draw the elements
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    vec3 moveDir(0, 0, 0);

    //movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveDir.z -= 1; // Move forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveDir.z += 1; // Move backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveDir.x -= 1; // Move left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveDir.x += 1; // Move right
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        moveDir.y += 1;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        moveDir.y -= 1;

    if (length(moveDir) > 0)
        cam.updatePos(normalize(moveDir));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    vec2 mousePos((float)xpos, (float)-ypos);

    cam.updateMouse(mousePos);

}