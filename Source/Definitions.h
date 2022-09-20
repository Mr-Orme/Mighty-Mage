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


//Event Types
enum class Event {NA,up,down,left,right,space, shift, key_b, quit, numEvents};
enum class Direction {N =0, E = 90, S = 180, W = 270};
enum class Colors {red, yellow, blue, orange, green, purple, numColors};
enum class Levels {main, basement,};

enum class Components 
{
	body, 
	health, 
	renderer, 
	userInput, 
	backpack,
	inventory,
	ghost,
};

struct Vector2D
{
    int x;
    int y;
};


struct RotationStats
{
	float torque;
	float maxAngularVelocity;
	int radius;
	Vector2D center;
};

struct PhysicsStats
{
	enum class RotationDirection { left = -1, right = 1 };
	enum class BodyShape { rectangle, circle };
	enum class BodyType { staticBody, kinematic, dynamic };
	float spinSpeed;
	BodyType bodyType;
	BodyShape objectShape;
	float density;
	float friction;
	float restitution;
	float angularDamping;
	float linearDamping;
	float force;
	bool physicsOn;
};


struct ObjectFactoryPresets
{
	std::string objectType;
	Vector2D position;
	float angle;
	int health;//move to new AssetLibraryGame
	int destructTime;//move to new AssetLibraryGame
	ResourceManager* devices;
	std::map<Direction, bool> gDirection;
	//std::unique_ptr<GameObject> joinedObject; //only needed for joint
};

struct ObjectStats
{
	int health;
};

struct RGBA
{
	int R;
	int G;
	int B;
	int A;
};

struct Notice
{
	int x;
	int y;
	Direction direction;
	std::string text;
};

//Constants
const float PI = 3.14159f;
const float physicsModifier = 10.0f;



#endif