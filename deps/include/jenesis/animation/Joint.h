

#pragma once


#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <jenesis/Shader.h>

#include <string>
#include <vector>



class Joint
{
public:


    const int index;// ID
    const std::string name;
    const std::vector<Joint> children;

    // Default Constructor
    Joint(){}


    Joint(int index, std::string name, glm::mat4 bindLocalTransform)
    {
        this->index = index;
        this->name = name;
        this->localBindTransform = bindLocalTransform;
    }

    void addChild(Joint child)
    {
        this->children.push_back(child);
    }

    glm::mat4 getAnimatedTransform()
    {
        return animatedTransform;
    }

    void setAnimationTransform(glm::mat4 animationTransform)
    {
        this->animatedTransform = animationTransform;
    }

    glm::mat4 getInverseBindTransform()
    {
        return inverseBindTransform;
    }



private:

    glm::mat4 animatedTransform;
    glm::mat4 localBindTransform;
    glm::mat4 inverseBindTransform;


protected:

    void calcInverseBindTransform(glm::mat4 parentBindTransform)
    {
        glm::mat4 bindTransform = parentBindTransform * localBindTransform;
        inverseBindTransform = glm::inverse(bindTransform);
        
        for (Joint child : children)
        {
            child.calcInverseBindTransform(bindTransform);
        }
    }
};