
#pragma once




#include <jenesis/animation/AnimatedModel.h>
#include <jenesis/animation/KeyFrames.h>

#include <string>
#include <map>


struct KeyFrame
{
	float timeStamp;
	std::map<std::string, JointTransform> pose;


	KeyFrame(float timeStamp, std::map<Std::string, JointTransform> pose) :
		timeStamp(timeStamp),
		pose(pose)
		{}
};