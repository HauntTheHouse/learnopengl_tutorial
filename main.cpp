#include <iostream>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Shader.h"
#include "Cumera.h"
#include "Mesh.h"
#include "Model.h"

int width = 1366, height = 720;

float lastX = (float)width/2, lastY = (float)height/2;

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(0.0f, 0.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

Camera cumera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const * path);

void fillVerticesOfSphere(std::vector<float> &vertices);
void fillIndicesOfSphere(std::vector<unsigned int> &indices);
void initCube(unsigned int &VAO, unsigned int &VBO, std::vector<float> &vertices);
void initSphere(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO, std::vector<float> &vertices, std::vector<unsigned int> &indices);

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Usual staff", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if(glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
//    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }

//
//    const char* vertexPathLighting = "../shaders/lighting.vert";
//    const char* fragmentPathLighting = "../shaders/lighting.frag";
//    Shader lightingShader(vertexPathLighting, fragmentPathLighting);
//
    const char* vertexPathLamp = "../shaders/lamp.vert";
    const char* fragmentPathLamp = "../shaders/lamp.frag";
    Shader lampShader(vertexPathLamp, fragmentPathLamp);

//
//    std::vector<float> vertices = {
//            // координати            // нормалі             // текстурні координати
//            -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
//             0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
//             0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
//             0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
//            -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
//
//            -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
//             0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
//             0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
//             0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
//            -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
//
//            -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
//            -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
//
//             0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
//             0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
//             0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
//             0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
//             0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
//             0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
//
//            -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
//             0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
//             0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
//             0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
//            -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
//
//            -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
//             0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
//             0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
//             0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
//            -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f
//    };
//
//    glm::vec3 cubePositions[] = {
//            glm::vec3( 0.0f,  0.0f,  0.0f),
//            glm::vec3( 2.0f,  5.0f, -15.0f),
//            glm::vec3(-1.5f, -2.2f, -2.5f),
//            glm::vec3(-3.8f, -2.0f, -12.3f),
//            glm::vec3( 2.4f, -0.4f, -3.5f),
//            glm::vec3(-1.7f,  3.0f, -7.5f),
//            glm::vec3( 1.3f, -2.0f, -2.5f),
//            glm::vec3( 1.5f,  2.0f, -2.5f),
//            glm::vec3( 1.5f,  0.2f, -1.5f),
//            glm::vec3(-1.3f,  1.0f, -1.5f)
//    };
//
    std::vector<float> verticesOfSphere;
    std::vector<unsigned int> indicesOfSphere;

    fillVerticesOfSphere(verticesOfSphere);
    fillIndicesOfSphere(indicesOfSphere);
//
//    std::string path = "../images/box.png";
//    unsigned int diffuseMap = loadTexture(path.c_str());
//    path = "../images/border.png";
//    unsigned int specularMap = loadTexture(path.c_str());
//    path = "../images/matrix.jpg";
//    unsigned int emissionMap = loadTexture(path.c_str());
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, diffuseMap);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, specularMap);
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, emissionMap);
//
//    unsigned int lightingVAO, lightingVBO, lightingEBO;
//    initCube(lightingVAO, lightingVBO, vertices);
////    initSphere(lightingVAO, lightingVBO, lightingEBO, verticesOfSphere, indicesOfSphere);
//
    unsigned int lampVAO, lampVBO, lampEBO;
    initSphere(lampVAO, lampVBO, lampEBO, verticesOfSphere, indicesOfSphere);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
    Model ourModel("../model/backpack.obj");

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.0f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(cumera.Zoom), (float)width / (float)height, 0.01f, 100.0f);
        glm::mat4 view = cumera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Рендеринг загруженной модели
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos); // смещаем вниз чтобы быть в центре сцены
        model = glm::scale(model, glm::vec3(0.5f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
        model = glm::rotate(model, (float)glfwGetTime()*glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(3.5f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime()*glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));


        ourShader.setMat4("model", model);

        ourShader.setVec3("pointLight.position",  glm::vec3(view * glm::vec4(lightPos, 1.0f)));

        ourShader.setVec3("pointLight.ambient",  lightColor * glm::vec3(0.05f));
        ourShader.setVec3("pointLight.diffuse",  lightColor * glm::vec3(1.0f));
        ourShader.setVec3("pointLight.specular", lightColor * glm::vec3(1.0f));

        ourShader.setFloat("pointLight.constant", 1.0f);
        ourShader.setFloat("pointLight.linear", 0.07f);
        ourShader.setFloat("pointLight.quadratic", 0.017f);

        ourModel.Draw(ourShader);
//
//        lightingShader.use();
//
////        lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
////        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
////        lightingShader.setVec3("material.specular", 1.0f, 0.5f, 0.31f);
//
//        lightingShader.setInt("material.diffuse", 0);
//        lightingShader.setInt("material.specular", 1);
//        lightingShader.setInt("material.emission", 2);
//        lightingShader.setFloat("material.shininess", 16.0f);
//
//
//        glm::vec3 dirColor = glm::vec3(sin(1 + glfwGetTime() * 0.7f), sin(1 + glfwGetTime() * 2.0f), sin(glfwGetTime() * 0.5f));
//        lightingShader.setVec3("dirLight.direction",  glm::vec3(0.0f, -1.0f, 0.0f));
//
//        lightingShader.setVec3("dirLight.ambient",  dirColor * glm::vec3(0.01f));
//        lightingShader.setVec3("dirLight.diffuse",  dirColor * glm::vec3(0.2f));
//        lightingShader.setVec3("dirLight.specular", dirColor * glm::vec3(0.2f));
//
//        lightColor.x = sin(glfwGetTime() * 2.0f);
//        lightColor.y = sin(glfwGetTime() * 0.7f);
//        lightColor.z = sin(glfwGetTime() * 1.3f);
//
//        lightingShader.setVec3("pointLight.position",  glm::vec3(0.0f, 0.0f, 0.0));
//
//        lightingShader.setVec3("pointLight.ambient",  lightColor * glm::vec3(0.05f));
//        lightingShader.setVec3("pointLight.diffuse",  lightColor * glm::vec3(0.6f));
//        lightingShader.setVec3("pointLight.specular", lightColor * glm::vec3(0.6f));
//
//        lightingShader.setFloat("pointLight.constant", 1.0f);
//        lightingShader.setFloat("pointLight.linear", 0.07f);
//        lightingShader.setFloat("pointLight.quadratic", 0.017f);
//
//
//        lightingShader.setVec3("flashLight.position",  glm::vec3(0.0f, 0.0f, 0.0));
//        lightingShader.setVec3("flashLight.direction", glm::vec3(0.0f, 0.0f, -1.0));
//        lightingShader.setFloat("flashLight.innerCutOff",   glm::cos(glm::radians(12.5f)));
//        lightingShader.setFloat("flashLight.outerCutOff",   glm::cos(glm::radians(17.5f)));
//
//        lightingShader.setVec3("flashLight.ambient",  glm::vec3(0.1f));
//        lightingShader.setVec3("flashLight.diffuse",  glm::vec3(1.0f));
//        lightingShader.setVec3("flashLight.specular", glm::vec3(1.0f));
//
//        lightingShader.setFloat("flashLight.constant", 1.0f);
//        lightingShader.setFloat("flashLight.linear", 0.07f);
//        lightingShader.setFloat("flashLight.quadratic", 0.017f);
//
////        lightingShader.setVec3("viewPos",  cumera.Position);
//
//        view = glm::mat4(1.0f);
//        view = cumera.GetViewMatrix();
//        lightingShader.setMat4("view", view);
//
//        projection = glm::mat4(1.0f);
//        projection = glm::perspective(glm::radians(cumera.Zoom), (float)width / (float)height, 0.001f, 100.0f);
//        lightingShader.setMat4("projection", projection);
//
////        model = glm::mat4(1.0f);
////        model = glm::translate(model, lightPos);
////        model = glm::rotate(model, (float)glfwGetTime()*glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
////        model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
////        model = glm::rotate(model, (float)glfwGetTime()*glm::radians(-30.0f), glm::vec3(0.5f, 1.0f, 0.2f));
////
////        lightingShader.setMat4("model", model);
//
//        glBindVertexArray(lightingVAO);
////        glBindBuffer(GL_ARRAY_BUFFER, lightingVBO);
////        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightingEBO);
//        for(unsigned int i = 0; i < 10; i++)
//        {
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
//            model = glm::translate(model, cubePositions[i]);
//            float angle = 20.0f * i * glfwGetTime();
//            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//            lightingShader.setMat4("model", model);
//
//            glDrawArrays(GL_TRIANGLES, 0, 36);
////            glDrawElements(GL_TRIANGLES, indicesOfSphere.size(), GL_UNSIGNED_INT, (void*)0);
//        }
//
////        glDrawElements(GL_TRIANGLES, indicesOfSphere.size(), GL_UNSIGNED_INT, (void*)0);
////        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
        lampShader.use();
        lampShader.setVec3("lightColor", lightColor);

        glBindVertexArray(lampVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lampEBO);

        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.3f));
        lampShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, indicesOfSphere.size(), GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
//    glDeleteVertexArrays(1, &lightingVAO);
//    glDeleteVertexArrays(1, &lampVAO);
//    glDeleteBuffers(1, &lightingVBO);
//    glDeleteBuffers(1, &lampVBO);
//    glDeleteBuffers(1, &lightingEBO);
//    glDeleteBuffers(1, &lampEBO);

    glfwTerminate();
    return 0;
}

void fillVerticesOfSphere(std::vector<float> &vertices)
{
    unsigned int sectorCount = 60;
    unsigned int stackCount = 60;
    float radius = 0.5;

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2.0f * M_PI / (float)sectorCount;
    float stackStep = M_PI / (float)stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2.0 - (float)i * stackStep;
        xy = (float)radius * cos(stackAngle);
        z = (float)radius * sin(stackAngle);
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = (float)j * sectorStep;
            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
            s = j / (float)sectorCount;
            t = i / (float)stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }
}

void fillIndicesOfSphere(std::vector<unsigned int> &indices)
{
    int k1, k2;

    unsigned int stackCount = 60;
    unsigned int sectorCount = 60;

    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if(i != stackCount - 1)
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void initCube(unsigned int &VAO, unsigned int &VBO, std::vector<float> &vertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void initSphere(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO, std::vector<float> &vertices, std::vector<unsigned int> &indices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int imageWidth, imageHeight, nrComponents;
    unsigned char *data = stbi_load(path, &imageWidth, &imageHeight, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;

    cumera.ProcessMouseMovement(xoffset, yoffset);

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cumera.ProcessMouseScroll(yoffset);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cumera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cumera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cumera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cumera.ProcessKeyboard(RIGHT, deltaTime);
}