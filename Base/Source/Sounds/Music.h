#ifndef MUSIC_H
#define MUSIC_H

#include "Vector3.h"
#include <irrKlang.h>
#include <string>
#include "SingletonTemplate.h"

using std::string;
using namespace irrklang;

class Music : public Singleton<Music>
{
	friend Singleton<Music>;
public:
	void SetListener(Vector3 pos,Vector3 view);
	ISound* playSound(string fileName, bool loop = false,bool startPaused = false,bool track = false);
	ISound* play3DSound(string fileName, float x, float y, float z, bool loop = false, bool startPaused = false, bool track = false);
	void Init();
	void Exit();
	ISoundEngine* theSoundEngine;
private:
	Music();
	~Music();
};

#endif