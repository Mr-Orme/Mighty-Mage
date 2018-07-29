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

struct GAME_VEC
{
	GAME_FLT x;
	GAME_FLT y;
	
	friend GAME_VEC operator+ (const GAME_VEC & src1, const GAME_VEC & src2)
	{
		return { src1.x + src2.x, src1.y + src2.y };
	}
	friend GAME_VEC operator- (const GAME_VEC & src1, const GAME_VEC & src2)
	{
		return { src1.x - src2.x, src1.y - src2.y };
	}
	friend bool operator> (const GAME_VEC & src1, int src2)
	{
		if (src1.x > src2 && src1.y > src2)
		{
			return true;
		}
		return false;
	}
	friend bool operator< (const GAME_VEC & src1, int src2)
	{
		if (src1.x < src2 && src1.y < src2)
		{
			return true;
		}
		return false;
	}
	friend bool operator== (const GAME_VEC & src1, const GAME_VEC & src2)
	{
		if (src1.x == src2.x && src1.y == src2.y)
		{
			return true;
		}
		return false;
	}
	friend GAME_VEC abs(GAME_VEC & src)
	{
		return { abs(src.x),abs(src.y) };
	}
};


enum GAME_DIRECTION {N =0, E = 90, S = 180, W = 270};

//TODO:make levels an array of resource managers! or something that has everything! 
enum GAME_LEVEL {GAME_LEVEL_MAIN, GAME_LEVEL_BASEMENT,};


typedef struct GAME_RGBA
{
	GAME_INT R;
	GAME_INT G;
	GAME_INT B;
	GAME_INT A;
}GAME_RGBA;

//Constants
const GAME_FLT PI = 3.14159f;
const int squareDimension = 110;



#endif