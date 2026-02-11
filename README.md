# 🎨 OpenGL Photo Editor

A simple photo editor written in **C++ using OpenGL**, supporting texture blending, opacity control, color filtering, and shader-based effects.

---

## ✨ Features

- Load **two images** as textures at runtime
- Blend textures using opacity
- Apply RGB color filters
- Duplicate texture effects via shader uniforms
- Keyboard-controlled interactions
- Uses modern OpenGL (3.3 Core)

---

## 🧰 Technologies Used

- C++
- OpenGL 3.3 Core Profile
- GLFW
- GLAD
- stb_image
- GLSL (Vertex & Fragment Shaders)

---

## 📁 Project Structure

PhotoShop/

│

├── src/

│ ├── main.cpp

│ ├── VertexShader.glsl

│ ├── FragmentShader.glsl

│ └── stb_image.h

│

└── README.md


---

## 🚀 How to Build & Run

### ✅ Requirements

- C++ Compiler (GCC / MSVC / Clang)
- OpenGL 3.3+
- GLFW
- GLAD
- stb_image.h

---

### 🛠 Build Example (Windows – MinGW)
```bash
g++ main.cpp glad.c -o PhotoEditor -lglfw3 -lopengl32 -lgdi32

