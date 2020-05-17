#include "NoticesAssetLibrary.h"
#include "GraphicsDevice.h"
NoticesAssetLibrary::NoticesAssetLibrary()
{
	
}

//**************************************
//This function sets the graphic's device for the library.
bool NoticesAssetLibrary::Initialize()

//**************************************
{
	return true;
}

//**************************************
//Based on your position in the game space and direction you are facing
//this method searches the library for the proper notice to display
//and returns that notice.
//if not found, it sets the text to nothing of the notice passed in and
//returns that notice.
GAME_NOTICE NoticesAssetLibrary::Search(GAME_NOTICE square)
//**************************************
{
	for (auto notices : library)
	{
		if(notices.x == square.x && notices.y == square.y && notices.direction == square.direction)
		{
			return notices;
		}
	}
	square.text = "";
	return square;
}
//**************************************
//adds a notice to the library vector
bool NoticesAssetLibrary::AddAsset(GAME_NOTICE notice)
//**************************************
{
	library.push_back(notice);
	return true;
}

//**************************************
//Based on your position in the game space and direction of the passed in notice
//a notice is found and removed from the library.
bool NoticesAssetLibrary::RemoveAsset(GAME_NOTICE notice)
//**************************************
{
	bool foundNotice = false;
	std::vector<GAME_NOTICE>::iterator notices;
	//iterate through all notices
	for (notices = library.begin(); notices != library.end(); notices++)
	{
		//if position and direction match, delete the notice from the library.
		if(notices -> x == notice.x && notices -> y == notice.y && notices -> direction == notice.direction)
		{
			library.erase(notices);
			notices--;
			foundNotice = true;
		}
	}
	//if we did not find a notice, we return false.
	return foundNotice;
}