#include "BackpackComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "ComponentsList.h"

BackpackComponent::BackpackComponent(GameObject* owner, ResourceManager* devices)	:
	Component(owner, devices)
{}
BackpackComponent::~BackpackComponent(){}

//**************************************
//sets devices
bool BackpackComponent::initialize(ObjectFactoryPresets& presets)
//**************************************
{
	devices = presets.devices;
	open = false;
	
	//probably want to base the openSlots size on the number of slots we want
	//and not on the screen size. . .
	slotSize = 25; //pixels
	Vector2D screenDimensions{ devices->GetGraphicsDevice()->getScreenDimensions() };
	
	topLeft.x = (int)(screenDimensions.x *.1);
	topLeft.y = (int)(screenDimensions.y*.1);
	bottomRight.x = screenDimensions.x-topLeft.x;
	bottomRight.y = screenDimensions.y-topLeft.y;

	//***************************Set up openSlots and find dimensions **********************
	Vector2D backpackDimensions = {bottomRight.x - topLeft.x, bottomRight.y - topLeft.y};
	//maximum number of rows in openSlots
	maxRows = backpackDimensions.x/slotSize;
	maxColumns = backpackDimensions.y/slotSize;
			
	openSlots.resize(maxRows);
	for(auto& column : openSlots)
	{
		column.resize(maxColumns, true);
	}
	//*************************************************************************************
	return true;
}

//**************************************
//kills the item passed in, grabs it's rendere, and saves it to, what is essentially
//the Graphics device's openSlots vector.
bool BackpackComponent::addItem(std::unique_ptr<GameObject> item)
//**************************************
{
	//return whether it was added or not
	return (ToBackpack(std::move(item)));
	
}
	
	//Have this iterate through all items in inventory and draw them. .  . Must happen after Graphic's
	//Device does it's draw. . .
std::unique_ptr<GameObject> BackpackComponent::update()
	{
		if (open)
		{
			
			RGBA background = {255, 255, 255 , 220};
			RGBA border = {0,0,0,255};			
			std::map<Texture*, Vector2D> objects;
			for(const auto& item : inventory)
			{
				objects.emplace
				(
					item -> getComponent<RendererComponent>() -> GetTexture(), 
					item -> getComponent<InventoryComponent>() -> locationInPack()
				);
			}
			
			devices->GetGraphicsDevice()->addOverlay({ topLeft, bottomRight, background, border, objects });
				

			
		}
		return nullptr;
	}

	bool BackpackComponent::ToBackpack(std::unique_ptr<GameObject> item)
	{
		

		Vector2D screenDimensions{ devices->GetGraphicsDevice()->getScreenDimensions()};
			Vector2D topLeft { (int)(screenDimensions.x*.1), (int)(screenDimensions.y*.1)};
			Vector2D bottomRight {screenDimensions.x-topLeft.x, screenDimensions.y-topLeft.y};

			

			//**********************Find Item dimensions*************************
			auto rend{ item->getComponent<RendererComponent>() };
			//number of sequential rows we need (+.5 makes sure we round up)
			int numRows{ (int)(((float)rend->GetTexture()->getDimensions().x / slotSize) + .5) };
			//number of sequential columns we need
			int numColumns{ (int)(((float)rend->GetTexture()->getDimensions().y / slotSize) + .5) };
			//********************************************************************
				
			//number of sequential open rows found 
			int seqRows{ 0 };
			//current row & column we are checking
			Vector2D currPosition{ 0,0 };
				
			//Found a row that the item can fit in
			bool rowFound = false;

			//let's find a spot for it. . .
			//while we have not reached the last row, and we have not found a row to put it in.
			while(currPosition.x < maxRows && !rowFound)
			{
				//Not yet found a column that the item can fit in
				bool columnFound = false;
				//# of sequential open columns we have found
				int seqColumns = 0;
				//back to the first column
				currPosition.y = 0;
					
				//check for enough empty columns in the current row
				//While we haven't found a set of columns that works and we have not reached the last column
				while (!columnFound  && currPosition.y < maxColumns)
				{
					//If it is empty, add one to our sequential columns
					if(openSlots[currPosition.x][currPosition.y]) seqColumns++;
					//if it is not empty, start sequential column count over.
					else seqColumns = 0;
					//If we have found enough sequential columns, set the found to true
					if(seqColumns >= numColumns) columnFound = true;
					//otherwise, move on to the next column.
					else currPosition.y++;						
				}
					
					
				//if we found enough empty slots in the columns in this row
				if(columnFound)
				{
					//let's check the next row
					int r{ 0 };
					//back up to first in column that was empty in this sequence of empty columns
					//"-1" because currPosition.y count starts at 0, and numColumns starts at 1.
					currPosition.y -= (numColumns-1);

					//start by assuming all slots in the proper columns are empty
					bool rowGood{ true };
					//now let's check we have enough empty rows in these columns
					//We keep looping if 
						//we have not found enough empty rows
						//we have not reached the last row
						//The slots in the columns in the last row we checked are all empty
					while (!rowFound && r < maxRows && rowGood)
					{
						//iterate through the proper columns in this row
						for(int c = currPosition.y; c < currPosition.y + numColumns; c++)
						{
							//if even one is not empty, this row is no good.
							if(!openSlots[currPosition.x+r][currPosition.y+c]) rowGood = false;
						}
						//if we have found enough rows
						if(r >= numRows-1) rowFound = true;
						//otherwise move on to the next row
						else r++;

					}
				}
				//we have finished checking this row, so let's move to the next one.
				currPosition.x++;
			}

			//this can only be true if column is also true, which means we found the columns and rows we needed to place the item.
			if(rowFound)
			{
				//back up to the proper row
				currPosition.x--;
				//calculate position based on row & column.
				Vector2D position = {currPosition.y*slotSize + topLeft.x+5, currPosition.x*slotSize + topLeft.y+5};
				
				
				
				//Set the position of the item to be in the proper spot in the inventory
				item -> getComponent<InventoryComponent>() -> placeInPack(position);
				
				
				//mark used slots. . .
				for(int i = currPosition.x; i < currPosition.x + numRows; i ++)
				{
					for(int j = currPosition.y; j < currPosition.y + numColumns; j++)
					{
						openSlots[i][j] = false;
					}
				}
				//save it to the inventory vector
				inventory.push_back(std::move(item));
				//remove the item from the list of level objects
				inventory.back() -> getComponent<InventoryComponent>() -> pickUp();
				//we found a spot for it.
				return true;
			}
			//we didn't find a spot for it.
			return false;
	}