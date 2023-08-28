#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

class Camera
{
private:
    float aspectRatio;
    float width;
    float height;
    float angle;
    float deltaTime;
    float lastFrame;
    float rotCamera = 0.0f;
    float radius = 1.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;

    glm::vec3 position; // location
    glm::vec3 startPosition;
    glm::vec3 target;    // lookat
    glm::vec3 up;        // up
    glm::vec3 direction; // w
    glm::vec3 upCam;     // u
    glm::vec3 rightCam;  // v
    glm::mat4 view;
    glm::mat4 invview;
    glm::mat4 projection;
    glm::mat4 invprojection;
    glm::mat4 model;
    glm::mat4 mvp;

public:
    Camera(){};
    Camera(const glm::vec3 e, const glm::vec3 t, const glm::vec3 v, float radians, const int width, const int height);
    ~Camera(){};

    void setAspectRatio(const float aspect) { aspectRatio = aspect; };
    float getAspectRatio() { return aspectRatio; };

    void setHeight(const float h) { height = h; };
    float getHeight() { return height; };

    void setWidth(const float w) { width = w; };
    float getWidth() { return width; };

    void setAngle(const float radians) { angle = glm::radians(radians); };
    float getAngle() { return angle; };

    void setRadius(const float r) { radius = r; };
    float getRadius() { return radius; };

    void setYaw(const float y) { yaw = y; };
    float getYaw() { return yaw; };

    void setPitch(const float p) { pitch = p; };
    float getPitch() { return pitch; };

    void setPosition(const glm::vec3 &e) { position = e; };
    glm::vec3 getPosition() { return position; };

    void setTarget(const glm::vec3 &t) { target = t; };
    glm::vec3 getTarget() { return target; };

    void setUp(const glm::vec3 &v) { up = v; };
    glm::vec3 getUp() { return up; };

    void setDirection(const glm::vec3 &d) { direction = d; };
    glm::vec3 getDirection() { return direction; };

    void calRight() { rightCam = glm::normalize(glm::cross(direction, up)); };
    glm::vec3 getRightCam() { return rightCam; };

    void calUp() { upCam = glm::normalize(glm::cross(rightCam, direction)); };
    glm::vec3 getUpCam() { return upCam; };
    ;

    void createView()
    {
        view = glm::lookAt(position, position + direction, upCam);
        invview = glm::inverse(view);
    };

    glm::mat4 getView() { return view; };

    void createProjection()
    {
        projection = glm::perspective(angle, (float)width / (float)height, 0.1f, 100.0f);
        invprojection = glm::inverse(projection);
    };

    glm::mat4 getProjection() { return projection; };

    glm::mat4 getModel() { return model; };

    glm::mat4 getMVP() { return mvp; };

    void updateCamera();

    void updateDirection();

    void resetCamera();
};

Camera::Camera(const glm::vec3 e, const glm::vec3 t, const glm::vec3 v, float radians, const int iwidth, const int iheight)
{
    width = (float)iwidth;
    height = (float)iheight;
    aspectRatio = (float)iwidth / (float)iheight;
    angle = glm::radians(radians);
    deltaTime = 0.0f;
    lastFrame = 0.0f;

    startPosition = e;
    position = e;
    target = t;
    up = v;
    direction = glm::vec3(0.0f, 0.0f, -1.0f);

    projection = glm::perspective(angle, aspectRatio, 0.1f, 100.0f);
    invprojection = glm::inverse(projection);

    model = glm::mat4(1.0f);

    updateCamera();
};

void Camera::updateCamera()
{
    rightCam = glm::normalize(glm::cross(direction, up));
    upCam = glm::normalize(glm::cross(rightCam, direction));

    view = glm::lookAt(position, position + direction, upCam);
    invview = glm::inverse(view);

    mvp = projection * view * model;
};

void Camera::updateDirection()
{
    glm::vec3 orientation;
    orientation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    orientation.y = sin(glm::radians(pitch));
    orientation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    direction = glm::normalize(orientation);

    updateCamera();
};

void Camera::resetCamera()
{
    position = startPosition;
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    updateCamera();
}

#endif