
#pragma once

#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <jenesis/Shader.h>
#include <jenesis/Vertex.h>

#include <string>
#include <vector>

using namespace std;

struct mat4
{
	float m[16];
};

struct float3
{
	float3(){}

	float3(float x, float y, float z) :
		x(x), y(y), z(z)
	{}

	float x, y, z;
};

struct uint3
{
	uint3(){}

	uint3(unsigned int x, unsigned y, unsigned int z) :
		x(x), y(y), z(z)
	{}

	unsigned int x, y, z;
};


class Terrain {

    public:

        // Default Constructor
        Terrain(){}


        // constructor 
        Terrain(Vertex* vertices, unsigned int* indices, int vertSize, int indexSize, glm::vec4 color = glm::vec4(0.8f, 0.7f, 0.3f, 1))
        {
            model = glm::mat4(1.0f);

            this->color = color;

            this->vertSize = vertSize;
            this->indexSize = indexSize;

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupTerrain(vertices, indices);
        }


        glm::vec4 getColor()
        {
            return this->color;
        }

        void setColor(glm::vec4 color)
        {
            this->color = color;
        }

        void setColor(float red, float green, float blue, float alpha)
        {
            this->color.x = red;
            this->color.y = green;
            this->color.z = blue;
            this->color.w = alpha;
        }

        void scale(float x, float y, float z)
        {
            model = glm::scale(model, glm::vec3(x, y, z));
        }

        void translate(glm::vec3 amount)
        {
            model = glm::translate(model, amount);
        }

        void rotate(float dt, glm::vec3 amount)
        {
            model = glm::rotate(model, dt, amount);
        }

        // render the mesh
        void Draw(Shader shader)
        {
            glBindVertexArray(VAO);

            // retrieve the matrix uniform locations
            unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
            // pass them to the shaders
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // retrieve color location
            unsigned int colorLoc = glGetUniformLocation(shader.ID, "aColor");
            // pass color to shader
            glUniform4f(colorLoc, color.x, color.y, color.z, color.w);

            glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
        }

    private:


        // render data
        unsigned int VBO, EBO, VAO;

        glm::vec4 color;

        int vertSize;
        int indexSize;

        // Model Matrix
        glm::mat4 model;



        // initializes all the buffer objects/arrays
        void setupTerrain(Vertex* vertices,  unsigned int* indices)
        {

            // create buffers/arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            // vertex normals
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (3 * sizeof(float)) );

            glBindVertexArray(0);
        }
};