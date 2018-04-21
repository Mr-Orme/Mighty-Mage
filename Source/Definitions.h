/*
* Author:	Sean Orme
*			UALR
* 
* Date:		May, 2015 (Completed)
*
* File:		Definitions.h 
*
* Purpose:	This is a standard type redefinitions header that allows
*           for flexibility in programming should design parameters change.
*/

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

enum GAME_ROTATE_DIRECTION {GAME_ROTATE_LEFT = -1, GAME_ROTATE_RIGHT = 1};
enum GAME_OBJECT_SHAPE {GAME_RECTANGLE, GAME_CIRCLE};
enum GAME_BODY_TYPE {GAME_STATIC, GAME_KINEMATIC, GAME_DYNAMIC};
//Event Types
enum GAME_EVENT {GAME_NA,GAME_UP,GAME_DOWN,GAME_LEFT,GAME_RIGHT,GAME_SPACE, GAME_SHIFT, GAME_B, GAME_QUIT, GAME_NUM_EVENTS};
enum GAME_DIRECTION {N =0, E = 90, S = 180, W = 270};
enum GAME_COLORS {GAME_RED, GAME_YELLOW, GAME_BLUE, GAME_ORANGE, GAME_GREEN, GAME_PURPLE, GAME_NUM_COLORS};
enum GAME_LEVEL {GAME_LEVEL_MAIN, GAME_LEVEL_BASEMENT,};

enum GAME_COMPONENT_LIST 
{
	GAME_BODY_COMP, 
	GAME_HEALTH_COMP, 
	GAME_RENDERER_COMP, 
	GAME_USERINPUT_COMP, 
	GAME_BACKPACK_COMP,
	GAME_INVENTORY_COMP,
	GAME_GHOST_COMP,
};

typedef struct GAME_VEC
{
    GAME_FLT x;
    GAME_FLT y;
} GAME_VEC;


typedef struct GAME_ROTATE_STRUCT
{
	GAME_FLT torque;
	GAME_FLT maxAngularVelocity;
	GAME_INT radius;
	GAME_VEC center;
} GAME_ROTATE_STRUCT;

typedef struct GAME_PHYSICS
{
	GAME_FLT spinSpeed;
	GAME_BODY_TYPE bodyType;
	GAME_OBJECT_SHAPE objectShape;
	GAME_FLT density;
	GAME_FLT friction;
	GAME_FLT restitution;
	GAME_FLT angularDamping;
	GAME_FLT linearDamping;
	GAME_FLT force;
	bool physicsOn;
}GAME_PHYSICS;


typedef struct GAME_OBJECTFACTORY_PRESETS
{
	std::string objectType;
	GAME_VEC position;
	GAME_FLT angle;
	GAME_INT health;//move to new AssetLibraryGame
	GAME_INT destructTime;//move to new AssetLibraryGame
	std::shared_ptr<ResourceManager> devices;
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

typedef struct GAME_NOTICE
{
	GAME_INT x;
	GAME_INT y;
	GAME_DIRECTION direction;
	std::string text;
}GAME_NOTICE;

//Constants
const GAME_FLT PI = 3.14159f;
const float fPRV = 10.0f;



#endif