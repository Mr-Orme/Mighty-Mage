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
enum class Levels {sorpigal, sorpigal_basement,};

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

	const Vector2D& operator+=(const Vector2D& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}
	Vector2D& operator++()
	{
		x++;
		y++;

		return *this;
	}
	Vector2D& operator++(int)
	{
		Vector2D temp = *this;
		++* this;
		return temp;
	}
	const Vector2D& operator-=(const Vector2D& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2D& operator*=(const int& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const Vector2D& operator/=(const int& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==(const Vector2D& rhs)const
	{
		return (x == rhs.x && y == rhs.y);
	}

	bool operator!=(const Vector2D& rhs)const
	{
		return !(*this == rhs);
	}
	Vector2D operator-(const Vector2D& rhs)
	{
		return{ x - rhs.x, y - rhs.y };
	}

};

//-----------------------------------------------------------------------some more operator overloads
//inline Vector2D operator*(const Vector2D& lhs, int rhs);
//inline Vector2D operator*(int lhs, const Vector2D& rhs);
//inline Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs);
//inline Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs);
//inline Vector2D operator/(const Vector2D& lhs, int val);
//std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
//std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);



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
	Vector2D square;
	Direction direction;
	std::string text;
};

//Constants
const float PI = 3.14159f;
const float physicsModifier = 10.0f;



#endif