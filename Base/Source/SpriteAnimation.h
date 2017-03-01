#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include "Mesh.h"
#include <vector>

struct Animation
{
	Animation() {}
	~Animation() {}

	int startFrame, endFrame, repeatCount;
	float animTime;
	bool ended, animActive;

	void Set(int startFrame, int endFrame, int repeat, float time, bool active)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		repeatCount = repeat;
		animTime = time;
		animActive = active;
	}
};

class SpriteAnimation : public Mesh
{
public:
	SpriteAnimation(const std::string &meshName, int row, int col);
	~SpriteAnimation();
	void Update(double dt);
	virtual void Render();

	int m_row;
	int m_col;

	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	

	Animation* m_anim;
};
#endif SPRITE_ANIMATION_H