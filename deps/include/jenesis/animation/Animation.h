
#pragma once




#include <jenesis/animation/AnimatedModel.h>
#include <jenesis/animation/KeyFrames.h>


struct Animation
{
	float length;
	Keyframe[] frames;


	Animation(float length, KeyFrame[] frames) :
		length(length),
		frames(frames)
		{}
};