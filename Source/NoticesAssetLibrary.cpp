#include "NoticesAssetLibrary.h"
#include "GraphicsDevice.h"
NoticesAssetLibrary::NoticesAssetLibrary()
{
	
}



//**************************************
//Based on your position in the game space and direction you are facing
//this method searches the library for the proper notice to display
//and returns that notice.
//if not found, it sets the text to nothing of the notice passed in and
//returns that notice.
Notice NoticesAssetLibrary::search(Notice square)
//**************************************
{
	for (auto notices : library)
	{//TODO::this naming is weird.
		if(notices.square.x == square.square.x && notices.square.y == square.square.y && notices.direction == square.direction)
		{
			return notices;
		}
	}
	square.text = "";
	return square;
}
//**************************************
//adds a notice to the library vector
bool NoticesAssetLibrary::addAsset(Notice notice)
//**************************************
{
	library.push_back(notice);
	return true;
}

//**************************************
//Based on your position in the game space and direction of the passed in notice
//a notice is found and removed from the library.
bool NoticesAssetLibrary::removeAsset(Notice notice)
//**************************************
{
	bool foundNotice = false;
	std::vector<Notice>::iterator notices;
	//iterate through all notices
	for (notices = library.begin(); notices != library.end(); notices++)
	{
		//if position and direction match, delete the notice from the library.
		if(notices ->square.x == notice.square.x && notices ->square.y == notice.square.y && notices -> direction == notice.direction)
		{
			library.erase(notices);
			notices--;
			foundNotice = true;
		}
	}
	//if we did not find a notice, we return false.
	return foundNotice;
}