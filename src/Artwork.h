#ifndef ARTWORK_H
#define ARTWORK_H

#include <string>

struct ArtInfo {
    std::string title;
    std::string artist;
    std::string category;
    std::string description;
};

enum FrameType {
    FRAME_GOLD = 0,
    FRAME_WOOD = 1
};

class Artwork {
public:
    Artwork(float x, float y, float z, float width, float height, float rotY, 
            int style, const ArtInfo& info, unsigned int textureID = 0, FrameType frameType = FRAME_GOLD);
    
    static void setFrameTextures(unsigned int goldTex, unsigned int woodTex);

    void draw() const;
    bool isPlayerNear(float px, float pz) const;
    ArtInfo getInfo() const { return info; }

private:
    float x, y, z;
    float width, height;
    float rotY;
    int style; // Procedural art style generator index
    ArtInfo info;
    float interactionRadius;
    unsigned int textureID;
    FrameType frameType;
    
    static unsigned int texFrameGold;
    static unsigned int texFrameWood;

    void drawFrame() const;
    void drawCanvas() const;
    void generateProceduralArt() const;
};

#endif // ARTWORK_H

