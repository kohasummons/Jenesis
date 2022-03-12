

#pragma once


#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <jenesis/Shader.h>
#include <jenesis/animation/Joint.h>
#include <jenesis/animation/Animator.h>
#include <jenesis/Model.h>

#include <string>
#include <vector>



class AnimatedModel
{
public:

    // Default Constructor
    AnimatedModel(){}
    
    AnimatedModel(ModelMesh model, Joint rootJoint, int jointCount)
    {
        this->model = model;
        this->rootJoint = rootJoint;
        this->jointCount = jointCount;
        this->animator = new Animator(this);
        rootJoint.calcInverseBindTransform(glm::mat4(1.0f));
    }

    ModelMesh getModel()
    {
        return model;
    }

    Joint getRootJoint()
    {
        return rootJoint;
    }

    void doAnimation(Animation animation)
    {
        animator->doAnimation(animation);
    }

    void update()
    {
        animator->update();
    }

    glm::mat4[] getJointTransforms()
    {
        glm::mat4* jointMatrices = new glm::mat4[jointCount];
        addJointsToArray(rootJoint, jointMatrices);

        return jointMatrices;
    }


private:

    // skin
    const ModelMesh model;

    // skeleton
    const Joint rootJoint;
    const int jointCount;

    const Animator* animator;

    

    void addJointsToArray(Joint headJoint, glm::mat4[] jointMatrices)
    {
        jointMatrices[headJoint.index] = headJoint.getAnimatedTransform();
        for (Joint childJoint : headJoint.children)
        {
            addJointsToArray(childJoint, jointMatrices);
        }
    }
};