#include "Museum.h"
#include "Texture.h"
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <algorithm>

Museum::Museum() : currentRoomIndex(0), currentInteractableIndex(-1), floorStyle(0),
                   texRenaissance(0), texImpressionist(0), texModern(0), texSurrealism(0), texMonaLisa(0),
                   texStarryNight(0), texGreatWave(0), texMondrian(0), texPearlEarring(0), texKandinsky(0), texVenus(0),
                   texFloorMarble(0), texFloorWood(0), texWallPlaster(0), texBaseboard(0), texFrameGold(0), texFrameWood(0) {}

void Museum::init() {
    // Load artwork textures
    texRenaissance = Texture::loadTexture("assets/textures/renaissance.jpg");
    texImpressionist = Texture::loadTexture("assets/textures/impressionist.jpg");
    texModern = Texture::loadTexture("assets/textures/modern.jpg");
    texSurrealism = Texture::loadTexture("assets/textures/surrealism.jpg");
    texMonaLisa = Texture::loadTexture("assets/textures/monalisa.jpg");
    texStarryNight = Texture::loadTexture("assets/textures/starry_night.jpg");
    texGreatWave = Texture::loadTexture("assets/textures/great_wave.jpg");
    texMondrian = Texture::loadTexture("assets/textures/mondrian.jpg");
    texPearlEarring = Texture::loadTexture("assets/textures/pearl_earring.jpg");
    texKandinsky = Texture::loadTexture("assets/textures/kandinsky.jpg");
    texVenus = Texture::loadTexture("assets/textures/birth_of_venus.jpg");

    // Load architectural and frame textures
    texFloorMarble = Texture::loadTexture("assets/textures/floor_marble.jpg");
    texFloorWood   = Texture::loadTexture("assets/textures/floor_wood.jpg");
    texWallPlaster = Texture::loadTexture("assets/textures/wall_plaster.jpg");
    texBaseboard   = Texture::loadTexture("assets/textures/wood_dark.jpg");
    texFrameGold   = Texture::loadTexture("assets/textures/frame_gold.jpg");
    texFrameWood   = Texture::loadTexture("assets/textures/wood_dark.jpg");

    // Configure shared frame textures for all artworks
    Artwork::setFrameTextures(texFrameGold, texFrameWood);

    // Define Rooms
    rooms.push_back({"MAIN HALL", -5.0f, -5.0f, 5.0f, 5.0f});
    rooms.push_back({"MODERN ART GALLERY", -20.0f, -5.0f, -5.0f, 5.0f});
    rooms.push_back({"CLASSICAL ART GALLERY", 5.0f, -5.0f, 20.0f, 5.0f});
    rooms.push_back({"SCULPTURE GALLERY", -5.0f, -20.0f, 5.0f, -5.0f});
    rooms.push_back({"IMPRESSIONIST GALLERY", -20.0f, -20.0f, -5.0f, -5.0f});
    rooms.push_back({"RENAISSANCE GALLERY", 5.0f, -20.0f, 20.0f, -5.0f});
    
    // Create Collision Boundaries
    createCollisionBoundaries();
    
    // Populate Artworks with specific frames (Gold for Classical/Renaissance, Wood for Modern/Contemporary)
    
    // Modern Art Gallery (Left Room)
    artworks.push_back(Artwork(-19.8f, 2.0f, 0.0f, 2.0f, 2.0f, 90.0f, 5, 
        {"GEOMETRIC SYMPHONY", "V. Vector", "Modern", "An exploration of sharp angles and bright colors."}, texModern, FRAME_WOOD));
    artworks.push_back(Artwork(-17.5f, 2.0f, -4.8f, 3.0f, 1.5f, 0.0f, 1, 
        {"THE GREY", "A. Nonymous", "Abstract", "A study in grey and shadows."}, 0, FRAME_WOOD));
    artworks.push_back(Artwork(-7.5f, 2.0f, -4.8f, 2.0f, 2.0f, 0.0f, 0, 
        {"COMPOSITION WITH RED BLUE AND YELLOW", "Piet Mondrian", "De Stijl", "A purely abstract geometric composition."}, texMondrian, FRAME_WOOD));
    artworks.push_back(Artwork(-12.5f, 2.0f, 4.8f, 3.0f, 3.0f, 180.0f, 0, 
        {"COMPOSITION 8", "Wassily Kandinsky", "Abstract", "A logical and geometric abstraction."}, texKandinsky, FRAME_WOOD));
    
    // Classical Gallery (Right Room)
    artworks.push_back(Artwork(19.8f, 2.0f, 0.0f, 2.0f, 2.5f, -90.0f, 2, 
        {"SCULPTED LIGHT", "F. Classic", "Classical", "Classic geometric composition."}, 0, FRAME_GOLD));
    artworks.push_back(Artwork(15.0f, 2.0f, 4.8f, 2.5f, 1.5f, 180.0f, 6, 
        {"DIGITAL RINGS", "O. Circles", "Digital", "Hypnotic concentric rings of cyan and magenta."}, 0, FRAME_WOOD));
    artworks.push_back(Artwork(10.0f, 2.0f, 4.8f, 3.0f, 2.0f, 180.0f, 0, 
        {"THE BIRTH OF VENUS", "Sandro Botticelli", "Renaissance", "A masterpiece of the Italian Renaissance."}, texVenus, FRAME_GOLD));
    artworks.push_back(Artwork(7.5f, 2.0f, -4.8f, 2.5f, 1.8f, 0.0f, 0, 
        {"THE GREAT WAVE", "Hokusai", "Ukiyo-e", "One of the most recognizable works of Japanese art in the world."}, texGreatWave, FRAME_WOOD));
    
    // Main Hall (Entrance / Info)
    artworks.push_back(Artwork(-2.0f, 2.0f, 4.8f, 2.0f, 1.5f, 180.0f, 4, 
        {"MUSEUM INTRODUCTION", "Curator", "Information", "Welcome to the Interactive 3D Museum."}, 0, FRAME_WOOD));
    
    artworks.push_back(Artwork(-3.5f, 2.0f, -4.8f, 1.5f, 2.0f, 0.0f, 0,
        {"MONA LISA", "Leonardo da Vinci", "Renaissance", "The most famous portrait in the world."}, texMonaLisa, FRAME_GOLD));
    
    artworks.push_back(Artwork(17.5f, 2.0f, -4.8f, 2.0f, 2.0f, 0.0f, 0, 
        {"SUNSET CALM", "N. Ature", "Landscape", "A beautiful gradient sunset."}, 0, FRAME_GOLD));
        
    artworks.push_back(Artwork(3.5f, 2.0f, -4.8f, 2.2f, 2.2f, 0.0f, 0, 
        {"THE STARRY NIGHT", "Vincent van Gogh", "Post-Impressionism", "A magnificent night sky over a sleepy town."}, texStarryNight, FRAME_GOLD));
        
    // Impressionist Gallery (Back Left)
    artworks.push_back(Artwork(-12.5f, 2.0f, -19.8f, 3.0f, 2.0f, 0.0f, 0, 
        {"PARISIAN EVENING", "C. Monet-esque", "Impressionism", "A lively street scene at dusk with beautiful atmospheric lighting."}, texImpressionist, FRAME_GOLD));
    artworks.push_back(Artwork(-19.8f, 2.0f, -12.5f, 2.0f, 2.0f, 90.0f, 0, 
        {"MELTED TIME", "S. Dali-esque", "Surrealism", "A dreamlike desert landscape featuring melting clocks."}, texSurrealism, FRAME_WOOD));
        
    // Renaissance Gallery (Back Right)
    artworks.push_back(Artwork(12.5f, 2.0f, -19.8f, 3.0f, 2.0f, 0.0f, 0, 
        {"ALPINE VILLAGE", "L. Da Vinci-esque", "Renaissance", "A masterfully detailed classical painting of a mountain village by a lake."}, texRenaissance, FRAME_GOLD));
    artworks.push_back(Artwork(19.8f, 2.0f, -12.5f, 1.8f, 2.2f, -90.0f, 0, 
        {"GIRL WITH A PEARL EARRING", "Johannes Vermeer", "Baroque", "A tronie of a girl with a headscarf and a pearl earring."}, texPearlEarring, FRAME_GOLD));

    // Populate Sculptures (Sculpture Gallery - back room)
    sculptures.push_back(Sculpture(-2.0f, 0.0f, -15.0f, 0)); // Twisted Torus
    sculptures.push_back(Sculpture(2.0f, 0.0f, -15.0f, 1));  // Stacked Spheres
    sculptures.push_back(Sculpture(0.0f, 0.0f, -10.0f, 2));  // Abstract Wireframe
}

void Museum::toggleFloorStyle() {
    floorStyle = (floorStyle == 0) ? 1 : 0;
    std::cout << "[Museum] Floor toggled to: " << (floorStyle == 0 ? "Polished Marble" : "Rich Hardwood") << std::endl;
}

void Museum::createCollisionBoundaries() {
    float t = 0.5f; // Wall thickness approximation
    
    // Outer walls
    collisionBoundaries.push_back(BoundingBox(-20-t, -20-t, 20+t, -20+t)); // Back
    collisionBoundaries.push_back(BoundingBox(-20-t, 5-t, 20+t, 5+t));     // Front
    collisionBoundaries.push_back(BoundingBox(-20-t, -20-t, -20+t, 5+t));  // Left
    collisionBoundaries.push_back(BoundingBox(20-t, -20-t, 20+t, 5+t));    // Right
    
    // Left divider (Main to Modern)
    collisionBoundaries.push_back(BoundingBox(-5-t, 2, -5+t, 5));
    collisionBoundaries.push_back(BoundingBox(-5-t, -5, -5+t, -2));
    
    // Right divider (Main to Classical)
    collisionBoundaries.push_back(BoundingBox(5-t, 2, 5+t, 5));
    collisionBoundaries.push_back(BoundingBox(5-t, -5, 5+t, -2));
    
    // Back divider (Main to Sculpture room)
    collisionBoundaries.push_back(BoundingBox(-5, -5-t, -2, -5+t));
    collisionBoundaries.push_back(BoundingBox(2, -5-t, 5, -5+t));
    
    // Left back divider (Modern to Impressionist)
    collisionBoundaries.push_back(BoundingBox(-20, -5-t, -15, -5+t));
    collisionBoundaries.push_back(BoundingBox(-10, -5-t, -5, -5+t));
    
    // Right back divider (Classical to Renaissance)
    collisionBoundaries.push_back(BoundingBox(5, -5-t, 10, -5+t));
    collisionBoundaries.push_back(BoundingBox(15, -5-t, 20, -5+t));
    
    // Divider between Sculpture and Impressionist (Left)
    collisionBoundaries.push_back(BoundingBox(-5-t, -20, -5+t, -15));
    collisionBoundaries.push_back(BoundingBox(-5-t, -10, -5+t, -5));
    
    // Divider between Sculpture and Renaissance (Right)
    collisionBoundaries.push_back(BoundingBox(5-t, -20, 5+t, -15));
    collisionBoundaries.push_back(BoundingBox(5-t, -10, 5+t, -5));
    
    // Pedestals bounding boxes
    collisionBoundaries.push_back(BoundingBox(-2.5f, -15.5f, -1.5f, -14.5f));
    collisionBoundaries.push_back(BoundingBox(1.5f, -15.5f, 2.5f, -14.5f));
    collisionBoundaries.push_back(BoundingBox(-0.5f, -10.5f, 0.5f, -9.5f));
}

void Museum::updateAnimations(float deltaTime) {
    for (auto& s : sculptures) {
        s.update(deltaTime);
    }
}

void Museum::draw(const Vector3& playerPos, const float sculptureLightPos[4]) const {
    const_cast<Museum*>(this)->updateCurrentRoom(playerPos);
    
    buildArchitecture();
    
    for (const auto& art : artworks) {
        art.draw();
    }
    
    // Planar shadows for sculptures projected onto the floor
    if (sculptureLightPos != nullptr) {
        for (const auto& s : sculptures) {
            s.drawShadow(sculptureLightPos);
        }
    }

    for (const auto& s : sculptures) {
        s.draw();
    }
}

void Museum::updateCurrentRoom(const Vector3& playerPos) {
    for (size_t i = 0; i < rooms.size(); ++i) {
        if (rooms[i].contains(playerPos.x, playerPos.z)) {
            currentRoomIndex = i;
            break;
        }
    }
}

std::string Museum::getCurrentRoomName() const {
    if (currentRoomIndex >= 0 && currentRoomIndex < rooms.size()) {
        return rooms[currentRoomIndex].name;
    }
    return "UNKNOWN AREA";
}

bool Museum::isPlayerNearInteractable(const Vector3& playerPos) const {
    currentInteractableIndex = -1;
    for (size_t i = 0; i < artworks.size(); ++i) {
        if (artworks[i].isPlayerNear(playerPos.x, playerPos.z)) {
            currentInteractableIndex = i;
            return true;
        }
    }
    return false;
}

ArtInfo Museum::getCurrentInteractableInfo() const {
    if (currentInteractableIndex >= 0 && currentInteractableIndex < artworks.size()) {
        return artworks[currentInteractableIndex].getInfo();
    }
    return {"", "", "", ""};
}

void Museum::buildArchitecture() const {
    drawFloors();
    drawWalls();
    drawBaseboards();
    drawCeilings();
}

void Museum::drawFloors() const {
    glEnable(GL_TEXTURE_2D);

    if (floorStyle == 0) {
        // Polished White Carrara Marble
        glBindTexture(GL_TEXTURE_2D, texFloorMarble);
        GLfloat mat_ambient[]  = { 0.35f, 0.35f, 0.35f, 1.0f };
        GLfloat mat_diffuse[]  = { 0.95f, 0.95f, 0.95f, 1.0f };
        GLfloat mat_specular[] = { 0.65f, 0.65f, 0.65f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 65.0f);
    } else {
        // Luxury Polished Hardwood Parquet
        glBindTexture(GL_TEXTURE_2D, texFloorWood);
        GLfloat mat_ambient[]  = { 0.30f, 0.25f, 0.20f, 1.0f };
        GLfloat mat_diffuse[]  = { 0.88f, 0.82f, 0.78f, 1.0f };
        GLfloat mat_specular[] = { 0.38f, 0.32f, 0.28f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 35.0f);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE); // Floor must never be culled

    const float tileSize = 2.5f;
    const float minX = -20.0f, maxX = 20.0f;
    const float minZ = -20.0f, maxZ = 5.0f;
    const float step = 1.0f; // 1m subdivision ensures smooth point lighting and specular sheen

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (float x = minX; x < maxX; x += step) {
        float xNext = std::min(x + step, maxX);
        for (float z = minZ; z < maxZ; z += step) {
            float zNext = std::min(z + step, maxZ);

            // Counter-clockwise winding viewed from above (+Y looking down)
            glTexCoord2f(x / tileSize, z / tileSize);
            glVertex3f(x, 0.0f, z);

            glTexCoord2f(x / tileSize, zNext / tileSize);
            glVertex3f(x, 0.0f, zNext);

            glTexCoord2f(xNext / tileSize, zNext / tileSize);
            glVertex3f(xNext, 0.0f, zNext);

            glTexCoord2f(xNext / tileSize, z / tileSize);
            glVertex3f(xNext, 0.0f, z);
        }
    }
    glEnd();

    glEnable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}

// Subdivided double-sided textured wall chunk
static void drawTexturedWallBlock(float x1, float z1, float x2, float z2, float h, float texTile = 2.5f) {
    float dx = x2 - x1;
    float dz = z2 - z1;
    float len = std::sqrt(dx * dx + dz * dz);
    if (len < 0.001f) return;

    int steps = (int)std::ceil(len / 2.0f);
    if (steps < 1) steps = 1;

    float stepDist = len / steps;
    float dirX = dx / len;
    float dirZ = dz / len;

    // Normal vectors
    float nx1 = -dirZ, nz1 = dirX; // normal side 1
    float nx2 = dirZ, nz2 = -dirX; // normal side 2

    for (int i = 0; i < steps; ++i) {
        float dA = i * stepDist;
        float dB = (i + 1) * stepDist;

        float pAx = x1 + dirX * dA;
        float pAz = z1 + dirZ * dA;
        float pBx = x1 + dirX * dB;
        float pBz = z1 + dirZ * dB;

        float uA = dA / texTile;
        float uB = dB / texTile;
        float v0 = 0.0f;
        float v1 = h / texTile;

        // Side 1
        glNormal3f(nx1, 0.0f, nz1);
        glTexCoord2f(uA, v0); glVertex3f(pAx, 0.0f, pAz);
        glTexCoord2f(uB, v0); glVertex3f(pBx, 0.0f, pBz);
        glTexCoord2f(uB, v1); glVertex3f(pBx, h,    pBz);
        glTexCoord2f(uA, v1); glVertex3f(pAx, h,    pAz);

        // Side 2 (opposite face)
        glNormal3f(nx2, 0.0f, nz2);
        glTexCoord2f(uB, v0); glVertex3f(pBx, 0.0f, pBz);
        glTexCoord2f(uA, v0); glVertex3f(pAx, 0.0f, pAz);
        glTexCoord2f(uA, v1); glVertex3f(pAx, h,    pAz);
        glTexCoord2f(uB, v1); glVertex3f(pBx, h,    pBz);
    }
}

// Textured doorway header (y = 3.0 to y = h)
static void drawTexturedDoorTop(float x1, float z1, float x2, float z2, float h, float texTile = 2.5f) {
    float dx = x2 - x1;
    float dz = z2 - z1;
    float len = std::sqrt(dx * dx + dz * dz);
    if (len < 0.001f) return;

    float dirX = dx / len;
    float dirZ = dz / len;
    float nx1 = -dirZ, nz1 = dirX;
    float nx2 = dirZ, nz2 = -dirX;

    float yBottom = 3.0f;
    float u0 = 0.0f, u1 = len / texTile;
    float v0 = yBottom / texTile, v1 = h / texTile;

    // Side 1
    glNormal3f(nx1, 0.0f, nz1);
    glTexCoord2f(u0, v0); glVertex3f(x1, yBottom, z1);
    glTexCoord2f(u1, v0); glVertex3f(x2, yBottom, z2);
    glTexCoord2f(u1, v1); glVertex3f(x2, h,       z2);
    glTexCoord2f(u0, v1); glVertex3f(x1, h,       z1);

    // Side 2
    glNormal3f(nx2, 0.0f, nz2);
    glTexCoord2f(u1, v0); glVertex3f(x2, yBottom, z2);
    glTexCoord2f(u0, v0); glVertex3f(x1, yBottom, z1);
    glTexCoord2f(u0, v1); glVertex3f(x1, h,       z1);
    glTexCoord2f(u1, v1); glVertex3f(x2, h,       z2);

    // Soffit underside (facing down at y = 3.0m)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(u0, 0.0f); glVertex3f(x1, yBottom, z1);
    glTexCoord2f(u1, 0.0f); glVertex3f(x2, yBottom, z2);
    glTexCoord2f(u1, 0.1f); glVertex3f(x2, yBottom, z2);
    glTexCoord2f(u0, 0.1f); glVertex3f(x1, yBottom, z1);
}

void Museum::drawWalls() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texWallPlaster);

    // Subtle off-white modern gallery plaster material
    GLfloat mat_ambient[]  = { 0.38f, 0.38f, 0.38f, 1.0f };
    GLfloat mat_diffuse[]  = { 0.92f, 0.91f, 0.89f, 1.0f };
    GLfloat mat_specular[] = { 0.06f, 0.06f, 0.06f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0f);
    
    float h = 4.0f;
    
    glBegin(GL_QUADS);
    
    // Outer boundary walls
    drawTexturedWallBlock(-20, -20, 20, -20, h); // Back
    drawTexturedWallBlock(-20, 5, 20, 5, h);     // Front
    drawTexturedWallBlock(-20, 5, -20, -20, h);  // Left
    drawTexturedWallBlock(20, -20, 20, 5, h);    // Right
    
    // Inner dividing walls
    // Z-axis walls (X = -5)
    drawTexturedWallBlock(-5, 5, -5, 2, h);
    drawTexturedDoorTop(-5, 2, -5, -2, h);
    drawTexturedWallBlock(-5, -2, -5, -5, h);
    
    drawTexturedWallBlock(-5, -5, -5, -10, h);
    drawTexturedDoorTop(-5, -10, -5, -15, h);
    drawTexturedWallBlock(-5, -15, -5, -20, h);
    
    // Z-axis walls (X = 5)
    drawTexturedWallBlock(5, 5, 5, 2, h);
    drawTexturedDoorTop(5, 2, 5, -2, h);
    drawTexturedWallBlock(5, -2, 5, -5, h);
    
    drawTexturedWallBlock(5, -5, 5, -10, h);
    drawTexturedDoorTop(5, -10, 5, -15, h);
    drawTexturedWallBlock(5, -15, 5, -20, h);
    
    // X-axis walls (Z = -5)
    drawTexturedWallBlock(-20, -5, -15, -5, h);
    drawTexturedDoorTop(-15, -5, -10, -5, h);
    drawTexturedWallBlock(-10, -5, -5, -5, h);
    
    drawTexturedWallBlock(-5, -5, -2, -5, h);
    drawTexturedDoorTop(-2, -5, 2, -5, h);
    drawTexturedWallBlock(2, -5, 5, -5, h);
    
    drawTexturedWallBlock(5, -5, 10, -5, h);
    drawTexturedDoorTop(10, -5, 15, -5, h);
    drawTexturedWallBlock(15, -5, 20, -5, h);
    
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Helper to draw 3D dark wood baseboard (skirting board) along a wall
static void drawBaseboardSegment(float x1, float z1, float x2, float z2, int sideMode, bool capStart, bool capEnd) {
    float dx = x2 - x1;
    float dz = z2 - z1;
    float len = std::sqrt(dx * dx + dz * dz);
    if (len < 0.001f) return;

    float dirX = dx / len;
    float dirZ = dz / len;
    float normX = -dirZ;
    float normZ = dirX;

    const float hb = 0.16f;  // Height of baseboard
    const float pb = 0.035f; // Protrusion out from wall
    const float texScale = 1.0f; // Wood texture repeat
    float uLen = len / texScale;

    // Side 1 (facing +normal)
    if (sideMode == 1 || sideMode == 3) {
        float ox = normX * pb;
        float oz = normZ * pb;

        // Vertical front face
        glNormal3f(normX, 0.0f, normZ);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1 + ox, 0.0f, z1 + oz);
        glTexCoord2f(uLen, 0.0f); glVertex3f(x2 + ox, 0.0f, z2 + oz);
        glTexCoord2f(uLen, 1.0f); glVertex3f(x2 + ox, hb,   z2 + oz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1 + ox, hb,   z1 + oz);

        // Top bevel returning to wall face at y = hb
        glNormal3f(normX * 0.707f, 0.707f, normZ * 0.707f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1 + ox, hb, z1 + oz);
        glTexCoord2f(uLen, 0.0f); glVertex3f(x2 + ox, hb, z2 + oz);
        glTexCoord2f(uLen, 0.3f); glVertex3f(x2,      hb, z2);
        glTexCoord2f(0.0f, 0.3f); glVertex3f(x1,      hb, z1);

        // End caps at doorway edges
        if (capStart) {
            glNormal3f(-dirX, 0.0f, -dirZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x1,      0.0f, z1);
            glTexCoord2f(0.3f, 0.0f); glVertex3f(x1 + ox, 0.0f, z1 + oz);
            glTexCoord2f(0.3f, 1.0f); glVertex3f(x1 + ox, hb,   z1 + oz);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x1,      hb,   z1);
        }
        if (capEnd) {
            glNormal3f(dirX, 0.0f, dirZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x2 + ox, 0.0f, z2 + oz);
            glTexCoord2f(0.3f, 0.0f); glVertex3f(x2,      0.0f, z2);
            glTexCoord2f(0.3f, 1.0f); glVertex3f(x2,      hb,   z2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x2 + ox, hb,   z2 + oz);
        }
    }

    // Side 2 (facing -normal)
    if (sideMode == 2 || sideMode == 3) {
        float ox = -normX * pb;
        float oz = -normZ * pb;

        // Vertical front face
        glNormal3f(-normX, 0.0f, -normZ);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x2 + ox, 0.0f, z2 + oz);
        glTexCoord2f(uLen, 0.0f); glVertex3f(x1 + ox, 0.0f, z1 + oz);
        glTexCoord2f(uLen, 1.0f); glVertex3f(x1 + ox, hb,   z1 + oz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x2 + ox, hb,   z2 + oz);

        // Top bevel returning to wall face at y = hb
        glNormal3f(-normX * 0.707f, 0.707f, -normZ * 0.707f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x2 + ox, hb, z2 + oz);
        glTexCoord2f(uLen, 0.0f); glVertex3f(x1 + ox, hb, z1 + oz);
        glTexCoord2f(uLen, 0.3f); glVertex3f(x1,      hb, z1);
        glTexCoord2f(0.0f, 0.3f); glVertex3f(x2,      hb, z2);

        // End caps at doorway edges
        if (capStart) {
            glNormal3f(-dirX, 0.0f, -dirZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x1 + ox, 0.0f, z1 + oz);
            glTexCoord2f(0.3f, 0.0f); glVertex3f(x1,      0.0f, z1);
            glTexCoord2f(0.3f, 1.0f); glVertex3f(x1,      hb,   z1);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x1 + ox, hb,   z1 + oz);
        }
        if (capEnd) {
            glNormal3f(dirX, 0.0f, dirZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x2,      0.0f, z2);
            glTexCoord2f(0.3f, 0.0f); glVertex3f(x2 + ox, 0.0f, z2 + oz);
            glTexCoord2f(0.3f, 1.0f); glVertex3f(x2 + ox, hb,   z2 + oz);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x2,      hb,   z2);
        }
    }
}

void Museum::drawBaseboards() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texBaseboard);

    // Dark wood material for baseboards
    GLfloat mat_ambient[]  = { 0.28f, 0.20f, 0.14f, 1.0f };
    GLfloat mat_diffuse[]  = { 0.75f, 0.70f, 0.65f, 1.0f };
    GLfloat mat_specular[] = { 0.40f, 0.32f, 0.25f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0f);

    glBegin(GL_QUADS);

    // Outer boundary walls (only interior face)
    drawBaseboardSegment(-20, -20, 20, -20, 1, false, false); // Back (facing +Z)
    drawBaseboardSegment(-20, 5, 20, 5, 2, false, false);     // Front (facing -Z)
    drawBaseboardSegment(-20, 5, -20, -20, 1, false, false);  // Left (facing +X)
    drawBaseboardSegment(20, -20, 20, 5, 1, false, false);    // Right (facing -X)

    // Inner walls (both sides, with end caps where meeting doorways)
    // Z-axis walls (X = -5)
    drawBaseboardSegment(-5, 5, -5, 2, 3, false, true);
    drawBaseboardSegment(-5, -2, -5, -5, 3, true, false);

    drawBaseboardSegment(-5, -5, -5, -10, 3, false, true);
    drawBaseboardSegment(-5, -15, -5, -20, 3, true, false);

    // Z-axis walls (X = 5)
    drawBaseboardSegment(5, 5, 5, 2, 3, false, true);
    drawBaseboardSegment(5, -2, 5, -5, 3, true, false);

    drawBaseboardSegment(5, -5, 5, -10, 3, false, true);
    drawBaseboardSegment(5, -15, 5, -20, 3, true, false);

    // X-axis walls (Z = -5)
    drawBaseboardSegment(-20, -5, -15, -5, 3, false, true);
    drawBaseboardSegment(-10, -5, -5, -5, 3, true, false);

    drawBaseboardSegment(-5, -5, -2, -5, 3, false, true);
    drawBaseboardSegment(2, -5, 5, -5, 3, true, false);

    drawBaseboardSegment(5, -5, 10, -5, 3, false, true);
    drawBaseboardSegment(15, -5, 20, -5, 3, true, false);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Museum::drawCeilings() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texWallPlaster);

    GLfloat mat_ambient[]  = { 0.30f, 0.30f, 0.30f, 1.0f };
    GLfloat mat_diffuse[]  = { 0.92f, 0.92f, 0.92f, 1.0f };
    GLfloat mat_specular[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0f);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE); // Ceiling must never be culled

    const float minX = -20.0f, maxX = 20.0f;
    const float minZ = -20.0f, maxZ = 5.0f;
    const float step = 2.0f;
    const float tileSize = 3.0f;

    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f); // Normal pointing down
    for (float x = minX; x < maxX; x += step) {
        float xNext = std::min(x + step, maxX);
        for (float z = minZ; z < maxZ; z += step) {
            float zNext = std::min(z + step, maxZ);

            // Counter-clockwise winding viewed from below (+Y looking up, normal -Y)
            glTexCoord2f(x / tileSize, z / tileSize);
            glVertex3f(x, 4.0f, z);

            glTexCoord2f(xNext / tileSize, z / tileSize);
            glVertex3f(xNext, 4.0f, z);

            glTexCoord2f(xNext / tileSize, zNext / tileSize);
            glVertex3f(xNext, 4.0f, zNext);

            glTexCoord2f(x / tileSize, zNext / tileSize);
            glVertex3f(x, 4.0f, zNext);
        }
    }
    glEnd();

    glEnable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}

