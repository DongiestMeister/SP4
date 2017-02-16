#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene() { b_isPaused = false; }
	virtual ~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void Pause() {}
	virtual void Resume() {}
	bool b_isPaused;
};

#endif // SCENE_H