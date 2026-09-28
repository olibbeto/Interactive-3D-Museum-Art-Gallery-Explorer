#ifndef CAMERA_H
#define CAMERA_H

#include "Input.h"
#include <vector>

struct BoundingBox {
    float minX, minZ;
    float maxX, maxZ;
    
    BoundingBox(float x1, float z1, float x2, float z2) 
        : minX(x1), minZ(z1), maxX(x2), maxZ(z2) {}
};

struct Vector3 {
    float x, y, z;
};

class Camera {
public:
    Camera(float startX, float startY, float startZ);
    
    void update(const Input& input, float deltaTime, const std::vector<BoundingBox>& walls);
    void processMouse(float deltaX, float deltaY);
    void applyView() const;
    
    Vector3 getPosition() const { return {x, y, z}; }

private:
    float x, y, z;
    float yaw, pitch;
    float speed;
    float sensitivity;
    
    bool checkCollision(float newX, float newZ, const std::vector<BoundingBox>& walls);
};

#endif // CAMERA_H
