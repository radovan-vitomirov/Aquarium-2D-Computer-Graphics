# 🐠 Aquarium — 2D OpenGL Project

A full-screen 2D aquarium simulation written in **C++** using **modern OpenGL 3.3 (Core Profile)** with a fully programmable pipeline. Two fish swim through the water, a crab walks and jumps along the sandy bottom, and a treasure chest can be opened and closed. Fish food can be dropped into the tank, and every creature that eats grows a little.

The project was made for the **Computer Graphics** course (2D project, 2026/2027 school year).

**Author:** Radovan Vitomirov, RA 132/2023

---

## 📑 Table of Contents

- [Features](#-features)
- [Controls](#-controls)
- [Technologies](#-technologies)
- [Project Structure](#-project-structure)
- [Building and Running](#-building-and-running)
- [How It Works](#-how-it-works)
  - [Rendering](#rendering)
  - [Shaders](#shaders)
  - [Game Logic](#game-logic)
  - [Crab Jump Physics](#crab-jump-physics)
- [Configuration](#-configuration)
- [Deviations from the Specification](#-deviations-from-the-specification)

---

## ✨ Features

| Feature | Description |
|---|---|
| **Full-screen aquarium** | Covers the full screen width and more than half of its height, touching the bottom edge. Black borders mark its left, right and bottom edges. |
| **Sandy bottom and seaweed** | A tiled sand texture fills the bottom, with two seaweed stalks growing out of it. |
| **Treasure chest** | Starts closed. Press `C` to open it and show the coins and gem inside, and press `C` again to close it. |
| **Two controllable fish** | A goldfish and a clownfish. Each one's texture is mirrored horizontally when it changes direction. |
| **Bubbles** | Each fish can release three bubbles from its mouth. The bubbles rise and disappear when they reach the surface. |
| **Jumping crab** | Walks left and right along the sand without mirroring, and jumps using projectile-motion (vertical throw) physics. |
| **Fish food** | Food particles fall from above the screen at a constant speed and stop on the sand. |
| **Eating and growing** | Fish eat food that is still sinking. The crab eats food lying on the sand, or sinking food it catches mid-jump. Each particle eaten makes the creature **0.01** taller. |
| **Custom cursor** | The mouse cursor is an anchor image. |
| **Name tag** | A semi-transparent texture in the top-left corner shows the author's name and index number (Radovan Vitomirov, RA 132/2023). |
| **75 FPS frame limiter** | The main loop is capped at 75 frames per second. |

---

## 🎮 Controls

| Key | Action |
|---|---|
| `W` `A` `S` `D` | Move the **goldfish** |
| `↑` `←` `↓` `→` | Move the **clownfish** |
| `Z` | Goldfish releases 3 bubbles |
| `K` | Clownfish releases 3 bubbles |
| `1` / `2` | Move the **crab** left / right |
| `Space` | Make the crab jump |
| `Enter` | Drop fish food |
| `C` | Open / close the treasure chest |
| `Esc` | Quit the application |

---

## 🛠 Technologies

- **C++** (MSVC, Visual Studio 2022, toolset `v143`)
- **OpenGL 3.3 Core Profile** with a programmable pipeline (vertex and fragment shaders)
- **[GLFW 3.4](https://www.glfw.org/)**: window, context, keyboard input and custom cursor
- **[GLEW 2.2.0](http://glew.sourceforge.net/)**: OpenGL extension loading
- **[stb_image](https://github.com/nothings/stb)**: loading PNG textures and the cursor image

GLFW and GLEW are included as NuGet packages (see `Kostur/packages.config`).

---

## 📁 Project Structure

```
Projekat 2D/
├── README.md
├── Specifikacija.docx          # Original project specification (Serbian)
└── Kostur/                     # Visual Studio solution
    ├── Kostur.sln
    ├── Kostur.vcxproj
    ├── packages.config         # NuGet dependencies (GLFW, GLEW)
    │
    ├── Header/
    │   ├── Aquarium.h          # Constants, data structures and game-logic declarations
    │   ├── Render.h            # VAO creation and drawing helpers
    │   ├── Util.h              # Shader, texture and cursor utilities
    │   └── stb_image.h         # Single-header image loader
    │
    ├── Source/
    │   ├── Main.cpp            # Setup, main loop, render order and cleanup
    │   ├── Aquarium.cpp        # Fish, bubbles, crab, food and input handling
    │   ├── Render.cpp          # Building quads (VAO/VBO) and draw calls
    │   └── Util.cpp            # Shader compilation, texture and cursor loading
    │
    ├── Shaders/
    │   ├── basic.vert          # Vertex shader (position, scale, mirroring)
    │   └── basic.frag          # Fragment shader (color or texture with alpha)
    │
    └── Resources/              # Textures
        ├── anchor.png          # Cursor
        ├── sky.png, water.png, sand.png, seaweed.png
        ├── chest_closed.png, chest_open.png
        ├── goldfish.png, clownfish.png, crab.png
        ├── bubble.png, food.png
        └── name.png            # Name tag (Radovan Vitomirov, RA 132/2023)
```

---

## 🚀 Building and Running

### Requirements

- Windows 10/11
- **Visual Studio 2022** with the *Desktop development with C++* workload
- A GPU and driver that support **OpenGL 3.3** or newer

### Steps

1. Open `Kostur/Kostur.sln` in Visual Studio.
2. If the NuGet packages are missing, restore them: right-click the solution → **Restore NuGet Packages**.
3. Select the **x64** platform and either the **Debug** or **Release** configuration.
4. Build and run with **F5** or **Ctrl + F5**.

> **Note:** Shaders and textures are loaded with relative paths (`Shaders/...`, `Resources/...`), so the working directory must be the `Kostur/` project folder. This is Visual Studio's default when you run from the IDE. To run the `.exe` directly, copy the `Shaders/` and `Resources/` folders, plus `glfw3.dll` and `glew32.dll`, next to it.

The application opens in **full screen** at the primary monitor's native resolution. Press `Esc` to exit at any time.

---

## 🔍 How It Works

### Rendering

Every object on screen is a **textured or colored quad**, drawn as a `GL_TRIANGLE_FAN` with 4 vertices. Each vertex has 8 floats:

```
[ x, y ]  [ r, g, b, a ]  [ s, t ]
position     color         texture coords
```

`Render.cpp` provides these helpers:

- `formColorRect(...)`: builds a solid-color quad (used for the black borders).
- `formTextureRect(...)`: builds a textured quad. Its optional `sRepeat` parameter tiles the texture horizontally; the sand uses this to repeat 8 times.
- `drawColored(VAO)` / `drawTextured(VAO, texture, x, y, scale, flipped, alpha)`: set the shader uniforms and draw the quad.

Each moving object (fish, crab, bubble, food) has **one shared VAO** centered at the origin. Its position, scale and mirroring are applied in the vertex shader through uniforms, so vertex data never needs to be rebuilt.

All sizes are given in pixels and converted to normalized device coordinates (NDC) with `widthFromPixels` and `heightFromPixels`, so objects look the same at any screen resolution.

**Draw order** (back to front):

1. Sky
2. Water (alpha 0.8)
3. Seaweed (×2)
4. Sand
5. Treasure chest
6. Food particles
7. Crab
8. Goldfish and clownfish
9. Bubbles
10. Aquarium borders
11. Name tag (alpha 0.5)

Alpha blending is enabled with `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)`.

### Shaders

**Vertex shader** (`basic.vert`)

| Uniform | Purpose |
|---|---|
| `uPosition` (`vec2`) | Moves the quad to the object's position |
| `uScale` (`float`) | Scales the quad **vertically**, which is how fish and the crab grow taller |
| `flipped` (`bool`) | Mirrors the texture horizontally (`s → 1 - s`) |

**Fragment shader** (`basic.frag`)

| Uniform | Purpose |
|---|---|
| `useTexture` (`bool`) | Chooses between the texture and the vertex color |
| `uTexture` (`sampler2D`) | The bound texture |
| `uAlpha` (`float`) | Multiplies the texture's alpha, for transparent water and the name tag |

### Game Logic

All game logic is in `Aquarium.cpp`, and the main loop calls it once per frame:

```cpp
moveFish(goldfish);   moveFish(clownfish);
updateBubbles();
updateCrab();
updateFood();
fishEat();
crabEat();
```

- **Fish movement:** fish move at a constant speed and are clamped to the aquarium: between the side borders, and between the sand and the water surface. Moving left or right sets the `flipped` flag.
- **Bubbles:** `MAX_BUBBLES` slots are reused. Each key press activates 3 free slots, placed diagonally from the fish's mouth on the side it is facing. A bubble is deactivated once it rises above the water surface.
- **Food:** each `Enter` press activates up to 5 particles at random positions **above the screen**. They fall at a constant speed and are marked `onSand` once they reach the sand.
- **Eating:**
  - A fish eats a particle when the particle lies inside the fish's bounding box and is **not** on the sand.
  - The crab eats a particle when it is inside the crab's bounding box and is either on the sand, or sinking while the crab is jumping.
  - Each particle eaten increases the eater's height by `GROW_STEP = 0.01`.

### Crab Jump Physics

The crab's jump follows **vertical projectile motion**:

$$
v_0 = \sqrt{2 g h}, \qquad s(t) = v_0 t - \tfrac{1}{2} g t^2
$$

`g = JUMP_G` and `h = JUMP_H` set the gravity and the maximum jump height. The jump starts when `Space` is pressed and ends when `s(t)` drops below zero. A new jump can't start until the current one has landed.

---

## ⚙ Configuration

Gameplay constants are defined at the top of [`Header/Aquarium.h`](Kostur/Header/Aquarium.h):

| Constant | Default | Meaning |
|---|---|---|
| `AQUARIUM_TOP` | `0.4` | Y coordinate of the water surface (NDC) |
| `SAND_TOP` | `-0.7` | Y coordinate of the top of the sand (NDC) |
| `FISH_SPEED` | `0.01` | Fish speed per frame |
| `CRAB_SPEED` | `0.008` | Crab speed per frame |
| `JUMP_G` / `JUMP_H` | `3.0` / `0.15` | Crab jump gravity and maximum height |
| `MAX_BUBBLES` | `30` | Maximum number of bubbles on screen at once |
| `BUBBLES_PER_PRESS` | `3` | Bubbles released per key press |
| `BUBBLE_SPEED` | `0.006` | Bubble rising speed per frame |
| `MAX_FOOD` | `50` | Maximum number of food particles at once |
| `MAX_PER_DROP` | `5` | Food particles per `Enter` press |
| `FOOD_SPEED` | `0.006` | Food falling speed per frame |
| `GROW_STEP` | `0.01` | Height gained per particle eaten |

---

## 📝 Deviations from the Specification

A few visual choices differ on purpose from the original specification:

- **Border thickness:** **30 px** instead of 10 px, because 10 px borders were barely visible in full screen.
- **Water:** the specification asks for a white quad with 0.2 transparency. A **water texture with alpha 0.8** is used instead, so the aquarium looks more like the sea.
- **Sky:** a sky texture was added above the aquarium so that the top part of the screen is not empty.
