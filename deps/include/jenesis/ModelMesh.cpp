
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#include "TexVertex.h"
#include "ModelMesh.h"
#include "Util.h"
#include "DebugTimer.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>



ModelMesh::ModelMesh()
	{}

ModelMesh::ModelMesh(const std::string& fileName, const std::string& texturePath)
{
    InitModelMesh(OBJModel(fileName).ToIndexedModel());

    model = glm::mat4(1.0f);

    LoadTexture(texturePath);
}

void ModelMesh::setMatrices(glm::mat4 projection)
{
    this->projection = projection;

    shader = new Shader("res/shaders/objmesh_vert.glsl", "res/shaders/objmesh_frag.glsl");
    shader->use();
    shader->setMat4("projection", projection);
}

void ModelMesh::InitModelMesh(const IndexedModel& model)
{
    m_numIndices = model.indices.size();

    glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ModelMesh::LoadTexture(const std::string& texturePath)
{
	int width, height, numComponents;
    unsigned char* data = stbi_load((texturePath).c_str(), &width, &height, &numComponents, 4);

    if(data == NULL)
		std::cerr << "Unable to load texture: " << texturePath << std::endl;
        
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

void ModelMesh::Update()
{
	// glBindTexture(GL_TEXTURE_2D, m_texture);
}

ModelMesh::ModelMesh(TexVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    IndexedModel model;

	for(unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}
	
	for(unsigned int i = 0; i < numIndices; i++)
        model.indices.push_back(indices[i]);

    InitModelMesh(model);
}

ModelMesh::~ModelMesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void ModelMesh::Draw(Camera camera, Light *light, float ambience)
{
    shader->use();
    view = camera.GetViewMatrix(); //
    shader->setMat4("view", view);


	glBindTexture(GL_TEXTURE_2D, m_texture);

    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    // pass them to the shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    shader->setVec3("lightPosition", light->position);
    shader->setVec3("lightColor", light->color);
    shader->setFloat("ambience", ambience); 

	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	// glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}


void ModelMesh::translate(glm::vec3 amount)
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, amount);
    model = model * transform;
}

void ModelMesh::rotate(float dt, glm::vec3 amount)
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, dt, amount);
    model = model * transform;
}

void ModelMesh::scale(float x, float y, float z)
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(x, y, z));
    model = model * transform;
}