//
//  Camera.cpp
//  VulkanTemplate
//
//  Created by 刘致畅 on 2021/9/27.
//

#include "Camera.hpp"

#ifdef VK_USE_GLFW
#include <GLFW/glfw3.h>
#endif

//常规构造
Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup) {
    this->position = position;
    this->worldUp = worldup;
    this->forward = glm::normalize(target - position);
    this->right = glm::normalize(glm::cross(forward, worldup));
    this->up = glm::normalize(glm::cross(right, forward));
}

//欧拉角构造
Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup) {
    this->position = position;
    this->worldUp = worldup;
    this->pitch = pitch;
    this->yaw = yaw;
    UpdataCameraVectors();
}

//获取视图观察矩阵
glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + forward, worldUp);
}

//更新摄像机角度
void Camera::UpdataCameraVectors() {
    forward.x = glm::cos(pitch) * glm::sin(yaw);
    forward.y = glm::sin(pitch);
    forward.z = glm::cos(pitch) * glm::cos(yaw);
    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
}

//鼠标移动
void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
    pitch += deltaY * senceX;
    yaw += deltaX * senceY;
    UpdataCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset) {
    if (zoom >= 1.0f && zoom <= 45.0f) {
        zoom -= yOffset;
    } else if (zoom <= 1.0f) {
        zoom = 1.0f;
    } else if (zoom >= 45.0f) {
        zoom = 45.0f;
    }
}

void Camera::move(Pos pos, float offset) {
    switch (pos) {
        case X:
            speedX += offset;
            break;
        case Y:
            speedY += offset;
            break;
        case Z:
            speedZ += offset;
            break;

        default:
            break;
    }
}

#ifdef USE_GLFW
void Camera::UpdataCameraPosition(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        this->move(Pos::Z, 2.0f);
    } else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        this->move(Pos::Z, -2.0f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->move(Pos::X,1.0f);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->move(Pos::X, -1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->move(Pos::Y, 1.0f);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->move(Pos::Y, -1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        this->move(Pos::Z, 1.0f);
    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        this->move(Pos::Z, -1.0f);
    }
}
#endif
