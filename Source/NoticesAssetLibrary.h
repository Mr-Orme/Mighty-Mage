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
	Notice search(Notice square);
	bool addAsset(Notice notice);
	bool removeAsset(Notice notice);
private:
	std::vector<Notice> library;
	

};


#endif