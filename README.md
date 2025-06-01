# Introduction
This project renders a single Phong-shaded sphere using OpenGL and GLSL. Instead of CPU rasterization, it leverages the GPU pipeline with:

### Vertex Shader (shaders/Phong.vert):
• Applies model, view, projection transforms.
• Computes world-space normals via normalMatrix.
• Outputs FragPos and Normal to the fragment shader.

### Fragment Shader (shaders/Phong.frag):
• Receives interpolated FragPos and Normal.
• Computes ambient, diffuse, and Blinn-Phong specular lighting using uniforms (lightPos, viewPos, material and light colors, shininess).
• Applies gamma correction (γ = 2.2)

### Host Program (src/main_Phong_Shader.cpp):
• Initializes GLFW/GLEW, creates an 800×800 window.
• Loads and links the two shaders.
• Builds a unit-sphere mesh (32×16 tesselation, identical to HW5).
• Uploads positions and normals to VBOs and indices to an EBO under one VAO.
• In each frame, sets model (scale 2, translate z = –7), view (identity), and projection (60° FOV, aspect 1:1).
• Computes normalMatrix and sends all Phong parameters exactly as in HW6

# Result
When running, an 800×800 OpenGL window opens showing the green sphere with smooth specular highlights under a white point light at (–4,4,–3). Gamma correction ensures correct brightness.

![Result_image](https://github.com/user-attachments/assets/348f324f-5b17-41cd-89e6-1dc90db0f8f8)


# Compilation & Run Instructions

### Requirements
Visual Studio 2022 (or newer) on Windows
GLEW (headers + glew32.lib, glew32.dll)
GLFW (headers + glfw3.lib, glfw3.dll)
GLM (headers only)

### Build Steps

1. Open CG_assignment7.sln in Visual Studio 2022.
2. In Project → Properties:
 -> C/C++ → General → Additional Include Directories:
    <path-to>\glew-2.x.x\include
    <path-to>\glfw-3.x.x\include
    <path-to>\glm-0.9.x
 -> Linker → General → Additional Library Directories:
    <path-to>\glew-2.x.x\lib\Release\x64
    <path-to>\glfw-3.x.x\lib-vc2022
 -> Linker → Input → Additional Dependencies:
     glew32.lib
    glfw3.lib
    opengl32.lib
    
3. Copy glew32.dll and glfw3.dll into the output folder (Debug\x64\ or Release\x64\).
4. Set Platform to x64, Configuration to Debug or Release.
5. Build (Ctrl+Shift+B) and run (Ctrl+F5).
  
# Files 

### main_Phong_Shader.cpp : 
C++ host program: GLFW/GLEW initialization, sphere mesh generation, VAO/VBO/EBO setup, render loop, uniform updates, cleanup.

### shaders/Phong.vert
Vertex shader: transforms vertices, computes normals, outputs FragPos and Normal.

### shaders/Phong.frag
Fragment shader: performs ambient + diffuse + specular (Blinn-Phong) lighting, gamma correction.
