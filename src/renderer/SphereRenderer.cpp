#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "src/renderer/Renderer.cpp"
#include "src/geometry/Sphere.cpp"
// #include "src/math/Matrix.cpp"

class SphereRenderer : virtual public Renderer {
private:
    Sphere *sphere;
public:
    SphereRenderer(unsigned int width, unsigned int height, const char* vertexShader, const char* fragmentShader, Sphere *sphere) 
    : Renderer("3D Sphere - main", width, height, vertexShader, fragmentShader) {
        this->sphere = sphere;
    }

    void generateBuffer() {
        this->vertices = sphere->getVertices();
        this->indices = sphere->getIndices();
    }

    void drawElements() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float lightPos[3] = { 1.5f, 0.7f, 2.2f }; 
        float lightIntensity[3] = { 10.0f, 10.0f, 10.0f }; 
        float objectColor[3] = { 0.31f, 0.5f, 1.0f }; 

        float albedo = 0.6f;

        // Set the camera position
        float cameraPos[3] = { 0.0f, 1.0f, 3.0f };

        // Set up matrices using the custom functions
        Mat4 model = Matrix::translateMatrix(0.0f, 0.0f, 0.0f);
        Mat4 view = Matrix::lookAtMatrix({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
        Mat4 projection = Matrix::perspectiveMatrix(45.0f * M_PI / 180.0f, 800.0f / 600.0f, 0.1f, 100.0f);

        // Use shader program
        glUseProgram(shaderProgram);

        // Set light and material uniforms in the shader
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, lightPos);
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.intensity"), 1, lightIntensity);
        glUniform1f(glGetUniformLocation(shaderProgram, "albedo"), albedo);
        glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, objectColor);
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, cameraPos);

        // Set uniforms
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.m);

        // Bind VAO and draw sphere
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
};