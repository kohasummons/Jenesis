// #ifndef MESH_INCLUDED_H
// #define MESH_INCLUDED_H

#pragma once

#include "stb_image.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "ObjLoader.h"
#include "TexVertex.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

class ModelMesh
{
	public:
		ModelMesh();
	    ModelMesh(const std::string& fileName, const std::string& texturePath);
		ModelMesh(TexVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

		void setMatrices(glm::mat4 projection);

		void Draw(Camera camera, Light *light, float ambience);

		void Update();

		void translate(glm::vec3 amount);
		void rotate(float dt, glm::vec3 amount);
		void scale(float x, float y, float z);

		virtual ~ModelMesh();

		
	protected:


	private:
		static const unsigned int NUM_BUFFERS = 4;
		// void operator=(const ModelMesh& mesh) {}
		ModelMesh(const ModelMesh& mesh) {}

		void LoadTexture(const std::string& texturePath);

	    void InitModelMesh(const IndexedModel& model);

		GLuint m_vertexArrayObject;
		GLuint m_vertexArrayBuffers[NUM_BUFFERS];
		unsigned int m_numIndices;

		GLuint m_texture;

	    // Model Matrix
	    glm::mat4 model;
	    glm::mat4 view, projection;

	    Shader* shader;
};

// #endif
