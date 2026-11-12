# Interactive 3D Museum & Art Gallery Explorer

A real-time, first-person 3D art museum built in **C++** using the **OpenGL fixed-function pipeline** and **freeGLUT** — developed as a university Computer Graphics project with no external game engines.

---

## Features

- **6 Explorable Gallery Rooms**: Main Hall, Modern Art, Classical Art, Sculpture Gallery, Impressionist Gallery, and Renaissance Gallery.
- **Famous Artworks & Procedural Pieces**: High-resolution `.jpg` textures loaded via `stb_image` (including *Mona Lisa*, *The Starry Night*, *The Birth of Venus*, *The Great Wave*, *Girl with a Pearl Earring*, Kandinsky's *Composition 8*, and Mondrian) alongside procedurally generated OpenGL artworks.
- **Ornate 3D Frames**: Gilded gold-leaf moulding for classical works and polished mahogany/walnut frames for modern pieces.
- **Tiled Polished Floors**: Carrara Marble and Luxury Hardwood Parquet with specular highlights (toggleable with `F`).
- **Venetian Plaster Walls & Baseboards**: Textured gallery walls grounded with dark wooden skirting boards.
- **Real-Time Planar Shadows**: Dynamic shadows beneath spinning sculptures using OpenGL shadow projection matrices and stencil buffering.
- **Glowing Ceiling Track Lights**: Suspended matte-black dual-rail fixtures with spotlight canisters, emissive lenses, and additive bloom halos.
- **First-Person Exploration & Collision**: Smooth `WASD` + mouse-look camera with AABB sliding wall collision.
- **Dynamic 2D HUD**: Room transition cards, interaction prompts, and artwork info panels rendered via orthographic projection (`E` to inspect).
- **Toggleable Lighting**: Press `L` to toggle the gallery lighting on and off.

---

## Controls

| Key / Input | Action |
| :--- | :--- |
| `W` `A` `S` `D` | Move Forward, Left, Back, Right |
| `Mouse` | Look around (Pitch / Yaw) |
| `E` | Inspect nearby artwork |
| `L` | Toggle museum lighting |
| `F` | Toggle floor style (Carrara Marble / Hardwood) |
| `ESC` | Close info panel or exit |

---

## Computer Graphics Concepts Demonstrated

1. **3D Transformations**: Positioning walls, frames, track lights, and sculptures using `glTranslatef()`, `glRotatef()`, and `glScalef()`.
2. **Hierarchical Modeling**: Matrix stack (`glPushMatrix()` / `glPopMatrix()`) used for spinning sculptures on pedestals and artwork frames relative to canvases.
3. **Perspective & Orthographic Projection**: `gluPerspective()` + `gluLookAt()` for the 3D world, switching dynamically to `gluOrtho2D()` for the 2D HUD overlay.
4. **Phong Lighting & Gouraud Shading**: Multi-light setup (`GL_LIGHT0`, `GL_LIGHT1`, etc.) with `GL_AMBIENT`, `GL_DIFFUSE`, `GL_SPECULAR`, and `GL_SHININESS` materials.
5. **Texture Mapping**: `GL_TEXTURE_2D` with mipmaps and UV mapping via `stb_image`.
6. **Depth Testing, Stencil Buffering & Culling**: `GL_DEPTH_TEST`, `GL_STENCIL_TEST` for planar shadows, and `GL_CULL_FACE` for back-face culling.
7. **Frame-Rate Independent Animation**: Delta-time updates for smooth sculpture rotation and UI fades.

---

## Project Structure

```text
Interactive3DMuseum/
├── assets/textures/    # Painting, floor, wall, and frame textures (.jpg)
├── src/
│   ├── main.cpp        # Entry point, GLUT loop, delta-time
│   ├── Museum.cpp/.h   # 6-room layout, collision boxes, artwork/sculpture placement
│   ├── Camera.cpp/.h   # First-person camera & AABB sliding collision
│   ├── Artwork.cpp/.h  # Ornate frames, texture mapping, procedural art
│   ├── Sculpture.cpp/.h# Animated sculptures & planar shadow rendering
│   ├── Lighting.cpp/.h # OpenGL light sources & ceiling track lights
│   ├── UI.cpp/.h       # 2D HUD overlay, room cards, info panels
│   ├── Texture.cpp/.h  # stb_image loader & mipmap generation
│   └── stb_image.h     # Single-header image loading library
├── build.bat           # Quick MinGW build script (Windows)
└── CMakeLists.txt      # Cross-platform CMake configuration
```

---

## Build & Run

### Option 1: Quick Run (Windows Portable Release)
Download the latest `.zip` from **GitHub Releases**, extract it, and double-click `Interactive3DMuseum.exe` (all required `.dll` files and textures are included).

### Option 2: MinGW (`build.bat` on Windows)
```bat
build.bat
```
*(Requires MinGW `g++` with `freeglut`, `opengl32`, and `glu32`.)*

### Option 3: CMake (Windows & Linux)
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
CMake will automatically fetch and configure `freeGLUT` if it isn't installed.

---

## Future Improvements

- Second floor / mezzanine connected by a staircase
- Interactive audio guide narration per artwork
- Dynamic mini-map in the HUD
