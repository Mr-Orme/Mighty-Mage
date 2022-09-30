#ifndef NOTICESASSETLIBRARY_H
#define NOTICESASSETLIBRARY_H

#include <vector>
#include <string>
#include "Vector2D.h"
#include "Definitions.h"


struct Notice
{
	Vector2D square;
	Direction direction;
	std::string text;
};
class NoticesAssetLibrary{
public:
	NoticesAssetLibrary();
	//If notice is found, it updates square and returns true, else false.
	bool search(Notice& square);

	bool addAsset(Notice notice);
	bool removeAsset(Notice notice);
private:
	std::vector<Notice> library;
	

};
bool operator==(const Notice& srcL, const Notice& srcR);

#endif