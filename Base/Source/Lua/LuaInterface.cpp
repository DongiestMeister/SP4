#include "LuaInterface.h"
#include <iostream>
#include <string>
using namespace std;

CLuaInterface::CLuaInterface()
{

}

CLuaInterface::~CLuaInterface()
{
	Drop();
}

bool CLuaInterface::Init()
{
	bool result = false;

	theLuaState = lua_open();

	if (theLuaState)
	{
		luaL_openlibs(theLuaState);

		luaL_dofile(theLuaState, "Image//Lua//GameSettings.lua");

		result = true;
	}

	theErrorState = lua_open();

	if ((theLuaState) && (theErrorState))
	{
		luaL_openlibs(theErrorState);

		luaL_dofile(theErrorState, "Image//Lua//errorLookup.lua");
	}

	theCharactersState = lua_open();

	if ((theCharactersState))
	{
		luaL_openlibs(theCharactersState);

		luaL_dofile(theCharactersState, "Image//Lua//Characters.lua");
	}

	thePlayerState = lua_open();

	if ((thePlayerState))
	{
		luaL_openlibs(thePlayerState);

		luaL_dofile(thePlayerState, "Image//Lua//Player.lua");
	}

	return result;
}

void CLuaInterface::Run()
{
	if (theLuaState == NULL)
	{
		return;
	}

	lua_getglobal(theLuaState, "title");

	const char *title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);

	cout << title << endl;
	cout << "-------------------------------" << endl;
	cout << "Width of screen: " << width << endl;
	cout << "Height of screen: " << height << endl;
}

void CLuaInterface::Drop()
{
	if (theLuaState)
	{
		lua_close(theLuaState);
	}
	if (theErrorState)
	{
		lua_close(theErrorState);
	}
	if (theCharactersState)
	{
		lua_close(theCharactersState);
	}
	if (thePlayerState)
	{
		lua_close(thePlayerState);
	}
}

int CLuaInterface::getIntValue(const char* varName, lua_State *state)
{
	lua_getglobal(state, varName);
	return lua_tointeger(state, -1);
}

float CLuaInterface::getFloatValue(const char* varName, lua_State *state)
{
	lua_getglobal(state, varName);
	return (float)lua_tonumber(state, -1);
}

string CLuaInterface::getStringValue(const char* varName, lua_State *state)
{
	lua_getglobal(state, varName);
	return lua_tostring(state, -1);
}

void CLuaInterface::SaveIntValue(const char* varName, const int value, const bool bOverWrite, string filepath)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s= %d\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverWrite);
	lua_pushstring(theLuaState, filepath.c_str());
	lua_call(theLuaState, 3, 0);
}

void CLuaInterface::SaveFloatValue(const char* varName, const float value, const bool bOverWrite, string filepath)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s= %6.4f\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverWrite);
	lua_pushstring(theLuaState, filepath.c_str());
	lua_call(theLuaState, 3, 0);
}

void CLuaInterface::SaveStringValue(const char* varName, const string value, const bool bOverWrite, string filepath)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	string outputString = (string)varName + "=\"" + value + "\"";
	lua_pushstring(theLuaState, outputString.c_str());
	lua_pushinteger(theLuaState, bOverWrite);
	lua_pushstring(theLuaState, filepath.c_str());
	lua_call(theLuaState, 3, 0);
}

void CLuaInterface::GetGlobal(const char* varName)
{
	lua_getglobal(theLuaState, varName);
}

float CLuaInterface::GetField(const char* key)
{
	int result;

	if (!lua_istable(theLuaState, -1))
		error("error100");

	lua_pushstring(theLuaState, key);
	lua_gettable(theLuaState, -2);
	if (!lua_isnumber(theLuaState, -1))
		error("error101");
	result = (int)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return result;
}


void CLuaInterface::error(const char *errorCode)
{
	if (theErrorState == NULL)
	{
		return;
	}

	lua_getglobal(theErrorState, errorCode);
	const char *errorMsg = lua_tostring(theErrorState, -1);

	if (errorMsg != NULL)
	{
		cout << errorMsg << endl;
	}
	else
	{
		cout << errorCode << " is not valid.\nPlease forward this to me :)" << endl;
	}
}

Vector3 CLuaInterface::GetVector3(const char *key)
{
	GetGlobal(key);
	Vector3 temp;
	temp.x = GetField("x");
	temp.y = GetField("y");
	temp.z = GetField("z");

	return temp;
}

