#include "../include/Camera.hpp"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"


#include <iostream>

Camera& Camera::Instance(){
    static Camera* instance = new Camera();
    return *instance;
}

void Camera::MouseLook(int mouseX, int mouseY){
    // TODO: Which vector are you modifying?
	// *hint* not the eye...

    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);
    // Detect how much the mouse has moved since the last time
    glm::vec2 mouseDelta = 0.01f*(newMousePosition-m_oldMousePosition);

    m_viewDirection = glm::mat3(glm::rotate(-mouseDelta.x, m_upVector)) * m_viewDirection;
    m_rightVector = glm::cross(m_viewDirection, m_upVector);
    m_viewDirection = glm::mat3(glm::rotate(-mouseDelta.y, m_rightVector)) * m_viewDirection;

    // Update our old position after we have made changes
    m_oldMousePosition = newMousePosition;
}

void Camera::MoveForward(float speed){
    // TODO: Which vector are you modifying?
    if (GetViewZDirection() < 0) {
        speed = speed * -1.0f;
    }
    float angle = atan(GetViewXDirection()/GetViewZDirection());
    m_eyePosition.x += speed * sin(angle);
    m_eyePosition.z += speed * cos(angle);

    std::cout << m_eyePosition.x << ", " << m_eyePosition.y << ", " << m_eyePosition.z << std::endl;
}

void Camera::MoveBackward(float speed){
    // TODO: Which vector are you modifying?
    if (GetViewZDirection() > 0) {
        speed = speed * -1.0f;
    }
    float angle = atan(GetViewXDirection()/GetViewZDirection());
    m_eyePosition.x += speed * sin(angle);
    m_eyePosition.z += speed * cos(angle);

    std::cout << m_eyePosition.x << ", " << m_eyePosition.y << ", " << m_eyePosition.z << std::endl;
}

void Camera::MoveLeft(float speed){
    // TODO: Which vector are you modifying?
    if (GetViewZDirection() > 0) {
        speed = speed * -1.0f;
    }
    float angle = atan(m_rightVector.z/m_rightVector.x);
    m_eyePosition.x += -speed * cos(angle);
    m_eyePosition.z += -speed * sin(angle);

    std::cout << m_eyePosition.x << ", " << m_eyePosition.y << ", " << m_eyePosition.z << std::endl;
}

void Camera::MoveRight(float speed){
    // TODO: Which vector are you modifying?
    if (GetViewZDirection() > 0) {
        speed = speed * -1.0f;
    }
    float angle = atan(m_rightVector.z/m_rightVector.x);
    m_eyePosition.x += speed * cos(angle);
    m_eyePosition.z += speed * sin(angle);
    
    std::cout << m_eyePosition.x << ", " << m_eyePosition.y << ", " << m_eyePosition.z << std::endl;
    
}
    

void Camera::MoveUp(float speed){
    // TODO: Which vector are you modifying?
    m_eyePosition.y += speed;

    std::cout << m_eyePosition.x << ", " << m_eyePosition.y << ", " << m_eyePosition.z << std::endl;
}

void Camera::MoveDown(float speed){
    // TODO: Which vector are you modifying?
    m_eyePosition.y -= speed;

    std::cout << m_eyePosition.x << ", " << m_eyePosition.y << ", " << m_eyePosition.z << std::endl;
}

void Camera::Jump(float velocity) {
    yVelocity = velocity;
    m_eyePosition.y += 0.01f;
    jumping = true;
    std::cout << "jumped!" << std::endl;
}

void Camera::UpdateHeight(float gravity){
    if (jumping) {
        float deltaY = (yVelocity * jumpElapsedTime) + 0.5f * gravity * (float)pow(jumpElapsedTime, 2.0);
        if ((m_eyePosition.y += deltaY) > 0.35f) {
            m_eyePosition.y += deltaY;
        } else {
            m_eyePosition.y = 0.35f;
            jumping = false;
            std::cout << "on the ground: " << m_eyePosition.y << std::endl;
        }
        std::cout << "in the air: " << m_eyePosition.y << std::endl;
    }
}

void Camera::UpdateJumpTimes(){
    if (jumping) {
        jumpElapsedTime += 0.036f;
        std::cout << "time since jump: " << jumpElapsedTime << std::endl;
    } else {
        jumpElapsedTime = 0.0f;
    }
}

// Set the position for the camera
void Camera::SetCameraEyePosition(float x, float y, float z){
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

// Set the view direction for the camera
void Camera::SetCameraViewDirection(float x, float y, float z) {
    m_viewDirection.x = x;
    m_viewDirection.y = y;
    m_viewDirection.z = z;
}

float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}

bool Camera::isJumping(){
    return jumping;
}

Camera::Camera(){
    std::cout << "Created a Single Camera!\n";
	// Position us at the origin.
    m_eyePosition = glm::vec3(0.0f,0.0f, 0.0f);
	// Looking down along the z-axis initially.
	// Remember, this is negative because we are looking 'into' the scene.
    m_viewDirection = glm::vec3(0.0f,0.0f, -1.0f);
	// For now--our upVector always points up along the y-axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( m_eyePosition,
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
