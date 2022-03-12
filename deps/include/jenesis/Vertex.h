#pragma once

#include <glm/glm.hpp>

// A great thing about structs is that their memory layout is sequential for all its items.
// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
// again translates to 3/2 floats which translates to a byte array.

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;


	Vertex(glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 normal = glm::vec3(0,0,0)) :
		pos(pos),
		normal(normal){}
};