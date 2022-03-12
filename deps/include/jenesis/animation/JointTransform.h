

#pragma once


#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>


class JointTransform
{
public:

    // Default Constructor
    JointTransform(){}

    JointTransform(glm::vec3 position, Quaternion rotation)
    {
        this->position = position;
        this->rotation = rotation;
    }




private:

    // remember, this position and rotation are relative to the parent bone!
    glm::vec3 position;
    Quaternion rotation;

    static glm::vec3 interpolate(glm::vec3 start, glm::vec3 end, float progression) {
        float x = start.x + (end.x - start.x) * progression;
        float y = start.y + (end.y - start.y) * progression;
        float z = start.z + (end.z - start.z) * progression;

        return glm::vec3(x, y, z);
    }

protected:


    glm::mat4 getLocalTransform()
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, position);
        matrix = matrix * rotation.toRotationMatrix();

        return matrix;
    }

    static JointTransform interpolate(JointTransform frameA, JointTransform frameB, float progression)
    {
        glm::vec3 pos = interpolate(frameA.position, frameB.position, progression);
        Quaternion rot = Quaternion.interpolate(frameA.rotation, frameB.rotation, progression);

        return JointTransform(pos, rot);
    }
};