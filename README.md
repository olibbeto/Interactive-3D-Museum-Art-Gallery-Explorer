


# Interactive 3D Museum and Art Gallery Explorer

## PROJECT TITLE
Interactive 3D Museum and Art Gallery Explorer

## INTRODUCTION
This is a university-level Computer Graphics project built in C++ using traditional OpenGL (fixed-function pipeline) and freeGLUT. It simulates a first-person interactive art museum with various rooms, procedural artworks, rotating sculptures, and dynamic lighting.

## SYSTEM FEATURES
- First-person camera with WASD movement and mouse-look.
- Wall collision to keep the player inside the museum.
- 6 distinct rooms (Main Hall, Modern Art, Classical Art, Sculpture, Impressionist, Renaissance).
- High-resolution realistic artwork textures & procedurally drawn artworks.
- **Tiled Polished Floors**: High-res Carrara Marble and Luxury Hardwood Parquet with specular highlights (toggleable with `F`).
- **Ornate 3D Picture Frames**: Gilded gold-leaf baroque moulding for classical masterpieces and rich polished mahogany/walnut frames for modern works.
- **Subtle Gallery Plaster Walls & 3D Baseboards**: Realistic off-white Venetian plaster texture and dark wooden skirting boards lining every wall.
- **Real-Time Planar Shadows**: Grounded dynamic shadows beneath spinning sculptures using OpenGL shadow projection matrices and stencil buffering.
- **Glowing Ceiling Track Lights**: Suspended dual-rail matte black track fixtures with angled spotlight canisters, emissive lenses, and additive bloom halos.
- Artwork Interaction System (Press 'E' to open information panels).
- Dynamic lighting with toggle capability (Press 'L').
- Custom 2D HUD built with Orthographic projection.

## COMPUTER GRAPHICS CONCEPTS
1. **3D Transformations**: Used extensively via `glTranslatef()`, `glRotatef()`, and `glScalef()` to position walls, frames, and sculptures.
2. **Hierarchical Modeling**: Demonstrated in the `Sculpture` class. We use `glPushMatrix()` and `glPopMatrix()` to apply base transformations to pedestals, then stack rotations to make sculptures spin relative to the base.
3. **Camera and Perspective Projection**: Set up using `gluPerspective()` for realistic field-of-view and `gluLookAt()` driven by pitch and yaw calculations to simulate human sight.
4. **Lighting**: We use OpenGL's `GL_LIGHTING`. Different rooms have different light colors (cool in Modern, warm in Classical) mapped to `GL_LIGHT0`, `GL_LIGHT1`, etc.
5. **Materials**: Used to differentiate the polished floor (`GL_SHININESS = 50.0f`), matte walls, and specular frames.
6. **Depth Testing and Culling**: `GL_DEPTH_TEST` ensures closer objects obscure further ones, and `GL_CULL_FACE` improves performance by not rendering the backs of polygons.
7. **Animation**: Using delta-time to smoothly rotate sculptures regardless of frame rate.

## CONTROLS
- **W, A, S, D**: Move Forward, Left, Back, Right.
- **Mouse**: Look around (Pitch / Yaw).
- **E**: Interact with Artwork when prompted.
- **L**: Toggle Museum Lighting.
- **F**: Toggle Floor Style (Polished Carrara Marble / Luxury Hardwood).
- **ESC**: Exit game or close info panel.

## BUILD INSTRUCTIONS (Windows & Linux)

This project uses `CMake` to automatically download and configure `freeGLUT`. You do not need to install freeGLUT manually if you have CMake!

### Option 1: Using CMake via Command Line
1. Open terminal/PowerShell in this folder.
2. `mkdir build`
3. `cd build`
4. `cmake ..`
5. `cmake --build .`
6. Run the output executable (e.g., `Debug/Interactive3DMuseum.exe` on Windows or `./Interactive3DMuseum` on Linux).

### Option 2: Using Visual Studio
1. Open Visual Studio.
2. Select "Open a local folder" and choose the `Interactive3DMuseum` directory.
3. Visual Studio will automatically detect `CMakeLists.txt`, download freeGLUT, and build the project.
4. Select `Interactive3DMuseum.exe` as the startup item and hit Run.

## FUTURE IMPROVEMENTS
- Adding more museum rooms and a second floor.
- Implementing texture mapping with `stb_image` for realistic canvas textures.
- Adding shadows using stencil buffers or shadow mapping.
- Audio guide using an external audio library.
