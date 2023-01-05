#pragma once
enum class Direction { N = 0, E = 90, S = 180, W = 270 };
struct RGBA
{
	int R;
	int G;
	int B;
	int A;
};
//IDEA::breath first search to make rooms open when entering.
//Some container that keeps track of which squares have walls
//to do the search