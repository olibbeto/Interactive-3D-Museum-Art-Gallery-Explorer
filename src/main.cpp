#include <GL/freeglut.h>
#include <iostream>
#include "Camera.h"
#include "Museum.h"
#include "UI.h"
#include "Lighting.h"
#include "Input.h"

// Global instances
Camera camera(0.0f, 1.7f, 3.0f); // Player starts inside the museum, away from the back wall
Museum museum;
UI ui;
Lighting lighting;
Input input;

// Window size
int windowWidth = 1280;
int windowHeight = 720;

// Timing
int lastTime = 0;

// State
bool gameStarted = false;

void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE); // Back-face culling
    
    // Smooth shading
    glShadeModel(GL_SMOOTH);
    
    // Setup lighting
    lighting.init();
    
    // Initialize museum content
    museum.init();
    
    // Ignore key repeat to ensure smooth WASD movement
    glutIgnoreKeyRepeat(1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    if (!gameStarted) {
        ui.drawStartScreen(windowWidth, windowHeight);
        glutSwapBuffers();
        return;
    }
    
    // 3D Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth / (double)windowHeight, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply camera transformation
    camera.applyView();
    
    // Apply lighting before drawing objects
    lighting.apply();
    
    // Pass sculpture gallery light position for planar dynamic shadows
    const float* sculpLight = lighting.isEnabled() ? lighting.getLightPos(3) : nullptr;

    // Draw the museum architecture, artworks, and sculptures with dynamic planar shadows
    museum.draw(camera.getPosition(), sculpLight);

    // Draw glowing ceiling track lights and spotlights
    lighting.drawFixtures();
    
    // Draw 2D HUD
    ui.drawHUD(windowWidth, windowHeight, museum.getCurrentRoomName(), 
               museum.isPlayerNearInteractable(camera.getPosition()), 
               museum.getFloorStyle());
    
    // Draw interaction panel if active
    if (ui.isInteractionPanelOpen()) {
        ui.drawInteractionPanel(windowWidth, windowHeight, museum.getCurrentInteractableInfo());
    }
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

void update() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    
    if (gameStarted && !ui.isInteractionPanelOpen()) {
        camera.update(input, deltaTime, museum.getCollisionBoundaries());
    }
    
    museum.updateAnimations(deltaTime);
    
    glutPostRedisplay();
}

void keyboardDown(unsigned char key, int x, int y) {
    if (!gameStarted) {
        if (key == 13) { // ENTER
            gameStarted = true;
            // Center mouse when starting
            glutWarpPointer(windowWidth / 2, windowHeight / 2);
            glutSetCursor(GLUT_CURSOR_NONE);
        }
        return;
    }
    
    if (key == 27) { // ESC
        if (ui.isInteractionPanelOpen()) {
            ui.closeInteractionPanel();
        } else {
            exit(0);
        }
    } else if (key == 'e' || key == 'E') {
        if (ui.isInteractionPanelOpen()) {
            ui.closeInteractionPanel();
        } else if (museum.isPlayerNearInteractable(camera.getPosition())) {
            ui.openInteractionPanel();
        }
    } else if (key == 'l' || key == 'L') {
        lighting.toggle();
        ui.showToast(lighting.isEnabled() ? "LIGHTING: ON" : "LIGHTING: OFF");
    } else if (key == 'f' || key == 'F') {
        museum.toggleFloorStyle();
        ui.showToast(museum.getFloorStyle() == 0 ? "FLOOR: POLISHED MARBLE" : "FLOOR: RICH HARDWOOD");
    } else {
        input.setKeyDown(key);
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    input.setKeyUp(key);
}

void mouseMotion(int x, int y) {
    if (!gameStarted || ui.isInteractionPanelOpen()) return;
    
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    
    if (x == centerX && y == centerY) return;
    
    float deltaX = (float)(x - centerX);
    float deltaY = (float)(y - centerY);
    
    camera.processMouse(deltaX, deltaY);
    
    // Reset mouse to center
    glutWarpPointer(centerX, centerY);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Interactive 3D Museum and Art Gallery Explorer");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);
    
    glutMainLoop();
    
    return 0;
}
