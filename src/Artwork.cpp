#include "Artwork.h"
#include <GL/freeglut.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

unsigned int Artwork::texFrameGold = 0;
unsigned int Artwork::texFrameWood = 0;

void Artwork::setFrameTextures(unsigned int goldTex, unsigned int woodTex) {
    texFrameGold = goldTex;
    texFrameWood = woodTex;
}

Artwork::Artwork(float x, float y, float z, float width, float height, float rotY, 
                 int style, const ArtInfo& info, unsigned int textureID, FrameType frameType)
    : x(x), y(y), z(z), width(width), height(height), rotY(rotY), style(style), info(info), 
      interactionRadius(3.5f), textureID(textureID), frameType(frameType) {}

void Artwork::draw() const {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotY, 0, 1, 0);
    
    drawFrame();
    drawCanvas();
    
    glPopMatrix();
}

void Artwork::drawFrame() const {
    float bw = 0.16f; // Moulding border width
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    float ow = hw + bw;
    float oh = hh + bw;

    float zBack = 0.0f;
    float zFront = 0.095f;
    float zInner = 0.076f;

    unsigned int frameTex = (frameType == FRAME_GOLD) ? texFrameGold : texFrameWood;

    if (frameTex > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, frameTex);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    if (frameType == FRAME_GOLD) {
        GLfloat gold_ambient[]  = { 0.35f, 0.28f, 0.14f, 1.0f };
        GLfloat gold_diffuse[]  = { 0.95f, 0.82f, 0.45f, 1.0f };
        GLfloat gold_specular[] = { 0.95f, 0.85f, 0.40f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT, gold_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, gold_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, gold_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
    } else {
        GLfloat wood_ambient[]  = { 0.25f, 0.18f, 0.12f, 1.0f };
        GLfloat wood_diffuse[]  = { 0.85f, 0.78f, 0.72f, 1.0f };
        GLfloat wood_specular[] = { 0.35f, 0.28f, 0.22f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT, wood_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, wood_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, wood_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 28.0f);
    }

    float repeatH = (ow * 2.0f) / 0.4f;
    float repeatV = (oh * 2.0f) / 0.4f;

    glBegin(GL_QUADS);

    // ==================== FRONT MOULDING FACES (facing +Z) ====================
    glNormal3f(0.0f, 0.0f, 1.0f);

    // Top rail
    glTexCoord2f(0.0f, 0.0f);     glVertex3f(-ow, hh, zFront);
    glTexCoord2f(repeatH, 0.0f);  glVertex3f(ow, hh, zFront);
    glTexCoord2f(repeatH, 1.0f);  glVertex3f(ow, oh, zFront);
    glTexCoord2f(0.0f, 1.0f);     glVertex3f(-ow, oh, zFront);

    // Bottom rail
    glTexCoord2f(0.0f, 0.0f);     glVertex3f(-ow, -oh, zFront);
    glTexCoord2f(repeatH, 0.0f);  glVertex3f(ow, -oh, zFront);
    glTexCoord2f(repeatH, 1.0f);  glVertex3f(ow, -hh, zFront);
    glTexCoord2f(0.0f, 1.0f);     glVertex3f(-ow, -hh, zFront);

    // Left stile
    glTexCoord2f(0.0f, 0.0f);     glVertex3f(-ow, -hh, zFront);
    glTexCoord2f(1.0f, 0.0f);     glVertex3f(-hw, -hh, zFront);
    glTexCoord2f(1.0f, repeatV);  glVertex3f(-hw, hh, zFront);
    glTexCoord2f(0.0f, repeatV);  glVertex3f(-ow, hh, zFront);

    // Right stile
    glTexCoord2f(0.0f, 0.0f);     glVertex3f(hw, -hh, zFront);
    glTexCoord2f(1.0f, 0.0f);     glVertex3f(ow, -hh, zFront);
    glTexCoord2f(1.0f, repeatV);  glVertex3f(ow, hh, zFront);
    glTexCoord2f(0.0f, repeatV);  glVertex3f(hw, hh, zFront);

    // ==================== INNER BEVEL / RABBET (sloping to canvas) ====================
    // Top inner bevel
    glNormal3f(0.0f, -0.707f, 0.707f);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(-hw, hh, zFront);
    glTexCoord2f(repeatH, 0.0f); glVertex3f(hw, hh, zFront);
    glTexCoord2f(repeatH, 0.2f); glVertex3f(hw, hh, zInner);
    glTexCoord2f(0.0f, 0.2f);    glVertex3f(-hw, hh, zInner);

    // Bottom inner bevel
    glNormal3f(0.0f, 0.707f, 0.707f);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(-hw, -hh, zInner);
    glTexCoord2f(repeatH, 0.0f); glVertex3f(hw, -hh, zInner);
    glTexCoord2f(repeatH, 0.2f); glVertex3f(hw, -hh, zFront);
    glTexCoord2f(0.0f, 0.2f);    glVertex3f(-hw, -hh, zFront);

    // Left inner bevel
    glNormal3f(0.707f, 0.0f, 0.707f);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(-hw, -hh, zFront);
    glTexCoord2f(0.2f, 0.0f);    glVertex3f(-hw, -hh, zInner);
    glTexCoord2f(0.2f, repeatV); glVertex3f(-hw, hh, zInner);
    glTexCoord2f(0.0f, repeatV); glVertex3f(-hw, hh, zFront);

    // Right inner bevel
    glNormal3f(-0.707f, 0.0f, 0.707f);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(hw, -hh, zInner);
    glTexCoord2f(0.2f, 0.0f);    glVertex3f(hw, -hh, zFront);
    glTexCoord2f(0.2f, repeatV); glVertex3f(hw, hh, zFront);
    glTexCoord2f(0.0f, repeatV); glVertex3f(hw, hh, zInner);

    // ==================== OUTER SIDES (extending back to wall) ====================
    // Top outer side
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(-ow, oh, zBack);
    glTexCoord2f(repeatH, 0.0f); glVertex3f(ow, oh, zBack);
    glTexCoord2f(repeatH, 0.3f); glVertex3f(ow, oh, zFront);
    glTexCoord2f(0.0f, 0.3f);    glVertex3f(-ow, oh, zFront);

    // Bottom outer side
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(-ow, -oh, zFront);
    glTexCoord2f(repeatH, 0.0f); glVertex3f(ow, -oh, zFront);
    glTexCoord2f(repeatH, 0.3f); glVertex3f(ow, -oh, zBack);
    glTexCoord2f(0.0f, 0.3f);    glVertex3f(-ow, -oh, zBack);

    // Left outer side
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(-ow, -oh, zBack);
    glTexCoord2f(0.3f, 0.0f);    glVertex3f(-ow, -oh, zFront);
    glTexCoord2f(0.3f, repeatV); glVertex3f(-ow, oh, zFront);
    glTexCoord2f(0.0f, repeatV); glVertex3f(-ow, oh, zBack);

    // Right outer side
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);    glVertex3f(ow, -oh, zFront);
    glTexCoord2f(0.3f, 0.0f);    glVertex3f(ow, -oh, zBack);
    glTexCoord2f(0.3f, repeatV); glVertex3f(ow, oh, zBack);
    glTexCoord2f(0.0f, repeatV); glVertex3f(ow, oh, zFront);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Artwork::drawCanvas() const {
    // Darker canvas edge to simulate shadow/depth
    GLfloat mat_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    
    float zOffset = 0.081f; 
    
    if (textureID > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        // We use full white diffuse so texture colors are preserved
        GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2, -height/2, zOffset);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(width/2, -height/2, zOffset);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(width/2, height/2, zOffset);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2, zOffset);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    } else {
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-width/2, -height/2, zOffset);
        glVertex3f(width/2, -height/2, zOffset);
        glVertex3f(width/2, height/2, zOffset);
        glVertex3f(-width/2, height/2, zOffset);
        glEnd();
        
        glPushMatrix();
        glTranslatef(0, 0, zOffset + 0.02f); // Increased distance from canvas to prevent z-fighting
        generateProceduralArt();
        glPopMatrix();
    }
}

void Artwork::generateProceduralArt() const {
    glDisable(GL_LIGHTING); 
    
    // We use glTranslatef(0,0,0.01f) between overlapping layers 
    // to completely prevent Z-fighting flickering when looking from afar.

    if (style == 0) { // Sunset Gradient with Sun
        glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.4f); glVertex3f(-width/2, height/2, 0);
        glColor3f(0.1f, 0.1f, 0.4f); glVertex3f(width/2, height/2, 0); 
        glColor3f(0.8f, 0.3f, 0.1f); glVertex3f(width/2, -height/2, 0);
        glColor3f(0.8f, 0.3f, 0.1f); glVertex3f(-width/2, -height/2, 0);
        glEnd();
        
        glTranslatef(0, 0, 0.01f); // Next layer
        // Sun
        glColor3f(1.0f, 0.8f, 0.2f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, -height/4, 0);
        for(int i = 0; i <= 30; i++) {
            float angle = i * 2.0f * M_PI / 30.0f;
            glVertex3f(cos(angle) * width/4, -height/4 + sin(angle) * width/4, 0);
        }
        glEnd();
    } 
    else if (style == 1) { // Trippy Geometry
        glColor3f(0.05f, 0.05f, 0.05f);
        glBegin(GL_QUADS);
        glVertex3f(-width/2, -height/2, 0); glVertex3f(width/2, -height/2, 0);
        glVertex3f(width/2, height/2, 0); glVertex3f(-width/2, height/2, 0);
        glEnd();
        
        glLineWidth(2.0f);
        for(float s = 1.0f; s > 0.1f; s -= 0.15f) {
            glTranslatef(0, 0, 0.005f); // Step forward for each loop
            glBegin(GL_LINE_LOOP);
            glColor3f(s, 1.0f - s, 0.8f);
            glVertex3f(-width/2 * s, -height/2 * s, 0);
            glVertex3f(width/2 * s, -height/2 * s, 0);
            glVertex3f(width/2 * s, height/2 * s, 0);
            glVertex3f(-width/2 * s, height/2 * s, 0);
            glEnd();
            
            glTranslatef(0, 0, 0.005f);
            glBegin(GL_LINE_LOOP);
            glColor3f(1.0f - s, s, 0.5f);
            glVertex3f(0, -height/2 * s, 0);
            glVertex3f(width/2 * s, 0, 0);
            glVertex3f(0, height/2 * s, 0);
            glVertex3f(-width/2 * s, 0, 0);
            glEnd();
        }
        glLineWidth(1.0f);
    } 
    else if (style == 2) { // Minimalist Circles
        glColor3f(0.9f, 0.9f, 0.85f); 
        glBegin(GL_QUADS);
        glVertex3f(-width/2, -height/2, 0); glVertex3f(width/2, -height/2, 0);
        glVertex3f(width/2, height/2, 0); glVertex3f(-width/2, height/2, 0);
        glEnd();
        
        glTranslatef(0, 0, 0.01f);
        glColor3f(0.8f, 0.1f, 0.1f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(width/4, height/4, 0);
        for(int i = 0; i <= 40; i++) {
            float angle = i * 2.0f * M_PI / 40.0f;
            glVertex3f(width/4 + cos(angle) * width/3, height/4 + sin(angle) * width/3, 0);
        }
        glEnd();
        
        glTranslatef(0, 0, 0.01f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(4.0f);
        glBegin(GL_LINES);
        glVertex3f(-width/2, -height/3, 0);
        glVertex3f(width/2, height/3, 0);
        glEnd();
        glLineWidth(1.0f);
    } 
    else if (style == 4) { // Night Mountains
        glBegin(GL_QUADS);
        glColor3f(0.05f, 0.05f, 0.2f); glVertex3f(-width/2, -height/2, 0); glVertex3f(width/2, -height/2, 0);
        glColor3f(0.0f, 0.0f, 0.05f); glVertex3f(width/2, height/2, 0); glVertex3f(-width/2, height/2, 0);
        glEnd();
        
        glTranslatef(0, 0, 0.01f);
        glColor3f(0.9f, 0.9f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(-width/4, height/4, 0);
        for(int i = 0; i <= 20; i++) {
            float angle = i * 2.0f * M_PI / 20.0f;
            glVertex3f(-width/4 + cos(angle) * width/8, height/4 + sin(angle) * width/8, 0);
        }
        glEnd();
        
        glTranslatef(0, 0, 0.01f);
        glColor3f(0.1f, 0.2f, 0.3f);
        glBegin(GL_TRIANGLES);
        glVertex3f(-width/2, -height/2, 0); glVertex3f(0, height/8, 0); glVertex3f(width/4, -height/2, 0);
        glColor3f(0.15f, 0.25f, 0.35f);
        glVertex3f(-width/4, -height/2, 0); glVertex3f(width/3, height/4, 0); glVertex3f(width/2, -height/2, 0);
        glEnd();
    }
    else if (style == 5) { // Modern Minimalism Squares
        glColor3f(0.85f, 0.85f, 0.85f);
        glBegin(GL_QUADS);
        glVertex3f(-width/2, -height/2, 0); glVertex3f(width/2, -height/2, 0);
        glVertex3f(width/2, height/2, 0); glVertex3f(-width/2, height/2, 0);
        glEnd();
        
        glTranslatef(0, 0, 0.01f);
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.2f, 0.2f); // Black square
        glVertex3f(-width/4, -height/4, 0); glVertex3f(0, -height/4, 0);
        glVertex3f(0, 0, 0); glVertex3f(-width/4, 0, 0);
        
        glColor3f(0.7f, 0.2f, 0.2f); // Red square
        glVertex3f(width/8, height/8, 0); glVertex3f(width/3, height/8, 0);
        glVertex3f(width/3, height/3, 0); glVertex3f(width/8, height/3, 0);
        glEnd();
        
        glTranslatef(0, 0, 0.01f);
        glColor3f(0.1f, 0.1f, 0.1f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex3f(-width/2, 0, 0); glVertex3f(width/2, 0, 0);
        glVertex3f(0, -height/2, 0); glVertex3f(0, height/2, 0);
        glEnd();
        glLineWidth(1.0f);
    }
    else if (style == 6) { // Digital Geometry (Rings)
        glColor3f(0.1f, 0.1f, 0.1f);
        glBegin(GL_QUADS);
        glVertex3f(-width/2, -height/2, 0); glVertex3f(width/2, -height/2, 0);
        glVertex3f(width/2, height/2, 0); glVertex3f(-width/2, height/2, 0);
        glEnd();
        
        glLineWidth(3.0f);
        for(int r = 1; r <= 4; r++) {
            glTranslatef(0, 0, 0.01f);
            if (r%2 == 0) glColor3f(0.2f, 0.8f, 0.8f);
            else glColor3f(0.8f, 0.2f, 0.8f);
            
            glBegin(GL_LINE_LOOP);
            float rad = (width/2) * (r/5.0f);
            for(int i = 0; i < 30; i++) {
                float angle = i * 2.0f * M_PI / 30.0f;
                glVertex3f(cos(angle) * rad, sin(angle) * rad, 0);
            }
            glEnd();
        }
        glLineWidth(1.0f);
    }
    else if (style == 7) { // Abstract Portrait
        glBegin(GL_QUADS);
        glColor3f(0.9f, 0.8f, 0.7f); glVertex3f(-width/2, -height/2, 0); glVertex3f(width/2, -height/2, 0);
        glColor3f(0.7f, 0.6f, 0.5f); glVertex3f(width/2, height/2, 0); glVertex3f(-width/2, height/2, 0);
        glEnd();
        
        glTranslatef(0, 0, 0.01f);
        glColor3f(0.3f, 0.2f, 0.1f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for(int i = 0; i <= 20; i++) {
            float angle = i * 2.0f * M_PI / 20.0f;
            glVertex3f(cos(angle) * width/4, sin(angle) * height/3, 0);
        }
        glEnd();
        
        glTranslatef(0, 0, 0.01f);
        glColor3f(0.8f, 0.2f, 0.2f);
        glBegin(GL_TRIANGLES);
        glVertex3f(-width/8, height/8, 0); glVertex3f(0, -height/8, 0); glVertex3f(width/8, height/8, 0);
        glEnd();
    }
    else { // Vibrant Checker/Grid Abstract
        float stepX = width / 5.0f;
        float stepY = height / 5.0f;
        for(int x = 0; x < 5; x++) {
            for(int y = 0; y < 5; y++) {
                float px = -width/2 + x*stepX;
                float py = -height/2 + y*stepY;
                
                if((x+y)%2 == 0) glColor3f(0.2f, 0.6f, 0.8f);
                else glColor3f(0.1f, 0.2f, 0.4f);
                
                if (x == 2 && y == 2) glColor3f(0.9f, 0.8f, 0.1f); 
                
                // No Z-offset needed here because the tiles don't overlap each other
                glBegin(GL_QUADS);
                glVertex3f(px, py, 0);
                glVertex3f(px + stepX, py, 0);
                glVertex3f(px + stepX, py + stepY, 0);
                glVertex3f(px, py + stepY, 0);
                glEnd();
            }
        }
    }
    
    glEnable(GL_LIGHTING);
}

bool Artwork::isPlayerNear(float px, float pz) const {
    float dx = px - x;
    float dz = pz - z;
    return std::sqrt(dx*dx + dz*dz) < interactionRadius;
}
