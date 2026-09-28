#include "UI.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

// ---------------------------------------------------------------------------
// Drawing helpers (file-local). Everything here assumes the 2D orthographic
// setup from UI::setOrthographicProjection: origin top-left, y grows downward.
// ---------------------------------------------------------------------------
namespace {

const float GOLD[3]  = { 0.85f, 0.68f, 0.35f };
const float IVORY[3] = { 0.96f, 0.93f, 0.86f };
const float MUTED[3] = { 0.66f, 0.64f, 0.60f };
const float BLACK[3] = { 0.00f, 0.00f, 0.00f };
const float PI_F     = 3.14159265f;

// Functions instead of constants: on Windows the GLUT font macros are
// addresses of extern objects, so this avoids any static-init surprises.
inline void* fTitle() { return GLUT_BITMAP_TIMES_ROMAN_24; }
inline void* fBody()  { return GLUT_BITMAP_HELVETICA_18; }
inline void* fSmall() { return GLUT_BITMAP_HELVETICA_12; }

float clamp01(float v) { return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v); }
float smooth(float v)  { v = clamp01(v); return v * v * (3.0f - 2.0f * v); }
float nowSec()         { return glutGet(GLUT_ELAPSED_TIME) / 1000.0f; }

void setColor(const float c[3], float a) { glColor4f(c[0], c[1], c[2], a); }

// ----- text ---------------------------------------------------------------

int textWidth(void* font, const std::string& s) {
    return glutBitmapLength(font, (const unsigned char*)s.c_str());
}

// Width of a string drawn with extra letter-spacing ("tracking").
float spacedWidth(void* font, const std::string& s, float tracking) {
    if (s.empty()) return 0.0f;
    float w = 0.0f;
    for (size_t i = 0; i < s.size(); ++i)
        w += glutBitmapWidth(font, (unsigned char)s[i]) + tracking;
    return w - tracking;
}

void drawText(void* font, float x, float y, const std::string& s) {
    glRasterPos2f(std::floor(x), std::floor(y));
    for (size_t i = 0; i < s.size(); ++i)
        glutBitmapCharacter(font, (unsigned char)s[i]);  // unsigned: no UTF-8 garbage from signed chars
}

void drawSpaced(void* font, float x, float y, const std::string& s, float tracking) {
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char ch = (unsigned char)s[i];
        glRasterPos2f(std::floor(x), std::floor(y));
        glutBitmapCharacter(font, ch);
        x += glutBitmapWidth(font, ch) + tracking;
    }
}

void drawSpacedCentered(void* font, float cx, float y, const std::string& s, float tracking) {
    drawSpaced(font, cx - spacedWidth(font, s, tracking) * 0.5f, y, s, tracking);
}

// Word-wrap to maxWidth pixels. Honors '\n'. Always returns at least one line.
std::vector<std::string> wrapText(void* font, const std::string& text, float maxWidth) {
    std::vector<std::string> lines;
    size_t start = 0;
    while (start <= text.size()) {
        size_t end = text.find('\n', start);
        if (end == std::string::npos) end = text.size();
        std::string para = text.substr(start, end - start);

        std::string line;
        size_t i = 0;
        while (i < para.size()) {
            size_t j = para.find(' ', i);
            if (j == std::string::npos) j = para.size();
            std::string word = para.substr(i, j - i);
            i = j + 1;
            if (word.empty()) continue;

            std::string candidate = line.empty() ? word : line + " " + word;
            if (!line.empty() && textWidth(font, candidate) > maxWidth) {
                lines.push_back(line);
                line = word;
            } else {
                line = candidate;
            }
        }
        lines.push_back(line);
        start = end + 1;
    }
    return lines;
}

// ----- shapes -------------------------------------------------------------

void fillRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);     glVertex2f(x + w, y);
    glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();
}

void gradientV(float x, float y, float w, float h, const float top[4], const float bot[4]) {
    glBegin(GL_QUADS);
    glColor4fv(top); glVertex2f(x, y);     glVertex2f(x + w, y);
    glColor4fv(bot); glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();
}

void gradientH(float x, float y, float w, float h, const float left[4], const float right[4]) {
    glBegin(GL_QUADS);
    glColor4fv(left);  glVertex2f(x, y);     glVertex2f(x, y + h);
    glColor4fv(right); glVertex2f(x + w, y + h); glVertex2f(x + w, y);
    glEnd();
}

void strokeRect(float x, float y, float w, float h, float lineWidth) {
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 0.5f, y + 0.5f);         glVertex2f(x + w - 0.5f, y + 0.5f);
    glVertex2f(x + w - 0.5f, y + h - 0.5f); glVertex2f(x + 0.5f, y + h - 0.5f);
    glEnd();
}

void cornerBrackets(float x, float y, float w, float h, float len) {
    glBegin(GL_LINES);
    glVertex2f(x, y);         glVertex2f(x + len, y);
    glVertex2f(x, y);         glVertex2f(x, y + len);
    glVertex2f(x + w, y);     glVertex2f(x + w - len, y);
    glVertex2f(x + w, y);     glVertex2f(x + w, y + len);
    glVertex2f(x, y + h);     glVertex2f(x + len, y + h);
    glVertex2f(x, y + h);     glVertex2f(x, y + h - len);
    glVertex2f(x + w, y + h); glVertex2f(x + w - len, y + h);
    glVertex2f(x + w, y + h); glVertex2f(x + w, y + h - len);
    glEnd();
}

void diamond(float cx, float cy, float r) {
    glBegin(GL_QUADS);
    glVertex2f(cx, cy - r); glVertex2f(cx + r, cy);
    glVertex2f(cx, cy + r); glVertex2f(cx - r, cy);
    glEnd();
}

void ring(float cx, float cy, float r) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 40; ++i) {
        float a = i * 2.0f * PI_F / 40.0f;
        glVertex2f(cx + std::cos(a) * r, cy + std::sin(a) * r);
    }
    glEnd();
}

void ticks(float cx, float cy, float gap, float len) {
    glBegin(GL_LINES);
    glVertex2f(cx - gap - len, cy); glVertex2f(cx - gap, cy);
    glVertex2f(cx + gap, cy);       glVertex2f(cx + gap + len, cy);
    glVertex2f(cx, cy - gap - len); glVertex2f(cx, cy - gap);
    glVertex2f(cx, cy + gap);       glVertex2f(cx, cy + gap + len);
    glEnd();
}

// Soft elliptical gradient (used for the warm glow and the vignette).
void radialGlow(float cx, float cy, float rx, float ry, const float c[3],
                float centerAlpha, float edgeAlpha) {
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(c[0], c[1], c[2], centerAlpha);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 64; ++i) {
        float a = i * 2.0f * PI_F / 64.0f;
        glColor4f(c[0], c[1], c[2], edgeAlpha);
        glVertex2f(cx + std::cos(a) * rx, cy + std::sin(a) * ry);
    }
    glEnd();
}

// Thin rule that fades out at both ends with a small diamond in the middle.
void ornament(float cx, float y, float halfWidth, const float c[3], float alpha) {
    const float gap = 14.0f;
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor4f(c[0], c[1], c[2], 0.0f);  glVertex2f(cx - halfWidth, y + 0.5f);
    glColor4f(c[0], c[1], c[2], alpha); glVertex2f(cx - gap, y + 0.5f);
    glColor4f(c[0], c[1], c[2], alpha); glVertex2f(cx + gap, y + 0.5f);
    glColor4f(c[0], c[1], c[2], 0.0f);  glVertex2f(cx + halfWidth, y + 0.5f);
    glEnd();
    setColor(c, alpha);
    diamond(cx, y + 0.5f, 4.5f);
}

// ----- key caps + control legend -------------------------------------------

float keycapWidth(const std::string& label) {
    return (float)textWidth(fSmall(), label) + 16.0f;
}

float drawKeycap(float x, float y, const std::string& label, float alpha) {
    const float w = keycapWidth(label), h = 22.0f;
    glColor4f(1.0f, 1.0f, 1.0f, 0.07f * alpha);
    fillRect(x, y, w, h);
    setColor(GOLD, 0.65f * alpha);
    strokeRect(x, y, w, h, 1.0f);
    setColor(IVORY, 0.95f * alpha);
    drawText(fSmall(), x + 8.0f, y + 15.0f, label);
    return w;
}

typedef std::vector<std::pair<std::string, std::string> > LegendItems;

void drawLegend(float cx, float y, const LegendItems& items, float alpha) {
    if (alpha <= 0.01f || items.empty()) return;
    const float capGap = 8.0f, itemGap = 30.0f, tracking = 1.5f;

    float total = 0.0f;
    for (size_t i = 0; i < items.size(); ++i) {
        total += keycapWidth(items[i].first) + capGap + spacedWidth(fSmall(), items[i].second, tracking);
        if (i + 1 < items.size()) total += itemGap;
    }

    float x = cx - total * 0.5f;
    for (size_t i = 0; i < items.size(); ++i) {
        x += drawKeycap(x, y, items[i].first, alpha) + capGap;
        glColor4f(0.90f, 0.87f, 0.80f, 0.85f * alpha);
        drawSpaced(fSmall(), x, y + 15.0f, items[i].second, tracking);
        x += spacedWidth(fSmall(), items[i].second, tracking) + itemGap;
    }
}

LegendItems fullControls(int floorStyle = 0) {
    LegendItems v;
    v.push_back(std::make_pair(std::string("W A S D"), std::string("MOVE")));
    v.push_back(std::make_pair(std::string("MOUSE"),   std::string("LOOK")));
    v.push_back(std::make_pair(std::string("E"),       std::string("INTERACT")));
    v.push_back(std::make_pair(std::string("L"),       std::string("LIGHTS")));
    v.push_back(std::make_pair(std::string("F"),       floorStyle == 0 ? std::string("FLOOR (MARBLE)") : std::string("FLOOR (WOOD)")));
    v.push_back(std::make_pair(std::string("ESC"),     std::string("EXIT")));
    return v;
}

LegendItems compactControls(int floorStyle = 0) {
    LegendItems v;
    v.push_back(std::make_pair(std::string("E"),   std::string("INTERACT")));
    v.push_back(std::make_pair(std::string("L"),   std::string("LIGHTS")));
    v.push_back(std::make_pair(std::string("F"),   floorStyle == 0 ? std::string("FLOOR (MARBLE)") : std::string("FLOOR (WOOD)")));
    v.push_back(std::make_pair(std::string("ESC"), std::string("EXIT")));
    return v;
}

} // namespace

// ---------------------------------------------------------------------------
// UI
// ---------------------------------------------------------------------------

UI::UI()
    : interactionPanelOpen(false),
      hudStartMs(-1), lastHudMs(-1), roomChangeMs(0),
      panelOpenMs(0), lastPanelMs(-10000),
      crosshairFocus(0.0f),
      toastStartMs(-10000) {}

void UI::showToast(const std::string& message) const {
    toastMessage = message;
    toastStartMs = glutGet(GLUT_ELAPSED_TIME);
}

void UI::setOrthographicProjection(int width, int height) const {
    // Saves blend state, line width and current colour so the 3D scene is untouched.
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LINE_BIT | GL_CURRENT_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void UI::restorePerspectiveProjection() const {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

// ---------------------------------------------------------------------------
// Start screen
// ---------------------------------------------------------------------------
void UI::drawStartScreen(int width, int height) const {
    setOrthographicProjection(width, height);

    const float w = (float)width, h = (float)height;
    const float cx = w * 0.5f, cy = h * 0.5f;
    const float t = nowSec();
    const float intro = smooth(t / 1.4f);            // fade-in on launch
    const float rise  = (1.0f - intro) * 12.0f;      // slight upward glide

    // Background: deep blue-black gradient, warm glow behind the title, vignette
    const float top[4] = { 0.075f, 0.075f, 0.115f, 1.0f };
    const float bot[4] = { 0.012f, 0.012f, 0.028f, 1.0f };
    gradientV(0.0f, 0.0f, w, h, top, bot);
    radialGlow(cx, cy - 30.0f, w * 0.36f, h * 0.34f, GOLD, 0.14f * intro, 0.0f);
    radialGlow(cx, cy, w * 0.75f, h * 0.85f, BLACK, 0.0f, 0.6f);

    // Gallery-style double frame with corner brackets
    const float m = 26.0f;
    setColor(GOLD, 0.30f * intro); strokeRect(m, m, w - 2 * m, h - 2 * m, 1.0f);
    setColor(GOLD, 0.15f * intro); strokeRect(m + 8, m + 8, w - 2 * m - 16, h - 2 * m - 16, 1.0f);
    setColor(GOLD, 0.85f * intro);
    glLineWidth(2.0f);
    cornerBrackets(m, m, w - 2 * m, h - 2 * m, 30.0f);
    glLineWidth(1.0f);

    // Title block
    const float ty = cy - 20.0f + rise;
    setColor(GOLD, 0.95f * intro);
    drawSpacedCentered(fSmall(), cx, ty - 78.0f, "AN INTERACTIVE EXPERIENCE", 5.0f);
    setColor(IVORY, intro);
    drawSpacedCentered(fTitle(), cx, ty - 34.0f, "INTERACTIVE 3D MUSEUM", 7.0f);
    setColor(IVORY, 0.75f * intro);
    drawSpacedCentered(fBody(), cx, ty + 2.0f, "AND ART GALLERY EXPLORER", 5.0f);
    ornament(cx, ty + 30.0f, 230.0f, GOLD, 0.8f * intro);

    // "EXPLORE  <>  DISCOVER  <>  EXPERIENCE" (diamonds are drawn, not text,
    // because the GLUT bitmap fonts can't render the bullet character)
    {
        const std::string a = "EXPLORE", b = "DISCOVER", c = "EXPERIENCE";
        const float tr = 4.0f, side = 22.0f;
        const float wa = spacedWidth(fSmall(), a, tr);
        const float wb = spacedWidth(fSmall(), b, tr);
        const float wc = spacedWidth(fSmall(), c, tr);
        float x = cx - (wa + wb + wc + 4.0f * side) * 0.5f;
        const float y = ty + 62.0f;
        setColor(MUTED, intro);
        drawSpaced(fSmall(), x, y, a, tr);           x += wa + side;
        setColor(GOLD, 0.9f * intro); diamond(x, y - 4.0f, 3.0f); x += side;
        setColor(MUTED, intro);
        drawSpaced(fSmall(), x, y, b, tr);           x += wb + side;
        setColor(GOLD, 0.9f * intro); diamond(x, y - 4.0f, 3.0f); x += side;
        setColor(MUTED, intro);
        drawSpaced(fSmall(), x, y, c, tr);
    }

    // Pulsing call-to-action button
    {
        const std::string cta = "PRESS ENTER TO BEGIN";
        const float pulse = 0.5f + 0.5f * std::sin(t * 2.6f);
        const float bw = spacedWidth(fBody(), cta, 4.0f) + 72.0f, bh = 46.0f;
        const float bx = cx - bw * 0.5f, by = ty + 92.0f;
        setColor(GOLD, (0.05f + 0.10f * pulse) * intro);
        fillRect(bx, by, bw, bh);
        setColor(GOLD, (0.45f + 0.50f * pulse) * intro);
        strokeRect(bx, by, bw, bh, 1.5f);
        glColor4f(GOLD[0] + (IVORY[0] - GOLD[0]) * pulse,
                  GOLD[1] + (IVORY[1] - GOLD[1]) * pulse,
                  GOLD[2] + (IVORY[2] - GOLD[2]) * pulse, intro);
        drawSpacedCentered(fBody(), cx, by + 29.0f, cta, 4.0f);
    }

    // Controls cheat-sheet so players know the keys before starting
    drawLegend(cx, h - 82.0f, fullControls(), 0.95f * intro);

    restorePerspectiveProjection();
}

// ---------------------------------------------------------------------------
// HUD
// ---------------------------------------------------------------------------
void UI::drawHUD(int width, int height, const std::string& roomName, bool showInteractPrompt, int floorStyle) const {
    setOrthographicProjection(width, height);

    const float w = (float)width, h = (float)height;
    const float cx = w * 0.5f, cy = h * 0.5f;
    const int   nowMs = glutGet(GLUT_ELAPSED_TIME);
    const float t = nowMs / 1000.0f;

    if (hudStartMs < 0) hudStartMs = nowMs;
    if (roomName != lastRoomName) { lastRoomName = roomName; roomChangeMs = nowMs; }

    float dt = (lastHudMs < 0) ? 0.016f : (nowMs - lastHudMs) / 1000.0f;
    if (dt > 0.1f) dt = 0.1f;
    lastHudMs = nowMs;

    // --- Crosshair: small dot + ticks, morphs into a gold ring on artwork ---
    const float target = (showInteractPrompt && !interactionPanelOpen) ? 1.0f : 0.0f;
    crosshairFocus += (target - crosshairFocus) * std::min(1.0f, dt * 12.0f);
    const float f = crosshairFocus;
    {
        const float gap = 7.0f + 3.0f * f, tickA = 1.0f - 0.65f * f;
        glColor4f(0.0f, 0.0f, 0.0f, 0.45f);            // dark halo keeps it visible on white walls
        diamond(cx, cy, 3.6f);
        glLineWidth(3.0f);
        glColor4f(0.0f, 0.0f, 0.0f, 0.30f * tickA);
        ticks(cx, cy, gap, 7.0f);
        glLineWidth(1.5f);
        setColor(IVORY, 0.9f * tickA);
        ticks(cx, cy, gap, 7.0f);
        setColor(IVORY, 0.95f);
        diamond(cx, cy, 2.2f);
        if (f > 0.01f) {
            const float r = 13.0f + 1.5f * std::sin(t * 5.0f);
            glLineWidth(3.0f);
            glColor4f(0.0f, 0.0f, 0.0f, 0.30f * f);
            ring(cx, cy, r);
            glLineWidth(1.5f);
            setColor(GOLD, 0.95f * f);
            ring(cx, cy, r);
        }
    }

    // --- Location banner (top-left) ---
    {
        const float bx = 24.0f, by = 24.0f, bh = 46.0f;
        const float bw = spacedWidth(fBody(), roomName, 3.0f) + 52.0f;
        const float l[4] = { 0.0f, 0.0f, 0.0f, 0.60f };
        const float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        gradientH(bx, by, bw + 70.0f, bh, l, r);
        setColor(GOLD, 0.95f);
        fillRect(bx, by, 3.0f, bh);
        drawSpaced(fSmall(), bx + 16.0f, by + 17.0f, "LOCATION", 4.0f);
        setColor(IVORY, 1.0f);
        drawSpaced(fBody(), bx + 16.0f, by + 38.0f, roomName, 3.0f);
    }

    // --- Toast notification banner (top-center, e.g. for Floor / Light toggle) ---
    if (!toastMessage.empty()) {
        const float age = (nowMs - toastStartMs) / 1000.0f;
        const float duration = 2.4f;
        if (age < duration) {
            float alpha = 1.0f;
            if (age < 0.25f) {
                alpha = smooth(age / 0.25f);
            } else if (age > duration - 0.5f) {
                alpha = 1.0f - smooth((age - (duration - 0.5f)) / 0.5f);
            }
            if (alpha > 0.01f) {
                const float toastY = 46.0f;
                const float textW = spacedWidth(fBody(), toastMessage, 3.0f);
                const float tw = textW + 56.0f, th = 34.0f;
                const float tx = cx - tw * 0.5f;

                glColor4f(0.04f, 0.04f, 0.06f, 0.88f * alpha);
                fillRect(tx, toastY - th * 0.5f, tw, th);

                setColor(GOLD, 0.85f * alpha);
                strokeRect(tx, toastY - th * 0.5f, tw, th, 1.2f);
                diamond(tx + 14.0f, toastY, 3.0f);
                diamond(tx + tw - 14.0f, toastY, 3.0f);

                setColor(IVORY, alpha);
                drawSpacedCentered(fBody(), cx, toastY + 5.0f, toastMessage, 3.0f);
            }
        }
    }

    // --- "Now entering" card when the room changes ---
    {
        const float age = (nowMs - roomChangeMs) / 1000.0f;
        const float a = smooth(age / 0.6f) * (1.0f - smooth((age - 2.6f) / 1.0f));
        if (a > 0.01f && !roomName.empty()) {
            const float y = h * 0.22f - (1.0f - smooth(age / 0.6f)) * 8.0f;
            const float nameW = spacedWidth(fTitle(), roomName, 8.0f);
            const float half = std::max(nameW * 0.5f + 40.0f, 170.0f);
            const float e[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            const float m[4] = { 0.0f, 0.0f, 0.0f, 0.42f * a };
            gradientH(cx - half - 120.0f, y - 52.0f, half + 120.0f, 118.0f, e, m);
            gradientH(cx, y - 52.0f, half + 120.0f, 118.0f, m, e);
            setColor(GOLD, 0.95f * a);
            drawSpacedCentered(fSmall(), cx, y - 26.0f, "NOW ENTERING", 6.0f);
            setColor(IVORY, a);
            drawSpacedCentered(fTitle(), cx, y + 10.0f, roomName, 8.0f);
            ornament(cx, y + 34.0f, half, GOLD, 0.8f * a);
        }
    }

    // --- Interaction prompt pill ---
    if (f > 0.01f) {
        const std::string label = "VIEW ARTWORK";
        const float capW = keycapWidth("E");
        const float textW = spacedWidth(fBody(), label, 2.0f);
        const float pw = capW + 12.0f + textW + 36.0f, ph = 40.0f;
        const float px = cx - pw * 0.5f;
        const float py = cy + 42.0f + (1.0f - f) * 6.0f;
        glColor4f(0.04f, 0.04f, 0.06f, 0.78f * f);
        fillRect(px, py, pw, ph);
        setColor(GOLD, 0.85f * f);
        strokeRect(px, py, pw, ph, 1.0f);
        drawKeycap(px + 18.0f, py + 9.0f, "E", f);
        setColor(IVORY, f);
        drawSpaced(fBody(), px + 18.0f + capW + 12.0f, py + 26.0f, label, 2.0f);
    }

    // --- Controls hint: full legend for a few seconds, then a quiet compact one ---
    {
        const float elapsed  = (nowMs - hudStartMs) / 1000.0f;
        const float aFull    = 1.0f - smooth(elapsed - 10.0f);
        const float aCompact = smooth(elapsed - 10.5f);
        const float shade    = std::max(aFull * 0.75f, aCompact * 0.55f);
        const float g0[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        const float g1[4] = { 0.0f, 0.0f, 0.0f, shade };
        gradientV(0.0f, h - 100.0f, w, 100.0f, g0, g1);
        drawLegend(cx, h - 44.0f, fullControls(floorStyle), aFull);
        drawLegend(cx, h - 44.0f, compactControls(floorStyle), aCompact * 0.65f);
    }

    restorePerspectiveProjection();
}

// ---------------------------------------------------------------------------
// Artwork info panel
// ---------------------------------------------------------------------------
void UI::drawInteractionPanel(int width, int height, const ArtInfo& info) const {
    setOrthographicProjection(width, height);

    const float w = (float)width, h = (float)height;
    const int nowMs = glutGet(GLUT_ELAPSED_TIME);

    // Detect a fresh open (no panel frame for a moment) to restart the fade-in
    if (nowMs - lastPanelMs > 250) panelOpenMs = nowMs;
    lastPanelMs = nowMs;
    const float a     = smooth((nowMs - panelOpenMs) / 1000.0f / 0.25f);
    const float slide = (1.0f - a) * 14.0f;

    // Dim the museum behind the panel instead of hiding it completely
    glColor4f(0.0f, 0.0f, 0.0f, 0.60f * a);
    fillRect(0.0f, 0.0f, w, h);

    // Layout
    const float pw = std::min(660.0f, w - 60.0f);
    const float padX = 44.0f, lineH = 26.0f;
    const std::vector<std::string> lines = wrapText(fBody(), info.description, pw - 2.0f * padX);
    const float lastBaseline = 208.0f + (lines.size() - 1) * lineH;
    const float sepY = lastBaseline + 30.0f;
    const float footerBase = sepY + 30.0f;
    const float ph = footerBase + 24.0f;
    const float px = w * 0.5f - pw * 0.5f;
    const float py = h * 0.5f - ph * 0.5f + slide;

    // Soft shadow
    for (int i = 1; i <= 6; ++i) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.06f * a);
        fillRect(px - i * 3.0f, py - i * 3.0f + 8.0f, pw + i * 6.0f, ph + i * 6.0f);
    }

    // Panel body + gold frame
    const float p0[4] = { 0.105f, 0.105f, 0.125f, 0.95f * a };
    const float p1[4] = { 0.045f, 0.045f, 0.065f, 0.97f * a };
    gradientV(px, py, pw, ph, p0, p1);
    setColor(GOLD, 0.55f * a);  strokeRect(px, py, pw, ph, 1.5f);
    setColor(GOLD, 0.20f * a);  strokeRect(px + 6.0f, py + 6.0f, pw - 12.0f, ph - 12.0f, 1.0f);
    setColor(GOLD, 0.95f * a);
    glLineWidth(2.0f);
    cornerBrackets(px - 6.0f, py - 6.0f, pw + 12.0f, ph + 12.0f, 20.0f);
    glLineWidth(1.0f);

    // Header label, title, ornament
    setColor(GOLD, 0.95f * a);
    drawSpacedCentered(fSmall(), px + pw * 0.5f, py + 38.0f, "NOW VIEWING", 5.0f);

    float titleTrack = 4.0f;
    if (spacedWidth(fTitle(), info.title, titleTrack) > pw - 2.0f * padX) titleTrack = 0.0f;
    setColor(IVORY, a);
    drawSpacedCentered(fTitle(), px + pw * 0.5f, py + 78.0f, info.title, titleTrack);
    ornament(px + pw * 0.5f, py + 98.0f, pw * 0.5f - padX, GOLD, 0.8f * a);

    // Artist / category columns
    setColor(MUTED, a);
    drawSpaced(fSmall(), px + padX, py + 134.0f, "ARTIST", 3.0f);
    setColor(IVORY, a);
    drawText(fBody(), px + padX, py + 158.0f, info.artist);

    setColor(MUTED, a);
    drawSpaced(fSmall(), px + pw * 0.5f + 10.0f, py + 134.0f, "CATEGORY", 3.0f);
    setColor(IVORY, a);
    drawText(fBody(), px + pw * 0.5f + 10.0f, py + 158.0f, info.category);

    // Divider + wrapped description
    setColor(GOLD, 0.25f * a);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex2f(px + padX, py + 180.5f); glVertex2f(px + pw - padX, py + 180.5f);
    glEnd();

    glColor4f(0.86f, 0.84f, 0.76f, a);
    for (size_t i = 0; i < lines.size(); ++i)
        drawText(fBody(), px + padX, py + 208.0f + i * lineH, lines[i]);

    // Footer: [E] or [ESC] to close
    setColor(GOLD, 0.25f * a);
    glBegin(GL_LINES);
    glVertex2f(px + padX, py + sepY + 0.5f); glVertex2f(px + pw - padX, py + sepY + 0.5f);
    glEnd();
    {
        const float gap = 8.0f;
        const float wOr = spacedWidth(fSmall(), "OR", 2.0f);
        const float wClose = spacedWidth(fSmall(), "TO CLOSE", 2.0f);
        const float total = keycapWidth("E") + gap + wOr + gap + keycapWidth("ESC") + 12.0f + wClose;
        float x = px + pw * 0.5f - total * 0.5f;
        const float y = py + footerBase - 16.0f;
        x += drawKeycap(x, y, "E", a) + gap;
        setColor(MUTED, a);
        drawSpaced(fSmall(), x, y + 15.0f, "OR", 2.0f);
        x += wOr + gap;
        x += drawKeycap(x, y, "ESC", a) + 12.0f;
        setColor(MUTED, a);
        drawSpaced(fSmall(), x, y + 15.0f, "TO CLOSE", 2.0f);
    }

    restorePerspectiveProjection();
}
