#include "BackpackComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "ComponentsList.h"

BackpackComponent::BackpackComponent(std::shared_ptr<GameObject> owner):Component(owner){}
BackpackComponent::~BackpackComponent(){}

//**************************************
//sets devices
bool BackpackComponent::Initialize(GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	devices = presets.devices;
	open = false;
	
	//probably want to base the backpack size on the number of slots we want
	//and not on the screen size. . .
	slotSize = 25; //pixels
	GraphicsDevice* gDevice = devices -> GetGraphicsDevice();
	GAME_INT SCREEN_WIDTH = gDevice -> GetScreenWidth();
	GAME_INT SCREEN_HEIGHT = gDevice -> GetScreenHeight();
	topLeft.x = SCREEN_WIDTH*.1;
	topLeft.y = SCREEN_HEIGHT*.1;
	bottomRight.x = SCREEN_WIDTH-topLeft.x;
	bottomRight.y = SCREEN_HEIGHT-topLeft.y;

	//***************************Set up backpack and find dimensions **********************
	GAME_VEC backpackDimensions = {bottomRight.x - topLeft.x, bottomRight.y - topLeft.y};
	//maximum number of rows in backpack
	maxRows = backpackDimensions.x/slotSize;
	maxColumns = backpackDimensions.y/slotSize;
			
	backpack = new bool*[maxRows];
	//initialize backpack with all slots empty.
	for(int i = 0; i < maxRows; i++)
	{
		backpack[i] = new bool[maxColumns];
		for (int j = 0; j < maxColumns; j++)
		{
			backpack[i][j] = true;
		}

	}
	//*************************************************************************************
	return true;
}

//**************************************
//kills the item passed in, grabs it's rendere, and saves it to, what is essentially
//the Graphics device's backpack vector.
bool BackpackComponent::AddItem(GameObject* item)
//**************************************
{
	//grab the shared pointer from the owner
	std::shared_ptr<GameObject> itemSP = item -> GetComponent<RendererComponent>() -> GetOwner();

	//return whether it was added or not
	return (ToBackpack(itemSP));
	
}

void BackpackComponent::Start(){}
	
	//Have this iterate through all items in inventory and draw them. .  . Must happen after Graphic's
	//Device does it's draw. . .
std::shared_ptr<GameObject> BackpackComponent::Update()
	{
		if (open)
		{
			
			GAME_RGBA background = {255, 255, 255 , 220};
			GAME_RGBA border = {0,0,0,255};

			std::map<Texture*, GAME_VEC> objects;
			//grab all textures and their positions;
			for(auto item : inventory)
			{
				objects[(item -> GetComponent<RendererComponent>() -> GetTexture())] = item -> GetComponent<InventoryComponent>() -> GetPackPosition();
			}
			
			devices -> GetGraphicsDevice() -> DrawOverlay(topLeft, bottomRight, background, border, objects);
				

			
		}
		return NULL;
	}
	void BackpackComponent::Finish(){}

	bool BackpackComponent::ToBackpack(std::shared_ptr<GameObject> item)
	{
		

			GraphicsDevice* gDevice = devices -> GetGraphicsDevice();
			GAME_INT SCREEN_WIDTH = gDevice -> GetScreenWidth();
			GAME_INT SCREEN_HEIGHT = gDevice -> GetScreenHeight();
			GAME_VEC topLeft = {SCREEN_WIDTH*.1, SCREEN_HEIGHT*.1};
			GAME_VEC bottomRight = {SCREEN_WIDTH-topLeft.x, SCREEN_HEIGHT-topLeft.y};

			

			//**********************Find Item dimensions*************************
			std::shared_ptr<RendererComponent> rend = item -> GetComponent<RendererComponent>();
			//number of sequential rows we need (+.5 makes sure we round up)
			int numRows = (rend -> GetTexture() -> getWidth()/slotSize) + .5;
			//number of sequential columns we need
			int numColumns = (rend -> GetTexture() -> getHeight()/slotSize) + .5;
			//********************************************************************
				
			//number of sequential open rows found 
			int seqRows = 0;
			//current row & column we are checking
			int currRow = 0;
			int currColumn = 0;
				
			//Found a row that the item can fit in
			bool rowFound = false;

			//let's find a spot for it. . .
			//while we have not reached the last row, and we have not found a row to put it in.
			while(currRow < maxRows && !rowFound)
			{
				//Not yet found a column that the item can fit in
				bool columnFound = false;
				//# of sequential open columns we have found
				int seqColumns = 0;
				//back to the first column
				currColumn = 0;
					
				//check for enough empty columns in the current row
				//While we haven't found a set of columns that works and we have not reached the last column
				while (!columnFound  && currColumn < maxColumns)
				{
					//If it is empty, add one to our sequential columns
					if(backpack[currRow][currColumn]) seqColumns++;
					//if it is not empty, start sequential column count over.
					else seqColumns = 0;
					//If we have found enough sequential columns, set the found to true
					if(seqColumns >= numColumns) columnFound = true;
					//otherwise, move on to the next column.
					else currColumn++;						
				}
					
					
				//if we found enough empty slots in the columns in this row
				if(columnFound)
				{
					//let's check the next row
					int r=0;
					//back up to first in column that was empty in this sequence of empty columns
					//"-1" because currColumn count starts at 0, and numColumns starts at 1.
					currColumn -= (numColumns-1);

					//start by assuming all slots in the proper columns are empty
					bool rowGood = true;
					//now let's check we have enough empty rows in these columns
					//We keep looping if 
						//we have not found enough empty rows
						//we have not reached the last row
						//The slots in the columns in the last row we checked are all empty
					while (!rowFound && r < maxRows && rowGood)
					{
						//iterate through the proper columns in this row
						for(int c = currColumn; c < currColumn + numColumns; c++)
						{
							//if even one is not empty, this row is no good.
							if(!backpack[currRow+r][currColumn+c]) rowGood = false;
						}
						//if we have found enough rows
						if(r >= numRows-1) rowFound = true;
						//otherwise move on to the next row
						else r++;

					}
				}
				//we have finished checking this row, so let's move to the next one.
				currRow++;
			}

			//this can only be true if column is also true, which means we found the columns and rows we needed to place the item.
			if(rowFound)
			{
				//back up to the proper row
				currRow--;
				//calculate position based on row & column.
				GAME_VEC position = {currColumn*slotSize + topLeft.x+5, currRow*slotSize + topLeft.y+5};
				
				
				
				//Set the position of the item to be in the proper spot in the inventory
				item -> GetComponent<InventoryComponent>() -> SetPackPosition(position);
				
				
				//mark used slots. . .
				for(int i = currRow; i < currRow + numRows; i ++)
				{
					for(int j = currColumn; j < currColumn + numColumns; j++)
					{
						backpack[i][j] = false;
					}
				}
				//save it to the inventory vector
				inventory.push_back(item);
				//remove the item from the list of level objects
				item -> GetComponent<InventoryComponent>() -> SetPickedUp(true);
				//we found a spot for it.
				return true;
			}
			//we didn't find a spot for it.
			return false;
	}