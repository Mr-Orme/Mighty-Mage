#ifndef NOTICESASSETLIBRARY_H
#define NOTICESASSETLIBRARY_H

#include <vector>
#include <string>
#include "Definitions.h"

class NoticesAssetLibrary{
public:
	NoticesAssetLibrary();
	bool Initialize();
	GAME_NOTICE Search(GAME_NOTICE square);
	bool AddAsset(GAME_NOTICE notice);
	bool RemoveAsset(GAME_NOTICE notice);
private:
	std::vector<GAME_NOTICE> library;
	

};


#endif