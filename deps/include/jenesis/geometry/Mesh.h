

#pragma once


#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <jenesis/Shader.h>
#include <jenesis/Vertex.h>
#include <jenesis/Camera.h>
#include <jenesis/Light.h>

#include <string>
#include <vector>

using namespace std;


class Mesh 
{

    public:

        // Default Constructor
        Mesh(){}


        // constructor 
        Mesh(Vertex* vertices, int vertSize, glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 1))
        {
            model = glm::mat4(1.0f);

            Mesh::color = color;

            Mesh::vertSize = vertSize;

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupMesh(vertices);
        }

        void setMatrices(glm::mat4 projection)
        {
            this->projection = projection;

            shader = new Shader("res/shaders/smoothSH_vert.glsl", "res/shaders/smoothSH_frag.glsl");
            shader->use();
            shader->setMat4("projection", projection);
        }


        glm::vec4 getColor()
        {
            return this->color;
        }

        void setColor(glm::vec4 color)
        {
            Mesh::color = color;
        }

        void setColor(float red, float green, float blue, float alpha)
        {
            this->color.x = red;
            this->color.y = green;
            this->color.z = blue;
            this->color.w = alpha;
        }


        void translate(glm::vec3 amount)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, amount);
            model = model * trans;
        }

        void rotate(float dt, glm::vec3 amount)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, dt, amount);
            model = model * trans;
        }

        void scale(float x, float y, float z)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::scale(trans, glm::vec3(x, y, z));
            model = model * trans;
        }

        glm::mat3 getRotation()
        {
            glm::mat3 rotMatrix = glm::mat3(1.0f);

            for (int i=0 ; i<3 ; i++)
            {
                for (int j=0 ; j<3 ; j++)
                {
                    rotMatrix[i][j] = model[i][j];
                } 
            }

            float sx = glm::length(glm::vec3(model[0][0], model[1][0], model[2][0]));
            float sy = glm::length(glm::vec3(model[0][1], model[1][1], model[2][1]));
            float sz = glm::length(glm::vec3(model[0][2], model[1][2], model[2][2]));

            for (int i=0 ; i<3 ; i++)
                rotMatrix[i][0] /= sx;
            for (int i=0 ; i<3 ; i++)
                rotMatrix[i][1] /= sy;
            for (int i=0 ; i<3 ; i++)
                rotMatrix[i][2] /= sz;

            return rotMatrix;
        }

        glm::vec3 getTranslation()
        {
            float sx = glm::length(glm::vec3(view[0][0], view[1][0], view[2][0]));
            float sy = glm::length(glm::vec3(view[0][1], view[1][1], view[2][1]));
            float sz = glm::length(glm::vec3(view[0][2], view[1][2], view[2][2]));

            return glm::vec3(sx, sy, sz);
        }

        glm::vec3 getScale()
        {
            return glm::vec3(model[0][3], model[1][3], model[2][3]);
        }


        // render the mesh
        void Draw(Camera camera, Light *light, float ambience)
        {
            shader->use();
            view = camera.GetViewMatrix();
            shader->setMat4("view", view);


            glBindVertexArray(VAO);

            // retrieve the matrix uniform locations
            unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
            // pass them to the shaders
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // retrieve color location
            unsigned int colorLoc = glGetUniformLocation(shader->ID, "aColor");
            // pass color to shader
            glUniform4f(colorLoc, color.x, color.y, color.z, color.w);

            shader->setVec3("lightPosition", light->position);
            shader->setVec3("lightColor", light->color);
            shader->setFloat("ambience", ambience);

            glDrawArrays(GL_TRIANGLES, 0, vertSize);
        }

    private:

        // render data 
        unsigned int VBO, VAO;

        // MVP Matrices
        glm::mat4 model, view, projection;

        Shader* shader;

        glm::vec4 color;

        int vertSize;


        // initializes all the buffer objects/arrays
        void setupMesh(Vertex* vertices)
        {
            // create buffers/arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
            glBindVertexArray(VAO);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (3 * sizeof(float)) );

            glBindVertexArray(0);
        }
};