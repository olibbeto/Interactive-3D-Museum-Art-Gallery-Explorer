#include "Sculpture.h"
#include <GL/freeglut.h>

Sculpture::Sculpture(float x, float y, float z, int type)
    : x(x), y(y), z(z), type(type), rotationAngle(0.0f) {}

void Sculpture::update(float deltaTime) {
    rotationAngle += 45.0f * deltaTime; // Rotate 45 degrees per second
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
}

void Sculpture::drawGeometry(bool isShadow) const {
    // Pedestal
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rotating sculpture on top
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

    if (type == 0) { // Twisted Torus
        glutSolidTorus(0.2, 0.5, 30, 30);
    } else if (type == 1) { // Stacked Spheres
        glutSolidSphere(0.4, 30, 30);
        glTranslatef(0.0f, 0.6f, 0.0f);
        glutSolidSphere(0.3, 30, 30);
        glTranslatef(0.0f, 0.5f, 0.0f);
        glutSolidSphere(0.2, 30, 30);
    } else { // Abstract wireframe and solid combo
        glPushMatrix();
        glRotatef(45.0f, 1.0f, 1.0f, 0.0f);
        glutSolidCube(0.6f);
        glPopMatrix();

        if (isShadow) {
            // For shadow, solid sphere silhouette looks crisp and realistic
            glutSolidSphere(0.68, 24, 24);
        } else {
            GLfloat scul_wire[] = { 0.8f, 0.8f, 0.2f, 1.0f };
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, scul_wire);
            glutWireSphere(0.7, 15, 15);
        }
    }
    glPopMatrix();
}

void Sculpture::draw() const {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Pedestal material (smooth gallery plinth)
    GLfloat ped_mat[] = { 0.85f, 0.85f, 0.85f, 1.0f };
    GLfloat ped_spec[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ped_mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ped_spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Sculpture material
    glTranslatef(0.0f, 1.5f, 0.0f);
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

    if (type == 0) {
        GLfloat scul_mat[] = { 0.85f, 0.25f, 0.25f, 1.0f };
        GLfloat scul_spec[] = { 0.6f, 0.6f, 0.6f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, scul_mat);
        glMaterialfv(GL_FRONT, GL_SPECULAR, scul_spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
        glutSolidTorus(0.2, 0.5, 30, 30);
    } else if (type == 1) {
        GLfloat scul_mat[] = { 0.25f, 0.85f, 0.35f, 1.0f };
        GLfloat scul_spec[] = { 0.6f, 0.6f, 0.6f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, scul_mat);
        glMaterialfv(GL_FRONT, GL_SPECULAR, scul_spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
        glutSolidSphere(0.4, 30, 30);
        glTranslatef(0.0f, 0.6f, 0.0f);
        glutSolidSphere(0.3, 30, 30);
        glTranslatef(0.0f, 0.5f, 0.0f);
        glutSolidSphere(0.2, 30, 30);
    } else {
        GLfloat scul_mat[] = { 0.25f, 0.35f, 0.90f, 1.0f };
        GLfloat scul_spec[] = { 0.7f, 0.7f, 0.7f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, scul_mat);
        glMaterialfv(GL_FRONT, GL_SPECULAR, scul_spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
        glPushMatrix();
        glRotatef(45.0f, 1.0f, 1.0f, 0.0f);
        glutSolidCube(0.6f);
        glPopMatrix();

        GLfloat scul_wire[] = { 0.95f, 0.85f, 0.20f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, scul_wire);
        glutWireSphere(0.7, 15, 15);
    }

    glPopMatrix();
}

static void buildShadowMatrix(float m[16], const float light[4], float planeD) {
    float A = 0.0f, B = 1.0f, C = 0.0f, D = planeD;
    float dot = A * light[0] + B * light[1] + C * light[2] + D * light[3];

    m[0]  = dot - light[0] * A;
    m[1]  = 0.0f - light[1] * A;
    m[2]  = 0.0f - light[2] * A;
    m[3]  = 0.0f - light[3] * A;

    m[4]  = 0.0f - light[0] * B;
    m[5]  = dot - light[1] * B;
    m[6]  = 0.0f - light[2] * B;
    m[7]  = 0.0f - light[3] * B;

    m[8]  = 0.0f - light[0] * C;
    m[9]  = 0.0f - light[1] * C;
    m[10] = dot - light[2] * C;
    m[11] = 0.0f - light[3] * C;

    m[12] = 0.0f - light[0] * D;
    m[13] = 0.0f - light[1] * D;
    m[14] = 0.0f - light[2] * D;
    m[15] = dot - light[3] * D;
}

void Sculpture::drawShadow(const float lightPos[4]) const {
    float shadowMat[16];
    buildShadowMatrix(shadowMat, lightPos, -0.003f);

    glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Prevent z-fighting with the floor geometry
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-2.0f, -2.0f);
    glDepthMask(GL_FALSE);

    // Natural dark soft translucent shadow
    glColor4f(0.04f, 0.04f, 0.06f, 0.55f);

    glPushMatrix();
    glMultMatrixf(shadowMat);
    glTranslatef(x, y, z);
    drawGeometry(true);
    glPopMatrix();

    glPopAttrib();
}

