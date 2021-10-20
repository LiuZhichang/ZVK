//
//  Camera.hpp
//  VulkanTemplate
//
//  Created by 刘致畅 on 2021/9/27.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "../ZVKInternal.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef ZVK_USE_GLFW
struct GLFWWindow;
#endif

class Camera {
public:
    enum Pos {
        X = 0x01,
        Y = 0x02,
        Z = 0x04
    };

    //常规构造
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);

    //欧拉角构造
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);


    //获取观察矩阵
    glm::mat4 GetViewMatrix();

    //鼠标移动
    void ProcessMouseMovement(float deltaX, float deltaY);

    void ProcessMouseScroll(float yOffset);

    float Zoom() const {
        return zoom;
    }

    void move(Pos pos, float offset);

    void moveSence(float sence);


#ifdef ZVK_USE_GLFW
    void UpdataCameraPosition(GLFWwindow* window);
#endif

private:
    //更新摄像机角度
    void UpdataCameraVectors();

private:
    glm::vec3 position;         //摄像机位置
    glm::vec3 forward;          //摄像机方向
    glm::vec3 right;            //右轴
    glm::vec3 up;               //上轴
    glm::vec3 worldUp;          //世界上轴

    float pitch;                //欧拉_俯仰角
    float yaw;                  //欧拉_偏航角
    float senceX = 0.0004f;     //鼠标移动X速率
    float senceY = 0.0004f;     //鼠标移动Y速率

    float speedX = 0.01f;       //键盘X移动速率
    float speedY = 0.01f;       //键盘Y移动速率
    float speedZ = 0.01f;       //键盘Z移动速率

    float zoom = 45.0f;
};

#endif /* Camera_hpp */
