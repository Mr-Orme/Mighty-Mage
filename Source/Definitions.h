#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>
#include <map>
#include <memory>

class ResourceManager;
class View;
class ArtAssetLibrary;
class GameObject;

//Basic Types
typedef unsigned int	Uint32;
typedef float			GAME_FLT;
typedef Uint32			GAME_INT;

typedef struct GAME_VEC
{
	GAME_FLT x;
	GAME_FLT y;
} GAME_VEC;


enum GAME_DIRECTION {N =0, E = 90, S = 180, W = 270};

//make levels an array of resource managers! or something that has everything!
enum GAME_LEVEL {GAME_LEVEL_MAIN, GAME_LEVEL_BASEMENT,};

typedef struct GAME_OBJECTFACTORY_PRESETS
{
	std::string objectType;
	GAME_VEC position;
	GAME_FLT angle;
	GAME_INT health;//move to new AssetLibraryGame
	GAME_INT destructTime;//move to new AssetLibraryGame
	ResourceManager* devices;
	std::map<GAME_DIRECTION, bool> gDirection;
	//std::shared_ptr<GameObject> joinedObject; //only needed for joint
}GAME_OBJECTFACTORY_PRESETS;

typedef struct GAME_OBJECT_STATS
{
	int health;
}GAME_OBJECT_STATS;

typedef struct GAME_RGBA
{
	GAME_INT R;
	GAME_INT G;
	GAME_INT B;
	GAME_INT A;
}GAME_RGBA;

//Constants
const GAME_FLT PI = 3.14159f;
const float fPRV = 10.0f;



#endif