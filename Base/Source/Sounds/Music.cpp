#include "Music.h"

Music::Music()
{
	theSoundEngine = createIrrKlangDevice();
	theSoundEngine->setDefault3DSoundMinDistance(30.f);
}

Music::~Music()
{
	if (theSoundEngine != NULL)
	{
		theSoundEngine->drop();
	}
}

void Music::Init()
{

}

void Music::SetListener(Vector3 pos,Vector3 view)
{
	theSoundEngine->setListenerPosition(vec3df(pos.x,pos.y,pos.z),vec3df(view.x,view.y,view.z));
}

ISound* Music::playSound(string fileName,bool loop,bool startPaused,bool track)
{
	return theSoundEngine->play2D(fileName.c_str(), loop, startPaused,track);
}

ISound* Music::play3DSound(string fileName, float x, float y, float z,bool loop, bool startPaused, bool track)
{
	return theSoundEngine->play3D(fileName.c_str(), vec3df(x, y, z), loop, startPaused,track);
}

