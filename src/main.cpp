// Helper Classes
#include "Helpers.h"
#include "Camera.h"
#include "Image.h"
#include "Sphere.h"
#include "Object.h"
#include "Image.h"
#include "Texture.h"
#include "Skybox.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
/* GLFW to handle the OpenGL context */
#include <GLFW/glfw3.h>
#else
// GLFW to handle the OpenGL context
#include <GLFW/glfw3.h>
#endif

/* OpenGL Mathematics Library */
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Streams */
#include <iostream>
#include <fstream>
#include <sstream>

/* Mathematics */
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

/* Global Variables */
int winWidth = 1000;
int winHeight = 800;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = winWidth / 2.0f;
float lastY = winHeight / 2.0f;
float cameraSpeed = 2.0f;
float mouseSensitivity = 0.1f;
bool firstMouse = true;
int blurTimes = 20;
float gammaCorrection = 0.9f;
bool upKey = false;
bool downKey = false;

/* VAOs */
VertexArrayObject VAO;
VertexArrayObject QVAO;
VertexArrayObject SBVAO;

/* Vertex Buffer Objects */
BufferObject VBO;
BufferObject QVBO;
BufferObject SBVBO;
BufferObject NBO;
BufferObject EBO;
BufferObject TBO;

/* Buffer Variables */
GLuint FBO;
GLuint DBO;
GLuint BFBO;
GLuint BFBO2;

/* Camera */
Camera camera(glm::vec3(0.0f, 0.0f, 80.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, winWidth, winHeight);

glm::vec3 cameraPosition = camera.getPosition();
glm::vec3 cameraTarget = camera.getTarget();
glm::vec3 cameraDirection = camera.getDirection();
glm::vec3 cameraUp = camera.getUp();
glm::vec3 cameraRight = camera.getRightCam();
glm::mat4 viewMatrix;
glm::mat4 projMatrix = camera.getProjection();
glm::mat4 modelMatrix = camera.getModel();
glm::mat4 mvpMatrix;

/* Functions */
void fbSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_A:
        camera.setPosition(camera.getPosition() - (camera.getRightCam() * cameraSpeed));
        camera.updateCamera();
        break;
    case GLFW_KEY_D:
        camera.setPosition(camera.getPosition() + (camera.getRightCam() * cameraSpeed));
        camera.updateCamera();
        break;
    case GLFW_KEY_W:
        camera.setPosition(camera.getPosition() + (cameraSpeed * camera.getDirection()));
        camera.updateCamera();
        break;
    case GLFW_KEY_S:
        camera.setPosition(camera.getPosition() - (cameraSpeed * camera.getDirection()));
        camera.updateCamera();
        break;
    case GLFW_KEY_R:
        camera.resetCamera();
        break;
    case GLFW_KEY_UP:
        gammaCorrection += 0.01f;
        break;
    case GLFW_KEY_DOWN:
        gammaCorrection -= 0.01f;
        break;
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    default:
        break;
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    float xposin = static_cast<float>(xpos);
    float yposin = static_cast<float>(ypos);

    if (firstMouse)
    {
        lastX = xposin;
        lastY = yposin;
        firstMouse = false;
    }

    float xoffset = xposin - lastX;
    float yoffset = lastY - yposin;
    lastX = xposin;
    lastY = yposin;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    camera.setYaw(camera.getYaw() + xoffset);
    camera.setPitch(camera.getPitch() + yoffset);

    if (camera.getPitch() > 89.0f)
    {
        camera.setPitch(89.0f);
    }

    if (camera.getPitch() < -89.0f)
    {
        camera.setPitch(-89.0f);
    }

    camera.updateDirection();
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Activate supersampling */
    glfwWindowHint(GLFW_SAMPLES, 8);

    /* Ensure that we get at least a 3.2 context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

/* On apple we have to load a core profile with forward compatibility */
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a window */
    window = glfwCreateWindow(winWidth, winHeight, "Hello OpenGL", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    glewExperimental = true;
    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        cerr << "Error: " << glewGetErrorString(err) << endl;
    }

    glGetError(); /* pull and savely ignonre unhandled errors like GL_INVALID_ENUM */
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
#endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    cout << "OpenGL version received: " << major << "." << minor << "." << rev << endl;
    cout << "Supported OpenGL is " << (const char *)glGetString(GL_VERSION) << endl;
    cout << "Supported GLSL is " << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    /* Quad VAO */
    QVAO.init();
    QVAO.bind();

    QVBO.init();

    vector<glm::vec3> QV(3);

    QV.resize(0);
    QV.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
    QV.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
    QV.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
    QV.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
    QV.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
    QV.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

    QVBO.update(QV);

    /* Skybox VAO */
    SBVAO.init();
    SBVAO.bind();

    SBVBO.init();

    Skybox skybox;

    SBVBO.update(skybox.getVertices());

    vector<string> skyboxMap;
    skyboxMap.resize(0);
    skyboxMap.push_back("../data/milkyWayPositiveX.ppm");
    skyboxMap.push_back("../data/milkyWayNegativeX.ppm");
    skyboxMap.push_back("../data/milkyWayPositiveY.ppm");
    skyboxMap.push_back("../data/milkyWayNegativeY.ppm");
    skyboxMap.push_back("../data/milkyWayPositiveZ.ppm");
    skyboxMap.push_back("../data/milkyWayNegativeZ.ppm");

    skybox.createSkyBoxMap(skyboxMap);

    /* General VAO */
    VAO.init();
    VAO.bind();

    VBO.init();
    NBO.init();
    EBO.init(GL_ELEMENT_ARRAY_BUFFER);
    TBO.init();

    /* Planets */
    Sphere sphere(glm::vec3(0.0, 0.0, 0.0), 1.0f, 30, 10, glm::vec3(0.0, 0.0, 0.0), 0.8f, 50.0f);

    sphere.setAmbient(0.8f);
    float ambient = sphere.getAmbient();
    sphere.setSpecularExp(50.0f);
    float specularExp = sphere.getSpecularExp();

    VBO.update(sphere.getVertices());
    NBO.update(sphere.getNormals());
    EBO.update(sphere.getIndicies());
    TBO.update(sphere.getTexCoords());

    Image sun("../data/sun.ppm");
    Texture sunTexture(stoi(sun.getWidth()), stoi(sun.getHeight()), sun.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image uranus("../data/uranus.ppm");
    Texture uranusTexture(stoi(uranus.getWidth()), stoi(uranus.getHeight()), uranus.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image venus("../data/venus.ppm");
    Texture venusTexture(stoi(venus.getWidth()), stoi(venus.getHeight()), venus.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image mars("../data/mars.ppm");
    Texture marsTexture(stoi(mars.getWidth()), stoi(mars.getHeight()), mars.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image mercury("../data/mercury.ppm");
    Texture mercuryTexture(stoi(mercury.getWidth()), stoi(mercury.getHeight()), mercury.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image saturn("../data/saturn.ppm");
    Texture saturnTexture(stoi(saturn.getWidth()), stoi(saturn.getHeight()), saturn.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image saturnRing("../data/saturnRing.ppm");
    Texture saturnRingTexture(stoi(saturnRing.getWidth()), stoi(saturnRing.getHeight()), saturnRing.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image earth("../data/earth.ppm");
    Texture earthTexture(stoi(earth.getWidth()), stoi(earth.getHeight()), earth.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image specularEarth("../data/specularEarth.ppm");
    Texture specularEarthTexture(stoi(specularEarth.getWidth()), stoi(specularEarth.getHeight()), specularEarth.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image neptune("../data/neptune.ppm");
    Texture neptuneTexture(stoi(neptune.getWidth()), stoi(neptune.getHeight()), neptune.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image jupiter("../data/jupiter.ppm");
    Texture jupiterTexture(stoi(jupiter.getWidth()), stoi(jupiter.getHeight()), jupiter.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image moon("../data/moon.ppm");
    Texture moonTexture(stoi(moon.getWidth()), stoi(moon.getHeight()), moon.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    Image milkyWay("../data/milkyWay.ppm");
    Texture milkyWayTexture(stoi(milkyWay.getWidth()), stoi(milkyWay.getHeight()), milkyWay.getRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    /* Initialize the OpenGL Program */
    Program program;

    stringstream fragCode;
    stringstream vertCode;

    ifstream fragShaderFile("../shader/fragment.glsl");
    fragCode << fragShaderFile.rdbuf();
    ifstream vertShaderFile("../shader/vertex.glsl");
    vertCode << vertShaderFile.rdbuf();

    program.init(vertCode.str(), fragCode.str(), "fragColor", "brightFragColor");

    program.bind();

    program.bindVertexAttribArray("position", VBO);
    program.bindVertexAttribArray("normal", NBO);
    program.bindVertexAttribArray("texcoord", TBO);

    /* Program Uniforms */
    glUniform3fv(program.uniform("lightPosition"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
    glUniform3f(program.uniform("lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform1f(program.uniform("ambient"), ambient);
    glUniform1f(program.uniform("specularExp"), specularExp);

    /* Skybox Program */
    Program sbprogram;

    stringstream sbfragCode;
    stringstream sbvertCode;

    ifstream sbfragShaderFile("../shader/sbfragment.glsl");
    sbfragCode << sbfragShaderFile.rdbuf();
    ifstream sbvertShaderFile("../shader/sbvertex.glsl");
    sbvertCode << sbvertShaderFile.rdbuf();

    sbprogram.init(sbvertCode.str(), sbfragCode.str(), "sbFragColor");

    /* Skybox Uniforms */

    /* Render To Texture Program */
    Program rtprogram(1);
    stringstream rtfragCode;
    stringstream rtvertCode;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    Texture rtTexture(winWidth * 2, winHeight * 2, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rtTexture.getID(), 0);

    Texture rtbrightTexture(winWidth * 2, winHeight * 2, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, rtbrightTexture.getID(), 0);

    GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    glGenRenderbuffers(1, &DBO);
    glBindRenderbuffer(GL_RENDERBUFFER, DBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, winWidth * 2, winHeight * 2);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DBO);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "FRAMEBUFFER ERROR" << endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ifstream rtfragShaderFile("../shader/rtfragment.glsl");
    rtfragCode << rtfragShaderFile.rdbuf();
    ifstream rtvertShaderFile("../shader/rtvertex.glsl");
    rtvertCode << rtvertShaderFile.rdbuf();

    rtprogram.init(rtvertCode.str(), rtfragCode.str(), "rtFragColor");

    rtprogram.bind();

    /* Render To Texture Program Uniforms */

    /* Blur Texture Program */
    Program bprogram(2);
    stringstream bfragCode;
    stringstream bvertCode;

    /* Blur Framebuffers */

    glGenFramebuffers(1, &BFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, BFBO);

    Texture blurTexture(winWidth * 2, winHeight * 2, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexture.getID(), 0);

    GLenum blurBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, blurBuffers);

    GLenum bstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (bstatus != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "FRAMEBUFFER ERROR" << endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &BFBO2);
    glBindFramebuffer(GL_FRAMEBUFFER, BFBO2);

    Texture blur2Texture(winWidth * 2, winHeight * 2, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur2Texture.getID(), 0);

    GLenum blur2Buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, blur2Buffers);

    GLenum b2status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (b2status != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "FRAMEBUFFER ERROR" << endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ifstream bfragShaderFile("../shader/bfragment.glsl");
    bfragCode << bfragShaderFile.rdbuf();
    ifstream bvertShaderFile("../shader/bvertex.glsl");
    bvertCode << bvertShaderFile.rdbuf();

    bprogram.init(bvertCode.str(), bfragCode.str(), "bFragColor");

    /* Blur Texture Program Uniforms */

    /* Disable Cursor */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Update viewport */
    glfwSetFramebufferSizeCallback(window, fbSizeCallback);

    /* Update on keypress */
    glfwSetKeyCallback(window, keyCallback);

    /* Update on mouse */
    glfwSetCursorPosCallback(window, mouseCallback);

    float spin[8] = {140.0f, 580.0f, 2.5f, 2.55f, 1.0f, 1.05f, 1.5f, 1.55f}, rot[8] = {1.0f, 1 / 3.0f, 1 / 4.0f, 1 / 8.0f, 1 / 48.0f, 1 / 100.0f, 1 / 250.0f, 1 / 500.0f}, ts[8] = {140.0f, 580.0f, 2.5f, 2.55f, 1.0f, 1.05f, 1.5f, 1.55f}, tr[8] = {1.0f, 1 / 3.0f, 1 / 4.0f, 1 / 8.0f, 1 / 48.0f, 1 / 100.0f, 1 / 250.0f, 1 / 500.0f};
    
    while (!glfwWindowShouldClose(window))
    {

        /* Time Calculation */
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /* Get the size of the window */
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        /* Update Camera */
        viewMatrix = camera.getView();
        modelMatrix = camera.getModel();
        mvpMatrix = camera.getMVP();
        cameraPosition = camera.getPosition();

        /* Bind FBO */
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        /* Bind program */
        program.bind();

        VAO.bind();

        EBO.bind();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        /* Updating Uniform Values */
        glUniform3fv(program.uniform("camPosition"), 1, glm::value_ptr(cameraPosition));
        glUniformMatrix4fv(program.uniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(program.uniform("projMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniformMatrix4fv(program.uniform("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));

        /* Active Textures */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunTexture.getID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mercuryTexture.getID());

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, venusTexture.getID());

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, earthTexture.getID());

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, moonTexture.getID());

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, marsTexture.getID());

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, jupiterTexture.getID());

        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, saturnTexture.getID());

        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_2D, uranusTexture.getID());

        glActiveTexture(GL_TEXTURE9);
        glBindTexture(GL_TEXTURE_2D, neptuneTexture.getID());

        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_2D, specularEarthTexture.getID());

        /* Can change translation and scale below */

        glUniform1i(program.uniform("isEarth"), 0);

        /* Sun */
        glm::mat4 transModelMatrix = glm::scale(modelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 0);
        glUniform1i(program.uniform("bloom"), 1);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Mercury */
        spin[0] += ts[0] * 0.0775f, rot[0] += tr[0] * 0.075f;
        transModelMatrix = glm::rotate(modelMatrix, rot[0] / 10.f, glm::vec3(0.0f, 1.0f, 0.0f));
        transModelMatrix = glm::translate(transModelMatrix, glm::vec3(25.0f, 0.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
        transModelMatrix = glm::rotate(transModelMatrix, 1 / spin[0], glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 1);
        glUniform1i(program.uniform("bloom"), 0);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Venus */
        spin[1] += ts[1] * 0.0775f, rot[1] += tr[1] * 0.075f;
        transModelMatrix = glm::rotate(modelMatrix, rot[1] / 10.f, glm::vec3(0.0f, 1.0f, 0.0f));
        transModelMatrix = glm::translate(transModelMatrix, glm::vec3(35.0f, 0.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(4.75f, 4.75f, 4.75f));
        transModelMatrix = glm::rotate(transModelMatrix, spin[1], glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 2);
        glUniform1i(program.uniform("bloom"), 0);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Earth */
        spin[2] += ts[2] * 0.0775f, rot[2] += tr[2] * 0.075f;
        transModelMatrix = glm::rotate(modelMatrix, rot[2] / 10.f, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 temp = glm::translate(transModelMatrix, glm::vec3(50.0f, 0.0f, 0.0f));
        transModelMatrix = glm::rotate(temp, spin[2], glm::vec3(0.0f, 1.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 3);
        glUniform1i(program.uniform("specTextureID"), 10);
        glUniform1i(program.uniform("bloom"), 0);
        glUniform1i(program.uniform("isEarth"), 1);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Moon */
        transModelMatrix = glm::rotate(temp, rot[2] / 2.f, glm::vec3(0.0f, 0.0f, -1.0f));
        transModelMatrix = glm::translate(transModelMatrix, glm::vec3(7.5f, 0.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 4);
        glUniform1i(program.uniform("bloom"), 0);
        glUniform1i(program.uniform("isEarth"), 0);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Mars */
        spin[3] += ts[3] * 0.0775f, rot[3] += tr[3] * 0.075f;
        transModelMatrix = glm::rotate(modelMatrix, rot[3] / 10.f, glm::vec3(0.0f, 1.0f, 0.0f));
        transModelMatrix = glm::translate(modelMatrix, glm::vec3(60.0f, 0.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(2.75f, 2.75f, 2.75f));
        transModelMatrix = glm::rotate(transModelMatrix, spin[3], glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 5);
        glUniform1i(program.uniform("bloom"), 0);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Jupiter */
        spin[4] += ts[4] * 0.0775f, rot[4] += tr[4] * 0.075f;
        transModelMatrix = glm::rotate(modelMatrix, rot[4] / 10.f, glm::vec3(0.0f, 1.0f, 0.0f));
        transModelMatrix = glm::translate(transModelMatrix, glm::vec3(80.0f, 0.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(10.75f, 10.75f, 10.75f));
        transModelMatrix = glm::rotate(transModelMatrix, spin[4], glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 6);
        glUniform1i(program.uniform("bloom"), 0);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Saturn */
        spin[5] += ts[5] * 0.0775f, rot[5] += tr[5] * 0.075f;
        transModelMatrix = glm::rotate(modelMatrix, rot[5] / 10.f, glm::vec3(0.0f, 1.0f, 0.0f));
        transModelMatrix = glm::translate(transModelMatrix, glm::vec3(105.0f, 0.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(9.25f, 9.25f, 9.25f));
        transModelMatrix = glm::rotate(transModelMatrix, spin[5], glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 7);
        glUniform1i(program.uniform("bloom"), 0);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Uranus */
        spin[6] += ts[6] * 0.0775f, rot[6] += tr[6] * 0.075f;
        transModelMatrix = glm::rotate(modelMatrix, rot[6] / 10.f, glm::vec3(0.0f, 1.0f, 0.0f));
        transModelMatrix = glm::translate(transModelMatrix, glm::vec3(120.0f, 0.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(3.25f, 3.25f, 3.25f));
        transModelMatrix = glm::rotate(transModelMatrix, spin[6], glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 8);
        glUniform1i(program.uniform("bloom"), 0);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Neptune */
        spin[7] += ts[7] * 0.0775f, rot[7] += tr[7] * 0.075f;
        transModelMatrix = glm::rotate(modelMatrix, rot[7] / 10.f, glm::vec3(0.0f, 1.0f, 0.0f));
        transModelMatrix = glm::translate(transModelMatrix, glm::vec3(130.0f, 0.0f, 0.0f));
        transModelMatrix = glm::scale(transModelMatrix, glm::vec3(2.75f, 2.75f, 2.75f));
        transModelMatrix = glm::rotate(transModelMatrix, spin[7], glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(program.uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(transModelMatrix));
        glUniform1i(program.uniform("textureID"), 9);
        glUniform1i(program.uniform("bloom"), 0);
        glDrawElements(GL_TRIANGLES, sphere.getIndicies().size() * 3, GL_UNSIGNED_INT, 0);

        /* Skybox */

        glDepthFunc(GL_LEQUAL);

        sbprogram.bind();

        SBVAO.bind();

        sbprogram.bindVertexAttribArray("position", SBVBO);

        glUniform3fv(sbprogram.uniform("camPosition"), 1, glm::value_ptr(cameraPosition));
        glUniformMatrix4fv(sbprogram.uniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(sbprogram.uniform("projMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniformMatrix4fv(sbprogram.uniform("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture());

        glUniform1i(sbprogram.uniform("textureID"), 0);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LESS);

        /* Blur Texture */
        
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bool curBFBO = true;
        bool firstIteration = true;

        bprogram.bind();

        QVAO.bind();

        bprogram.bindVertexAttribArray("quadPosition", QVBO);

        for (int i = 0; i < blurTimes; i++)
        {
            if (curBFBO)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, BFBO);
            }
            else
            {
                glBindFramebuffer(GL_FRAMEBUFFER, BFBO2);
            }

            glUniform1i(bprogram.uniform("curBFBO"), curBFBO);

            glActiveTexture(GL_TEXTURE0);

            if (firstIteration)
            {
                glBindTexture(GL_TEXTURE_2D, rtbrightTexture.getID());
            }
            else
            {
                if (curBFBO)
                {
                    glBindTexture(GL_TEXTURE_2D, blur2Texture.getID());
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, blurTexture.getID());
                }
            }

            glUniform1i(bprogram.uniform("bTextureID"), 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            curBFBO = !curBFBO;

            if (firstIteration)
            {
                firstIteration = false;
            }
        }

        /* Render To Texture */

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, width * 2, height * 2);

        rtprogram.bind();

        QVAO.bind();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);

        rtprogram.bindVertexAttribArray("quadPosition", QVBO);

        glUniform1f(rtprogram.uniform("gammaCorrection"), gammaCorrection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rtTexture.getID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, blur2Texture.getID());

        glUniform1i(rtprogram.uniform("rtTextureID"), 0);
        glUniform1i(rtprogram.uniform("bTextureID"), 1);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    program.free();
    rtprogram.free();
    bprogram.free();
    VAO.free();
    QVAO.free();
    VBO.free();
    QVBO.free();
    EBO.free();
    NBO.free();
    TBO.free();

    glDisable(GL_TEXTURE_2D);
    glDeleteFramebuffers(1, &FBO);
    glDeleteFramebuffers(1, &BFBO);
    glDeleteFramebuffers(1, &BFBO2);
    glDeleteRenderbuffers(1, &DBO);

    /* Deallocate glfw internals */
    glfwTerminate();
    return 0;
}
