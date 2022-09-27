#include <algorithm>
#include "BackpackComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "ComponentsList.h"
#include "Texture.h"
#include "Vector2D.h"
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

	Vector2D screenDimensions{ devices->GetGraphicsDevice()->getScreenDimensions() };
	
	topLeft.x = (int)(screenDimensions.x *.1);
	topLeft.y = (int)(screenDimensions.y*.1);
	bottomRight.x = screenDimensions.x-topLeft.x;
	bottomRight.y = screenDimensions.y-topLeft.y;

	reset();
	return true;
}

//**************************************
//kills the item passed in, grabs it's rendere, and saves it to, what is essentially
//the Graphics device's openSlots vector.
bool BackpackComponent::pickUpItem(GameObject* item)
//**************************************
{
	pickedUpItem = item;
	return true;
}
	
	//Have this iterate through all items in inventory and draw them. .  . Must happen after Graphic's
	//Device does it's draw. . .
std::unique_ptr<GameObject> BackpackComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
	{
	if (pickedUpItem != nullptr)
	{
		if (auto toPickUp{ std::find_if(objects.begin(), objects.end(),
			[&](std::unique_ptr<GameObject>& object) {
				return pickedUpItem == object.get();
			}) }; toPickUp != objects.end())
		{
			ToBackpack(std::move(*toPickUp));
			pickedUpItem = nullptr;
		}
	}
		if (open)
		{			
			std::map<Texture*, Vector2D> objects;
			for(const auto& item : inventory)
			{
				objects.emplace
				(
					item -> getComponent<RendererComponent>() -> getTexture(), 
					item -> getComponent<InventoryComponent>() -> locationInPack()
				);
			}
			
			devices->GetGraphicsDevice()->
				addOverlay({ topLeft, bottomRight, background, border, objects });
		}
		return nullptr;
	}

	bool BackpackComponent::ToBackpack(std::unique_ptr<GameObject> item)
	{
		item->getComponent<BodyComponent>()->turnOffPhysics();

		Vector2D screenDimensions{ devices->GetGraphicsDevice()->getScreenDimensions()};
			Vector2D topLeft { (int)(screenDimensions.x*.1), (int)(screenDimensions.y*.1)};
			Vector2D bottomRight {screenDimensions.x-topLeft.x, screenDimensions.y-topLeft.y};

			

			//**********************Find Item dimensions*************************
			auto rend{ item->getComponent<RendererComponent>() };
			//number of sequential rows we need (+.5 makes sure we round up)
			int numRows{ (int)(((float)rend->getTexture()->getDimensions().x / slotSize) + .5) };
			//number of sequential columns we need
			int numColumns{ (int)(((float)rend->getTexture()->getDimensions().y / slotSize) + .5) };
			//********************************************************************
				
			//number of sequential open rows found 
			int seqRows{ 0 };
			//current row & column we are checking
			Vector2D currPosition{ 0,0 };
				
			//Found a row that the item can fit in
			bool rowFound = false;

			//let's find a spot for it. . .
			//while we have not reached the last row, and we have not found a row to put it in.
			while(currPosition.x < max.x && !rowFound)
			{
				//Not yet found a column that the item can fit in
				bool columnFound = false;
				//# of sequential open columns we have found
				int seqColumns = 0;
				//back to the first column
				currPosition.y = 0;
					
				//check for enough empty columns in the current row
				//While we haven't found a set of columns that works and we have not reached the last column
				while (!columnFound  && currPosition.y < max.y)
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
					while (!rowFound && r < max.x && rowGood)
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
				
				inventory.push_back(std::move(item));
				
				
				
				return true;
			}
			//we didn't find a spot for it.
			return false;
	}

	void BackpackComponent::reset()
	{
		Vector2D backpackDimensions { bottomRight.x - topLeft.x, bottomRight.y - topLeft.y };
		
		max.x = backpackDimensions.x / slotSize;
		max.y = backpackDimensions.y / slotSize;
		openSlots.clear();
		openSlots.resize(max.x);
		for (auto& column : openSlots)
		{
			column.resize(max.y, true);
		}
	}
