#ifndef COMPONENTSLIST_H
#define COMPONENTSLIST_H

/*If you add a component you need to make the following changes.
assets.xml 
enum in definitions 
switch in ComponentAssetLibrary.cpp 
if statements in ResourceManager.cpp
*/
#include "Component.h"
#include "BodyComponent.h"

#include "HealthComponent.h"
#include "RendererComponent.h"

#include "UserInputComponent.h"
#include "BackpackComponent.h"
#include "InventoryComponent.h"
#include "GhostComponent.h"



#endif