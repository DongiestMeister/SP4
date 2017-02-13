#pragma once
#include <lua.hpp>
#include <vector>
#include "Vector3.h"

#include <SingletonTemplate.h>

class CLuaInterface : public Singleton<CLuaInterface>
{
	friend Singleton<CLuaInterface>;
protected:
	CLuaInterface();
	~CLuaInterface();
public:
	bool Init();
	void Run();
	void Drop();

	int getIntValue(const char* varName);
	float getFloatValue(const char* varName);

	void SaveIntValue(const char* varName,const int value, const bool bOverWrite = NULL);
	void SaveFloatValue(const char* varName, const float value, const bool bOverWrite = NULL);

	float GetField(const char *key);
	void GetGlobal(const char* varName);

	Vector3 GetVector3(const char *key);

	void error(const char *errorCode);

	void SetHighscore(int score);

	void SetAudio(bool value);
	bool GetAudio();


	std::vector<int> GetHighscores();

	lua_State *theLuaState;
	lua_State *theErrorState;
	lua_State *theScoreState;
	lua_State *theOptionState;
	lua_State *theOBJState;
};