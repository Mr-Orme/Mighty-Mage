#include "BodyComponent.h"
#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h"

#include "Vector2D.h"
#include "Definitions.h"

BodyComponent::BodyComponent(GameObject* owner, ResourceManager* devices):Component(owner, devices){}
BodyComponent::BodyComponent():Component()
{
}
BodyComponent::~BodyComponent()
{
	//remove the physics body
	if (!devices->GetPhysicsDevice()->RemoveObject(_owner))
	{
		printf("Object could not be removed from Physics World");
		//exit(1);
	}
}

//**************************************
//Based on the presets struct passed in, a fixture is created
bool BodyComponent::initialize(ObjectFactoryPresets& presets)
//**************************************
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Bodycomponent";
		return false;
	}
	PhysicsStats physics;
	if(_owner->getComponent<SpriteComponent>() != nullptr)
	{
		//TODO::do we need to set devices here?
		devices = presets.devices;
		devices -> GetPhysicsDevice() -> createFixture(	_owner,	presets.bodyInitializers);
	}
	return true;
}

//**************************************
//finds the current position, subtract's off the last frame's position
//to get the change in position. This is for the auto scrolling feature.
//the newpostion then becomes the old position.
std::unique_ptr<GameObject> BodyComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
//**************************************
{
	//*************************************** True to 90**********************
	//sometimes the angle get's off perpendicular.
	int angle = (int(devices->GetPhysicsDevice()->getAngle(_owner)));

	if (angle > 360 && angle < 460) angle = 90;
	if (angle < 0) angle = 270;
	if (angle < 45 && angle >= 315) devices->GetPhysicsDevice()->SetAngle(_owner, 0);
	else if (angle < 135 && angle >= 45) devices->GetPhysicsDevice()->SetAngle(_owner, 90);
	else if (angle < 225 && angle >= 135) devices->GetPhysicsDevice()->SetAngle(_owner, 180);
	else if (angle < 315 && angle >= 225) devices->GetPhysicsDevice()->SetAngle(_owner, 270);

	return nullptr;
}

std::unique_ptr<Component> BodyComponent::copyMe() const
{
	return std::make_unique<BodyComponent>(*this);
}

void BodyComponent::turnLeft()
{
	devices->GetPhysicsDevice()->SetAngle(_owner, devices->GetPhysicsDevice()->getAngle(_owner) - angleAdjust);
	devices->GetPhysicsDevice()->SetLinearVelocity(_owner, { 0,0 });
}

void BodyComponent::turnRight()
{
	//set angle by angle adjust
	devices->GetPhysicsDevice()->SetAngle(
		_owner,
		devices->GetPhysicsDevice()->getAngle(_owner) + angleAdjust);
	//stop forward motion
	devices->GetPhysicsDevice()->SetLinearVelocity(_owner, { 0,0 });
}

void BodyComponent::moveForward()
{
	
	linearMovement(Way::forward);
}

void BodyComponent::moveBackward()
{
	linearMovement(Way::backward);

}

void BodyComponent::stop()
{
	devices->GetPhysicsDevice()->SetLinearVelocity(_owner, { 0,0 });
}

float BodyComponent::getAngle() const
{
	return devices->GetPhysicsDevice()->getAngle(_owner);
}

Direction BodyComponent::getDirection() const
{
	return static_cast<Direction>(abs((int(getAngle()) % 360)));
}

Vector2D BodyComponent::getPosition() const
{
	return devices->GetPhysicsDevice()->GetPosition(_owner);
}

void BodyComponent::turnOffPhysics() const
{
	devices->GetPhysicsDevice()->SetStopPhysics(_owner);
}
//**************************************
//find's the 15x15 game square based on current position
Vector2D BodyComponent::currentSquare() const
//**************************************
{
	Vector2D position
	{ devices->GetGraphicsDevice()->getView()->relativePosition(getPosition()) };
	Vector2D dimensions{ getDimenions() };
	Vector2D center{ (int)(position.x + .5 * dimensions.x), (int)(position.y + .5 * dimensions.y) };

	Vector2D viewPosition
	{ devices->GetGraphicsDevice()->getView()->getViewingWindowPosition() };
	Vector2D cityCorner{ devices->GetCityCorner() };

	//the city corner plus the view get's us the top left corner of the view.
	//subtract off the player's position on the screen to get the actual spot of the player.
	//divide the number of pixels in each square.
	//Adjust the y, because the 15x15 square starts in the bottom left corner, while SDL starts in the top left.
	Vector2D square =
	{
		int((cityCorner.x + viewPosition.x - position.x) * -1 / devices->pixelsPerSquare),
		devices->blocksPerMap + int((cityCorner.y + viewPosition.y - position.y) / devices->pixelsPerSquare)
	};
	return square;
	//this code is here for use for debugging.
	//run player position on himself
	/*std::string playerPositionText = "(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")";

	Vector2D position = _owner -> getComponent<SpriteComponent>() -> relativePosition(devices -> GetPhysicsDevice() -> GetPosition(_owner));
	devices -> GetGraphicsDevice() ->text2Screen(playerPositionText, position);*/
}

Vector2D BodyComponent::getDimenions() const
{
	return _owner->getComponent<SpriteComponent>()->getTexture()->getDimensions();
}

void BodyComponent::linearMovement(Way direction)
{
	Vector2D applyForce{};
	std::string walkSound{ "walking" };
	if (devices->GetInputDevice()->isPressed(InputDevice::Inputs::shift))
	{
		forceMultiplier = baseForceMultiplier * runMultiplier;
		walkSound = "run";
	}
	else
	{
		//back to walk speeds
		forceMultiplier = baseForceMultiplier;

	}
	//grab current velocity
	Vector2D velocity = devices->GetPhysicsDevice()->GetVelocity(_owner);
	//Calculate force vector for a forward push
	
	applyForce.x = (int)cosf((devices->GetPhysicsDevice()->getAngle(_owner) * Pi / 180) + direction*(Pi / 2)) * forceMultiplier;
	applyForce.y = (int)sinf((devices->GetPhysicsDevice()->getAngle(_owner) * Pi / 180) + direction*(Pi / 2)) * forceMultiplier;
	
	//PUSH BABY PUSH!!!
	devices->GetPhysicsDevice()->SetLinearVelocity(_owner, applyForce);
	//if last play is done and the player is still moving
	if (abs(velocity.x) > 1 || abs(velocity.y) > 1)
	{
		//play walking sound
		devices->GetSoundDevice()->PlaySound(SoundEffect::Event::walk, 0, 1);
	}
}
