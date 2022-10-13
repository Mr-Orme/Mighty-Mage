#ifndef COMPONENTSLIST_H
#define COMPONENTSLIST_H

/*If you add a component you need to make the following changes.
assets.xml 
if statements in ResourceManager.cpp loadComponent method.
Game->loadLevels may need a modification.
add the following to initializer:
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Backpack component";
		return false;
	}
create copyMe override!
*/
#include "Component.h"
#include "BodyComponent.h"

#include "HealthComponent.h"
#include "SpriteComponent.h"

#include "UserInputComponent.h"
#include "BackpackComponent.h"
#include "InventoryComponent.h"
#include "GhostComponent.h"
#include "TriggerComponent.h"
#include "QuestComponent.h"
#include "QuestCriteria.h"



#endif