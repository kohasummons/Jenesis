

#pragma once


#include "stb_image.h"

#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <jenesis/Shader.h>
#include <jenesis/Vertex.h>
#include <jenesis/TexVertex.h>
#include <jenesis/Light.h>

#include <string>
#include <vector>



class PlaneGround {

    public:

        // Default Constructor
        PlaneGround(){}


        // constructor
        PlaneGround(glm::mat4 view, glm::mat4 projection, const std::string& texturePath)
        {
            this->view = view;
            this->projection = projection;

            this->model = glm::mat4(1.0f);

            fillGrid();

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupTerrain(positions, texCoords, normals);

            LoadTexture(texturePath);   
        }

        void setMatrices(glm::mat4 projection)
        {
            this->projection = projection;

            this->shader = new Shader("res/shaders/plane_ground_vert.glsl", "res/shaders/plane_ground_frag.glsl");
            shader->use();
            shader->setMat4("projection", projection);
        }

        void rotate(float dt, glm::vec3 amount)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, dt, amount);
            model = model * trans;
        }

        void translate(glm::vec3 amount)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, amount);
            model = model * trans;
        }

        void scale(float x, float y, float z)
        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::scale(trans, glm::vec3(x, y, z));
            model = model * trans;
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

        // render the mesh
        void Draw(Camera camera, Light *light, float ambience)
        {
            // glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture);

            this->shader->use();
            this->view = camera.GetViewMatrix();
            shader->setMat4("view", view);

            glBindVertexArray(VAO);

            // retrieve the matrix uniform locations
            unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
            // pass them to the shaders
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            shader->setVec3("lightPosition", light->position);
            shader->setVec3("lightColor", light->color);
            shader->setFloat("ambience", ambience);


            glDrawElements(GL_TRIANGLES, IND, GL_UNSIGNED_INT, NULL);
        }

        void LoadTexture(const std::string& texturePath)
        {
            glGenTextures(1, &m_texture);
            glBindTexture(GL_TEXTURE_2D, m_texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load image, create texture and generate mipmaps
            int width, height, nrChannels;
            unsigned char *data = stbi_load((texturePath).c_str(), &width, &height, &nrChannels, 0);

            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }

            stbi_image_free(data);
        }

    private:

        glm::mat4 model, view, projection;

        unsigned int m_texture;

        // render data 
        unsigned int VAO;
        unsigned int VBO[4];

        unsigned int n = 5;

        unsigned int vertSize = n*n;

        glm::vec3* positions = new glm::vec3[vertSize];
        glm::vec2* texCoords = new glm::vec2[vertSize];
        glm::vec3* normals   = new glm::vec3[vertSize];

        unsigned int IND = 6 * (n-1) * (n-1);   // Number of indices
        unsigned int* indices = new unsigned int[IND];

        glm::vec3* positionPtr = positions;
        glm::vec2* texCoordPtr = texCoords;
        glm::vec3* normalPtr   = normals;
        unsigned int* indexPtr = indices;

        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec4 color;

        Shader* shader;



        void fillGrid()
        {
            for(unsigned int x=0 ; x<n ; x++) for(unsigned int z=0; z<n ; z++)
            {
                *positionPtr++ = glm::vec3( (z / (float)n) - 0.5f, 0, (x / (float)n) - 0.5f);

                *texCoordPtr++ = glm::vec2((z / (float)n) * n, (x / (float)n) * n);

                *normalPtr++ = upVector;
            }

            for(unsigned int x=0 ; x<n-1 ; x++) for(unsigned int z=0; z<n-1 ; z++)
            {
                *indexPtr++ = (x * n) + z;          
                *indexPtr++ = ((x + 1) * n) + z;    
                *indexPtr++ = ((x + 1) * n) + z + 1;

                *indexPtr++ = (x * n) + z;          
                *indexPtr++ = ((x + 1) * n) + z + 1;
                *indexPtr++ = (x * n) + z + 1;
            }
        }



        // initializes all the buffer objects/arrays
        void setupTerrain(glm::vec3* positions, glm::vec2* texCoords, glm::vec3* normals)
        {
            // create our vertex array object
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(4, VBO);

            // our terrain vertex positions are known so we can set those right away
            glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertSize, positions, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            // we've also computed our base texture coordinates, so fill those in
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertSize, texCoords, GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            // our normals are also computed, so pass those in too
            glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertSize, normals, GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

            // our terrain indices define how our terrain should be constructed using the vertices defined above
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * IND, indices, GL_STATIC_DRAW);

            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
};