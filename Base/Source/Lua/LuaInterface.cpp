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

		luaL_dofile(theLuaState, "Image//GameSettings.lua");

		result = true;
	}

	theErrorState = lua_open();

	if ((theLuaState) && (theErrorState))
	{
		luaL_openlibs(theErrorState);

		luaL_dofile(theErrorState, "Image//errorLookup.lua");
	}

	theScoreState = lua_open();

	if (theScoreState)
	{
		luaL_openlibs(theScoreState);

		luaL_dofile(theScoreState, "Image//DM2240_HighScore.lua");
	}

	theOptionState = lua_open();

	if (theOptionState)
	{
		luaL_openlibs(theOptionState);

		luaL_dofile(theOptionState, "Image//Options.lua");
	}

	theOBJState = lua_open();

	if (theOBJState)
	{
		luaL_openlibs(theOBJState);

		luaL_dofile(theOBJState, "Image//OBJs.lua");
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
	if (theScoreState)
	{
		lua_close(theScoreState);
	}
	if (theOptionState)
	{
		lua_close(theOptionState);
	}
	if (theOBJState)
	{
		lua_close(theOBJState);
	}
}

int CLuaInterface::getIntValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return lua_tointeger(theLuaState, -1);
}

float CLuaInterface::getFloatValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return (float)lua_tonumber(theLuaState, -1);
}

void CLuaInterface::SaveIntValue(const char* varName, const int value, const bool bOverWrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s= %d\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverWrite);
	lua_call(theLuaState, 2, 0);
}

void CLuaInterface::SaveFloatValue(const char* varName, const float value, const bool bOverWrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s= %6.4f\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverWrite);
	lua_call(theLuaState, 2, 0);
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

std::vector<int> CLuaInterface::GetHighscores()
{
	std::vector<int> temp;

	lua_getglobal(theScoreState, "Highscores");

	if (!lua_istable(theScoreState, -1))
		error("error100");

	lua_pushstring(theScoreState, "one");
	lua_gettable(theScoreState, -2);
	if (!lua_isnumber(theScoreState, -1))
		error("error101");
	temp.push_back((int)lua_tonumber(theScoreState, -1));

	lua_pop(theScoreState, 1);

	lua_pushstring(theScoreState, "two");
	lua_gettable(theScoreState, -2);
	if (!lua_isnumber(theScoreState, -1))
		error("error101");
	temp.push_back((int)lua_tonumber(theScoreState, -1));

	lua_pop(theScoreState, 1);

	lua_pushstring(theScoreState, "three");
	lua_gettable(theScoreState, -2);
	if (!lua_isnumber(theScoreState, -1))
		error("error101");
	temp.push_back((int)lua_tonumber(theScoreState, -1));

	lua_pop(theScoreState, 1);

	return temp;
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

void CLuaInterface::SetHighscore(int score)
{
	std::vector<int> temp = GetHighscores();
	if (score > temp[0])
	{
		temp[2] = temp[1];
		temp[1] = temp[0];
		temp[0] = score;
	}
	else if (score > temp[1])
	{
		temp[2] = temp[1];
		temp[1] = score;
	}
	else if (score > temp[2])
	{
		temp[2] = score;
	}

	string output = "Highscores = { one = " + std::to_string(temp[0]) + ", two = " + std::to_string(temp[1]) + ", three = " + std::to_string(temp[2]) + " }";

	lua_getglobal(theLuaState, "SaveToLuaFile");
	lua_pushstring(theLuaState, output.c_str());
	lua_pushinteger(theLuaState, true);
	lua_call(theLuaState, 2, 0);

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

