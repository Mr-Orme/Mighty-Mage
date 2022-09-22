#ifndef NOTICESASSETLIBRARY_H
#define NOTICESASSETLIBRARY_H

#include <vector>
#include <string>
#include "Definitions.h"

class NoticesAssetLibrary{
public:
	NoticesAssetLibrary();
	Notice Search(Notice square);
	bool AddAsset(Notice notice);
	bool RemoveAsset(Notice notice);
private:
	std::vector<Notice> library;
	

};


#endif