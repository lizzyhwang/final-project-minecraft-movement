/** @file Camera.hpp
 *  @brief Sets up an OpenGL camera.
 *  
 *  Sets up an OpenGL Camera. The camera is what
 *  sets up our 'view' matrix.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

class Camera{
public:
	// Singleton pattern for having one single camera.
	static Camera& Instance();
    // Return a 'view' matrix with our
    // camera transformation applied.
    glm::mat4 GetWorldToViewmatrix() const;
    // Move the camera around
    void MouseLook(int mouseX, int mouseY);
    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);
    void MoveUp(float speed);
    void MoveDown(float speed);
    void Jump(float velocity);
    void UpdateHeight(float gravity);
    void UpdateJumpTimes();
    // Set the position for the camera
    void SetCameraEyePosition(float x, float y, float z);
    // Set the view direction for the camera
    void SetCameraViewDirection(float x, float y, float z);
    // Returns the 'eye' position which
    // is where the camera is.
    float GetEyeXPosition();
    float GetEyeYPosition();
    float GetEyeZPosition();
	// Returns the 'view' position
    float GetViewXDirection();
    float GetViewYDirection();
    float GetViewZDirection();
    // Returns whether or not we are jumping
    bool isJumping();
private:
	// Constructor is private because we should
    // not be able to construct any cameras,
    // this how we ensure only one is ever created
    Camera();
    // Track the old mouse position
    glm::vec2 m_oldMousePosition;
    // Where is our camera positioned
    glm::vec3 m_eyePosition;
    // What direction is the camera looking
    glm::vec3 m_viewDirection;
    // Which direction is 'up' in our world
    // Generally this is constant, but if you wanted
    // to 'rock' or 'rattle' the camera you might play
    // with modifying this value.
    glm::vec3 m_upVector = {glm::vec3(0.0f,1.0f,0.0f)};
    // Which direction is 'right' in reference to the camera.
    glm::vec3 m_rightVector = {glm::vec3(1.0f,0.0f,0.0f)};

    // Initial horizontal angle : toward -Z
    float horizontalAngle = 3.14f;
    // Initial vertical angle : none
    float verticalAngle = 0.0f;
    // mouse speed
    float mouseSpeed = 0.0001f;

    // are we in the air
    bool jumping = {false};

    // y velocity
    float yVelocity = {0};

    // jump elapsed time
    float jumpStartTime = {0};
    float jumpCurTime = {0};
    float jumpElapsedTime = {0};
    
};




#endif
