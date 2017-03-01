#pragma once
#include <lua.hpp>
#include <vector>
#include "Vector3.h"
#include <string>

using std::string;

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

	int getIntValue(const char* varName, lua_State *state);
	float getFloatValue(const char* varName, lua_State *state);
	string getStringValue(const char* varName, lua_State *state);

	void SaveIntValue(const char* varName, const int value, const bool bOverWrite = NULL, string filepath = "");
	void SaveFloatValue(const char* varName, const float value, const bool bOverWrite = NULL, string filepath = "");
	void SaveStringValue(const char* varName, const string value, const bool bOverWrite = NULL, string filepath = "");

	float GetField(const char *key);
	void GetGlobal(const char* varName);

	Vector3 GetVector3(const char *key);

	void error(const char *errorCode);


	lua_State *theLuaState;
	lua_State *theErrorState;
	lua_State *theCharactersState;
	lua_State *thePlayerState;

};