#ifndef UI_H
#define UI_H

#include <string>
#include "Artwork.h"

class UI {
public:
    UI();

    void drawStartScreen(int width, int height) const;
    void drawHUD(int width, int height, const std::string& roomName, bool showInteractPrompt, int floorStyle = 0) const;
    void drawInteractionPanel(int width, int height, const ArtInfo& info) const;
    void showToast(const std::string& message) const;

    bool isInteractionPanelOpen() const { return interactionPanelOpen; }
    void openInteractionPanel() { interactionPanelOpen = true; }
    void closeInteractionPanel() { interactionPanelOpen = false; }

private:
    bool interactionPanelOpen;

    // Animation state. Mutable so the draw methods can stay const.
    mutable int   hudStartMs;      // first HUD frame (controls hint timing)
    mutable int   lastHudMs;       // previous HUD frame (delta time)
    mutable int   roomChangeMs;    // when the room name last changed
    mutable int   panelOpenMs;     // when the info panel last opened
    mutable int   lastPanelMs;     // previous panel frame (detects re-open)
    mutable float crosshairFocus;  // 0 = idle, 1 = looking at artwork
    mutable std::string lastRoomName;
    mutable std::string toastMessage;
    mutable int   toastStartMs;

    void setOrthographicProjection(int width, int height) const;
    void restorePerspectiveProjection() const;
};

#endif // UI_H
