#ifndef NOTICESASSETLIBRARY_H
#define NOTICESASSETLIBRARY_H

#include <vector>
#include <string>
#include "Definitions.h"

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