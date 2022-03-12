

#pragma once


#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>


class Animator
{
public:

    // Default Constructor
    Animator(){}

    Animator(AnimatedModel entity)
    {
        this->entity = entity;
    }

    void doAnimation(Animation animation)
    {
        this->animationTime = 0;
        this->currentAnimation = animation;
    }

    void update()
    {
        if (currentAnimation == NULL)
        {
            return;
        }
        increaseAnimationTime();
        std::map<std::string, glm::mat4> currentPose = calculateCurrentAnimationPose();
        applyPoseToJoints(currentPose, entity.getRootJoint(), glm::mat4(1.0f));
    }




private:

    AnimatedModel entity;

    Animation currentAnimation;
    float animationTime = 0;


    void increaseAnimationTime()
    {
        animationTime += DisplayManager.getFrameTime();
        if (animationTime > currentAnimation.getLength())
        {
            this->animationTime %= currentAnimation.getLength();
        }
    }

    std::map<std::string, glm::mat4> calculateCurrentAnimationPose()
    {
        KeyFrame[] frames = getPreviousAndNextFrames();
        float progression = calculateProgression(frames[0], frames[1]);

        return interpolatePoses(frames[0], frames[1], progression);
    }

    void applyPoseToJoints(std::map<std::string, glm::mat4> currentPose, Joint joint, glm::mat4 parentTransform)
    {
        glm::mat4 currentLocalTransform = currentPose.get(joint.name);
        glm::mat4 currentTransform = parentTransform * currentLocalTransform;
        
        for (Joint childJoint : joint.children)
        {
            applyPoseToJoints(currentPose, childJoint, currentTransform);
        }

        currentTransform = currentTransform * joint.getInverseBindTransform();
        joint.setAnimationTransform(currentTransform);
    }

    KeyFrame[] getPreviousAndNextFrames()
    {
        KeyFrame[] allFrames = currentAnimation.getKeyFrames();
        KeyFrame previousFrame = allFrames[0];
        KeyFrame nextFrame = allFrames[0];

        int number_of_frames = sizeof(allFrames) / sizeof(allFrames[0]);
        // int number_of_frames = *(&allFrames + 1) - allFrames;

        for (int i = 1; i < number_of_frames; i++)
        {
            nextFrame = allFrames[i];

            if (nextFrame.getTimeStamp() > animationTime)   break;

            previousFrame = allFrames[i];
        }

        return KeyFrame[] { previousFrame, nextFrame };
    }


    float calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame)
    {
        float totalTime = nextFrame.getTimeStamp() - previousFrame.getTimeStamp();
        float currentTime = animationTime - previousFrame.getTimeStamp();

        return currentTime / totalTime;
    }

    std::map<std::string, glm::mat4> interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression)
    {
        std::map<std::string, glm::mat4> currentPose;

        for (std::map<std::string, JointTransform>::iterator p_iter = previousFrame.pose.begin(), 
             std::map<std::string, JointTransform>::iterator n_iter = nextFrame.pose.begin() ; 
             p_iter!= previousFrame.pose.end(), n_iter!= nextFrame.pose.end() ;
             ++p_iter, ++n_iter)
        {
            JointTransform currentTransform = JointTransform.interpolate(p_iter->second, n_iter->second, progression);
            currentPose.insert(p_iter->first, currentTransform.getLocalTransform());
        }

        return currentPose;
    }

protected:


};