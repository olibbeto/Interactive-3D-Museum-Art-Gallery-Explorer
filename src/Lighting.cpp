#include "Lighting.h"
#include <GL/freeglut.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Light positions for the 6 museum rooms
static const GLfloat g_lightPositions[6][4] = {
    {   0.0f, 3.85f,   0.0f, 1.0f }, // 0: Main Hall
    { -12.5f, 3.85f,   0.0f, 1.0f }, // 1: Modern Gallery
    {  12.5f, 3.85f,   0.0f, 1.0f }, // 2: Classical Gallery
    {   0.0f, 3.85f, -12.5f, 1.0f }, // 3: Sculpture Gallery
    { -12.5f, 3.85f, -12.5f, 1.0f }, // 4: Impressionist Gallery
    {  12.5f, 3.85f, -12.5f, 1.0f }  // 5: Renaissance Gallery
};

// Fixture light colors (RGB)
static const float g_lightColors[6][3] = {
    { 0.95f, 0.92f, 0.85f }, // Main Hall: Warm neutral
    { 0.70f, 0.82f, 1.00f }, // Modern: Cool daylight
    { 1.00f, 0.88f, 0.65f }, // Classical: Warm gold
    { 0.92f, 0.92f, 0.95f }, // Sculpture: Crisp gallery white
    { 0.95f, 0.80f, 0.72f }, // Impressionist: Warm soft sunset
    { 0.98f, 0.86f, 0.60f }  // Renaissance: Amber gold
};

Lighting::Lighting() : enabled(true) {}

void Lighting::init() {
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    
    // Ambient light
    GLfloat global_ambient[] = { 0.22f, 0.22f, 0.22f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    
    GLfloat spec[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    for (int i = 0; i < 6; ++i) {
        GLenum lightEnum = GL_LIGHT0 + i;
        GLfloat diff[4] = { g_lightColors[i][0] * 0.85f, g_lightColors[i][1] * 0.85f, g_lightColors[i][2] * 0.85f, 1.0f };
        GLfloat amb[4]  = { g_lightColors[i][0] * 0.12f, g_lightColors[i][1] * 0.12f, g_lightColors[i][2] * 0.12f, 1.0f };

        glLightfv(lightEnum, GL_AMBIENT, amb);
        glLightfv(lightEnum, GL_DIFFUSE, diff);
        glLightfv(lightEnum, GL_SPECULAR, spec);

        // Realistic distance attenuation
        glLightf(lightEnum, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(lightEnum, GL_LINEAR_ATTENUATION, 0.04f);
        glLightf(lightEnum, GL_QUADRATIC_ATTENUATION, 0.005f);

        if (enabled) {
            glEnable(lightEnum);
        }
    }
}

void Lighting::apply() {
    if (!enabled) return;
    for (int i = 0; i < 6; ++i) {
        glLightfv(GL_LIGHT0 + i, GL_POSITION, g_lightPositions[i]);
    }
}

void Lighting::toggle() {
    enabled = !enabled;
    if (enabled) {
        glEnable(GL_LIGHTING);
        for (int i = 0; i < 6; ++i) {
            glEnable(GL_LIGHT0 + i);
        }
    } else {
        glDisable(GL_LIGHTING);
        for (int i = 0; i < 6; ++i) {
            glDisable(GL_LIGHT0 + i);
        }
    }
}

const float* Lighting::getLightPos(int index) const {
    if (index >= 0 && index < 6) {
        return g_lightPositions[index];
    }
    return g_lightPositions[0];
}

// Draw a single spotlight canister with mounting stem, casing, and glowing emissive lens
static void drawSpotlight(float x, float y, float z, float rotY, float pitch, const float col[3], bool on) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotY, 0, 1, 0);
    glRotatef(pitch, 1, 0, 0);

    // Matte black casing material
    GLfloat case_amb[]  = { 0.08f, 0.08f, 0.09f, 1.0f };
    GLfloat case_diff[] = { 0.12f, 0.12f, 0.13f, 1.0f };
    GLfloat case_spec[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat zero_em[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, case_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, case_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, case_spec);
    glMaterialfv(GL_FRONT, GL_EMISSION, zero_em);
    glMaterialf(GL_FRONT, GL_SHININESS, 30.0f);

    // Swivel stem to ceiling track
    glPushMatrix();
    glTranslatef(0.0f, 0.07f, 0.0f);
    glScalef(0.025f, 0.14f, 0.025f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cylindrical canister body
    const int segments = 16;
    const float radius = 0.075f;
    const float len = 0.22f;

    // Body sides
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * (float)M_PI / segments;
        float ca = std::cos(angle), sa = std::sin(angle);
        glNormal3f(ca, 0.0f, sa);
        glVertex3f(ca * radius, 0.0f, sa * radius);
        glVertex3f(ca * radius, -len, sa * radius);
    }
    glEnd();

    // Top cap of canister
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * (float)M_PI / segments;
        glVertex3f(std::cos(angle) * radius, 0.0f, std::sin(angle) * radius);
    }
    glEnd();

    // Recessed glowing lens at bottom face
    float lensY = -len + 0.005f;
    if (on) {
        GLfloat lens_em[] = { col[0] * 1.5f, col[1] * 1.5f, col[2] * 1.5f, 1.0f };
        GLfloat lens_diff[] = { col[0], col[1], col[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, lens_em);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, lens_diff);
    } else {
        GLfloat off_mat[] = { 0.15f, 0.15f, 0.15f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, zero_em);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, off_mat);
    }

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, lensY, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float angle = -i * 2.0f * (float)M_PI / segments;
        glVertex3f(std::cos(angle) * (radius - 0.015f), lensY, std::sin(angle) * (radius - 0.015f));
    }
    glEnd();

    // Reset emission
    glMaterialfv(GL_FRONT, GL_EMISSION, zero_em);

    // Glowing halo bloom effect when active
    if (on) {
        glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive bloom
        glDepthMask(GL_FALSE);

        const float bloomR = 0.28f;
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(col[0], col[1], col[2], 0.40f);
        glVertex3f(0.0f, lensY - 0.02f, 0.0f);
        glColor4f(col[0], col[1], col[2], 0.0f);
        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2.0f * (float)M_PI / segments;
            glVertex3f(std::cos(angle) * bloomR, lensY - 0.02f, std::sin(angle) * bloomR);
        }
        glEnd();

        glPopAttrib();
    }

    glPopMatrix();
}

// Draw a ceiling track lighting rail with modern dual bars, brackets, and spotlights
static void drawCeilingTrack(float cx, float cz, bool alongX, float length, const float col[3], bool on) {
    const float trackY = 3.94f;
    const float halfLen = length * 0.5f;

    // Track rail material: extruded matte black aluminum
    GLfloat rail_amb[]  = { 0.10f, 0.10f, 0.12f, 1.0f };
    GLfloat rail_diff[] = { 0.15f, 0.15f, 0.18f, 1.0f };
    GLfloat rail_spec[] = { 0.40f, 0.40f, 0.45f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, rail_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, rail_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, rail_spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 40.0f);

    // Dual parallel rails
    const float railDist = 0.06f; // Separation between dual tracks
    if (alongX) {
        // Rail 1
        glPushMatrix();
        glTranslatef(cx, trackY, cz - railDist);
        glScalef(length, 0.03f, 0.025f);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Rail 2
        glPushMatrix();
        glTranslatef(cx, trackY, cz + railDist);
        glScalef(length, 0.03f, 0.025f);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Vertical suspension rods anchoring track to ceiling (y = 4.0m)
        for (float dx = -halfLen + 0.5f; dx <= halfLen - 0.5f; dx += 2.0f) {
            glPushMatrix();
            glTranslatef(cx + dx, 3.97f, cz);
            glScalef(0.03f, 0.06f, railDist * 2.2f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }

        // Spotlights distributed along the track
        float spots[4] = { -halfLen * 0.65f, -halfLen * 0.2f, halfLen * 0.2f, halfLen * 0.65f };
        for (int i = 0; i < 4; ++i) {
            float rot = (i % 2 == 0) ? 0.0f : 180.0f;
            float pitch = 35.0f;
            drawSpotlight(cx + spots[i], trackY - 0.02f, cz, rot, pitch, col, on);
        }
    } else {
        // Rail 1
        glPushMatrix();
        glTranslatef(cx - railDist, trackY, cz);
        glScalef(0.025f, 0.03f, length);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Rail 2
        glPushMatrix();
        glTranslatef(cx + railDist, trackY, cz);
        glScalef(0.025f, 0.03f, length);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Vertical suspension rods anchoring to ceiling
        for (float dz = -halfLen + 0.5f; dz <= halfLen - 0.5f; dz += 2.0f) {
            glPushMatrix();
            glTranslatef(cx, 3.97f, cz + dz);
            glScalef(railDist * 2.2f, 0.06f, 0.03f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }

        // Spotlights distributed along the track
        float spots[4] = { -halfLen * 0.65f, -halfLen * 0.2f, halfLen * 0.2f, halfLen * 0.65f };
        for (int i = 0; i < 4; ++i) {
            float rot = (i % 2 == 0) ? 90.0f : -90.0f;
            float pitch = 35.0f;
            drawSpotlight(cx, trackY - 0.02f, cz + spots[i], rot, pitch, col, on);
        }
    }
}

void Lighting::drawFixtures() const {
    glDisable(GL_TEXTURE_2D);

    // Track lighting system in each of the 6 galleries
    // 0: Main Hall: central track along Z
    drawCeilingTrack(0.0f, 0.0f, false, 6.0f, g_lightColors[0], enabled);

    // 1: Modern Gallery: track along X
    drawCeilingTrack(-12.5f, 0.0f, true, 8.0f, g_lightColors[1], enabled);

    // 2: Classical Gallery: track along X
    drawCeilingTrack(12.5f, 0.0f, true, 8.0f, g_lightColors[2], enabled);

    // 3: Sculpture Gallery: track along X
    drawCeilingTrack(0.0f, -12.5f, true, 6.0f, g_lightColors[3], enabled);

    // 4: Impressionist Gallery: track along X
    drawCeilingTrack(-12.5f, -12.5f, true, 8.0f, g_lightColors[4], enabled);

    // 5: Renaissance Gallery: track along X
    drawCeilingTrack(12.5f, -12.5f, true, 8.0f, g_lightColors[5], enabled);
}

