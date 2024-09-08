#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "src/shader/bsdf/vertexShader.cpp"
#include "src/shader/bsdf/fragmentShader.cpp"
#include "src/renderer/SphereRenderer.cpp"

// settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const float RADIUS = 1.0f;
const unsigned int SECTOR_COUNT = 36;
const unsigned int STACK_COUNT = 18; 

int main() {
    Sphere sphere = Sphere(RADIUS, SECTOR_COUNT, STACK_COUNT);
    SphereRenderer renderer = SphereRenderer(WIDTH, HEIGHT, vertexShaderSource, fragmentShaderSource, &sphere);

    renderer.render();
    return 0;
}