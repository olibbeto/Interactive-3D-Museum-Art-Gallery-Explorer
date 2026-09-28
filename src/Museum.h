#ifndef MUSEUM_H
#define MUSEUM_H

#include <vector>
#include <string>
#include "Artwork.h"
#include "Sculpture.h"
#include "Camera.h" // For BoundingBox and Vector3

struct Room {
    std::string name;
    float minX, minZ;
    float maxX, maxZ;
    
    bool contains(float x, float z) const {
        return x >= minX && x <= maxX && z >= minZ && z <= maxZ;
    }
};

class Museum {
public:
    Museum();
    
    void init();
    void updateAnimations(float deltaTime);
    void draw(const Vector3& playerPos, const float sculptureLightPos[4] = nullptr) const;
    
    void toggleFloorStyle();
    int getFloorStyle() const { return floorStyle; }

    std::string getCurrentRoomName() const;
    const std::vector<BoundingBox>& getCollisionBoundaries() const { return collisionBoundaries; }
    
    bool isPlayerNearInteractable(const Vector3& playerPos) const;
    ArtInfo getCurrentInteractableInfo() const;

private:
    std::vector<Room> rooms;
    std::vector<Artwork> artworks;
    std::vector<Sculpture> sculptures;
    std::vector<BoundingBox> collisionBoundaries;
    
    int currentRoomIndex;
    mutable int currentInteractableIndex; // Cache for interaction
    int floorStyle; // 0 = Polished Marble, 1 = Rich Hardwood
    
    // Artwork textures
    unsigned int texRenaissance;
    unsigned int texImpressionist;
    unsigned int texModern;
    unsigned int texSurrealism;
    unsigned int texMonaLisa;
    unsigned int texStarryNight;
    unsigned int texGreatWave;
    unsigned int texMondrian;
    unsigned int texPearlEarring;
    unsigned int texKandinsky;
    unsigned int texVenus;
    
    // Architectural & Frame textures
    unsigned int texFloorMarble;
    unsigned int texFloorWood;
    unsigned int texWallPlaster;
    unsigned int texBaseboard;
    unsigned int texFrameGold;
    unsigned int texFrameWood;

    void updateCurrentRoom(const Vector3& playerPos);
    void buildArchitecture() const;
    void drawFloors() const;
    void drawWalls() const;
    void drawBaseboards() const;
    void drawCeilings() const;
    void createCollisionBoundaries();
};

#endif // MUSEUM_H
