
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <jenesis/Vertex.h>

class Shapes
{
	public:

		//CUBE
		static int cubeVertSize;
		static int cubeIndexSize;
		static Vertex cubeVertices[36];
		static unsigned int cubeIndices[36];
};