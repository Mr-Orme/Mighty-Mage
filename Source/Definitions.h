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

//Basic Types
typedef unsigned int	Uint32;
typedef float			GAME_FLT;
typedef Uint32			GAME_INT;

struct GAME_VEC
{
	GAME_FLT x;
	GAME_FLT y;
	GAME_VEC friend operator+ (const GAME_VEC & src1, const GAME_VEC& src2)
	{
		return { src1.x + src2.x, src1.y + src2.y };
	}
	GAME_VEC friend operator- (const GAME_VEC & src1, const GAME_VEC& src2)
	{
		return { src1.x - src2.x, src1.y - src2.y };
	}
};


enum GAME_DIRECTION {N =0, E = 90, S = 180, W = 270};

//make levels an array of resource managers! or something that has everything! FIXME
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




#endif