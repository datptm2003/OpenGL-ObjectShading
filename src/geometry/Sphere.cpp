#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/math/constant.cpp"
#include "src/math/Matrix.cpp"

#include <iostream>
#include <vector>

class Sphere {
private:
    std::vector<float> vertices; 
    std::vector<unsigned int> indices;

    float radius;
    unsigned int sectorCount;
    unsigned int stackCount;

public:
    Sphere(float radius, unsigned int sectorCount, unsigned int stackCount) {
        this->radius = radius;
        this->sectorCount = sectorCount;
        this->stackCount = stackCount;
        generateSphere();
    }

    Sphere(Sphere* sphere) {
        this->radius = sphere->getRadius();
        this->sectorCount = sphere->getSectorCount();
        this->stackCount = sphere->getStackCount();
        generateSphere();
    }

    std::vector<float> getVertices() {
        return vertices;
    }
    std::vector<unsigned int> getIndices() {
        return indices;
    }

    float getRadius() {
        return radius;
    }
    unsigned int getSectorCount() {
        return sectorCount;
    }
    unsigned int getStackCount() {
        return stackCount;
    }

private:
    void generateSphere() {
        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;

        for (unsigned int i = 0; i <= stackCount; ++i) {
            stackAngle = M_PI / 2 - i * stackStep;        // from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            for (unsigned int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;           // from 0 to 2pi

                x = xy * cosf(sectorAngle);             // x = r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // y = r * cos(u) * sin(v)
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                vertices.push_back(nx);
                vertices.push_back(ny);
                vertices.push_back(nz);
            }
        }

        // indices
        unsigned int k1, k2;
        for (unsigned int i = 0; i < stackCount; ++i) {
            k1 = i * (sectorCount + 1); // beginning of current stack
            k2 = k1 + sectorCount + 1;  // beginning of next stack

            for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }

};