#include "Camera.h"
#include <GL/freeglut.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Camera::Camera(float startX, float startY, float startZ) 
    : x(startX), y(startY), z(startZ), yaw(-90.0f), pitch(0.0f), speed(5.0f), sensitivity(0.15f) {}

void Camera::update(const Input& input, float deltaTime, const std::vector<BoundingBox>& walls) {
    float moveSpeed = speed * deltaTime;
    
    // Calculate forward and right vectors based on yaw (ignoring pitch for movement so we don't fly)
    float radYaw = yaw * M_PI / 180.0f;
    float dirX = cos(radYaw);
    float dirZ = sin(radYaw);
    
    float rightX = cos(radYaw + M_PI / 2.0f);
    float rightZ = sin(radYaw + M_PI / 2.0f);
    
    float nextX = x;
    float nextZ = z;

    if (input.isKeyDown('w')) {
        nextX += dirX * moveSpeed;
        nextZ += dirZ * moveSpeed;
    }
    if (input.isKeyDown('s')) {
        nextX -= dirX * moveSpeed;
        nextZ -= dirZ * moveSpeed;
    }
    if (input.isKeyDown('a')) {
        nextX -= rightX * moveSpeed;
        nextZ -= rightZ * moveSpeed;
    }
    if (input.isKeyDown('d')) {
        nextX += rightX * moveSpeed;
        nextZ += rightZ * moveSpeed;
    }
    
    // Simple sliding collision: check X and Z movement independently
    if (!checkCollision(nextX, z, walls)) {
        x = nextX;
    }
    if (!checkCollision(x, nextZ, walls)) {
        z = nextZ;
    }
}

bool Camera::checkCollision(float newX, float newZ, const std::vector<BoundingBox>& walls) {
    // Player bounding radius
    float radius = 0.3f; 
    
    for (const auto& wall : walls) {
        if (newX + radius > wall.minX && newX - radius < wall.maxX &&
            newZ + radius > wall.minZ && newZ - radius < wall.maxZ) {
            return true;
        }
    }
    return false;
}

void Camera::processMouse(float deltaX, float deltaY) {
    yaw += deltaX * sensitivity;
    pitch -= deltaY * sensitivity; // Minus because y goes down in screen coordinates
    
    // Clamp pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void Camera::applyView() const {
    float radYaw = yaw * M_PI / 180.0f;
    float radPitch = pitch * M_PI / 180.0f;
    
    float lookX = cos(radYaw) * cos(radPitch);
    float lookY = sin(radPitch);
    float lookZ = sin(radYaw) * cos(radPitch);
    
    gluLookAt(x, y, z, 
              x + lookX, y + lookY, z + lookZ, 
              0.0f, 1.0f, 0.0f);
}
