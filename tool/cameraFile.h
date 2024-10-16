#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>



// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.1f;
const GLfloat ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           GLfloat yaw = YAW, 
           GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
           MovementSpeed(SPEED), 
           MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Up = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        GLfloat x = 0, y = 0;
        this->updateCameraVectors(x, y);
    }
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Up = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        //this->updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        glm::vec3 target(0.0f, 0.0f, 0.0f);
        return glm::lookAt(this->Position, target, this->Up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;
        this->Pitch -= yoffset;

        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors(xoffset, yoffset);
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset)
    {
        this->Zoom -= yoffset;
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors(GLfloat &xoffset, GLfloat &yoffset)
    {
        //ÈÆµã£¨0.0£¬ 0.0£¬ 0.0£©Ðý×ª
        glm::vec3 direction = this->Front - this->Position;
        direction = glm::normalize(direction);
        this->Right = glm::normalize(glm::cross(direction, this->Up));
        
        glm::quat aQuat{ 0, 0, 0, 1 };
        aQuat = glm::angleAxis(yoffset * this->MouseSensitivity, this->Right);
        aQuat = glm::angleAxis(xoffset * this->MouseSensitivity, this->Up) * aQuat;

        auto func = [](glm::vec3 src, const glm::vec3& center, const glm::quat& aQuat)
        {
            src -= center;
            src = src * glm::mat3_cast(aQuat);
            src += center;
            return src;
        };
        glm::vec3 center{ 0.0, 0.0, 0.0 };
        this->Position = func(this->Position, center, aQuat);
        this->Front = func(this->Front, center, aQuat);
        glm::vec3 dir = glm::normalize(this->Position - this->Front);

        this->Up = glm::normalize(glm::cross(dir, this->Right));
    }
};