#include "PhysicsDevice.h"
#include "BodyComponent.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "Texture.h"
//#include "Box2DDebugDraw.h"
#include "ResourceManager.h"
//#include <cmath>


PhysicsDevice::PhysicsDevice(Vector2D gravity, ResourceManager* devices):
	gravity(RW2PW(gravity.x), RW2PW(gravity.y)),
	world(std::make_unique<b2World>(this->gravity)),
	listner(std::make_unique<ContactListener>(this))
{
	world->SetContactListener(listner.get());
	
}

PhysicsDevice::~PhysicsDevice()
{

}


//**************************************
//Steps the physics world
bool PhysicsDevice::Update(float dt)
//**************************************
{	
	world -> Step(dt, 8, 3);
	return true;
}



//**************************************
//Moves body to a set location & angle
bool PhysicsDevice::setTransform(GameObject* object, Vector2D position, float angle)
//**************************************
{
	
	FindBody(object)-> SetTransform
		(
			GV2PV(position), 
			RW2PWAngle(angle)
		);
	return true;
	
}
bool PhysicsDevice::SetAngle(GameObject* object, float angle)
{
	auto body{ FindBody(object) };
	body -> SetTransform(body -> GetPosition(), RW2PWAngle(angle));
	return true;
}


//**************************************
//Sets angular velocity
bool PhysicsDevice::setAngularVelocity(GameObject* object, float angularVelocity)
//**************************************
{
	FindBody(object) -> SetAngularVelocity(RW2PW(angularVelocity));
	return true;
	
}

//**************************************
//Sets angular velocity
bool PhysicsDevice::setLinearVelocity(GameObject* object, Vector2D linearVelociy)
//**************************************
{
	FindBody(object) -> SetLinearVelocity({ RW2PW(linearVelociy.x),RW2PW(linearVelociy.y) });
	return true;
}

//**************************************
//Sets a linear impulse based on passed force vector and center vector
bool PhysicsDevice::SetLinearImpulse(GameObject* object, Vector2D forceVec, Vector2D forceCenter)
//**************************************
{
	auto body{ FindBody(object) };
	body->ApplyLinearImpulse
	(
		GV2PV(forceVec),
		body->GetWorldPoint(GV2PV(forceCenter)),
		true
	);
	return true;
}

//**************************************
//Sets torque on body based on passed values
bool PhysicsDevice::SetTorque(GameObject* object, float torque)
//**************************************
{
	FindBody(object)->ApplyTorque(RW2PW(torque), true);
	return true;
}

bool PhysicsDevice::SetStatic(GameObject* object)
{
	FindBody(object)->SetType(b2_staticBody);
	return true;
}

bool PhysicsDevice::SetStopPhysics(GameObject* object)
{
	FindBody(object)->SetEnabled(false);
	return true;	
}

//**************************************
//Gets Angular velocity of body
float PhysicsDevice::GetAngularVelocity(GameObject* object)
//**************************************
{
	
	return (PW2RWAngle(FindBody(object)->GetAngularVelocity()));
}
Vector2D PhysicsDevice::GetLinearVelocity(GameObject* object)
{
	return(PV2GV(FindBody(object)->GetLinearVelocity()));
}

//**************************************
//Gets bodies' position
Vector2D PhysicsDevice::GetPosition(GameObject* object)
//**************************************
{
	return (AlignCenters(object));
}

//**************************************
//Gets bodies' angle
float PhysicsDevice::getAngle(GameObject* object)
//**************************************
{
	return (PW2RWAngle(FindBody(object)->GetAngle()));
}

Vector2D PhysicsDevice::GetVelocity(GameObject* object)
{
	return (PV2GV(FindBody(object)-> GetLinearVelocity()));
}

//**************************************
//Creates a body and fixture for a passed in object
//based on passed in values
bool PhysicsDevice::createFixture(GameObject* object, BodyPresets presets)
//**************************************
{
	
	//new body definition
	auto bd{ new b2BodyDef };
		
	//Set userData field with passed in object pointer.
	//bd -> userData.pointer = reinterpret_cast<uintptr_t>(object);
	//bd->SetUserData(object);

	//set body type
	bd->type = (b2BodyType)presets.physics.bodyType;


	//********Adjust postion because SDL is top left, while box2d is center*************
	presets.position += presets.dimensions/2;
	//**********************************************************************************

	// set starting position & angle
	bd -> position.Set(RW2PW(presets.position.x), RW2PW(presets.position.y));
	bd -> angle = RW2PWAngle((float)presets.angle);

	//add the body to the world
	auto body{ world->CreateBody(bd) };

	//Set damping values on the body
	body -> SetAngularDamping(presets.physics.angularDamping);
	body -> SetLinearDamping(presets.physics.linearDamping);
	bodyMap.insert({ object, body });

	//made need one or the other, depending on what was passed.
	
	

	//Fixture definition
	b2FixtureDef shapefd{};

	//Set fixture's shape
	b2PolygonShape pShape{};
	b2CircleShape cShape{};
	switch (presets.physics.bodyShape)
	{
	case BodyShape::Rectangle:
	{
		//rectangle's dimensions
		
		pShape.SetAsBox(RW2PW(presets.dimensions.x / 2.0f), RW2PW(presets.dimensions.y / 2.0f));
		shapefd.shape = &pShape;
		break;
	}
	case BodyShape::Circle:
	{
		//circle radius based on object's width.
		float width{ presets.dimensions.x / 2.0f };
		float height{ presets.dimensions.y / 2.0f };

		
		if (width > height)	cShape.m_radius = (RW2PW(width));
		else cShape.m_radius = (RW2PW(height));
		shapefd.shape = &cShape;

		break;
	}
	}
	
	//set fixture values based on passed in values.
	shapefd.density = presets.physics.density;
	shapefd.friction = presets.physics.friction;
	shapefd.restitution = presets.physics.restitution;

	//create the fixture on the body.
	body -> CreateFixture(&shapefd);
	body->SetEnabled(presets.physics.physicsOn);
	
	return true;
	
}

//**************************************
//Removes an object from the World
bool PhysicsDevice::RemoveObject(GameObject* object)
//**************************************
{
	if(auto body{ FindBody(object) }; body)
	{
		DestroyJoint(body);
		//delete the body from the world
		world -> DestroyBody(body);
		return true;
	}
	return false;
}

//**************************************
//Destroys a  physics joint from outside the physics device
bool PhysicsDevice::DestroyJoint(GameObject* object)
//**************************************
{
	return DestroyJoint(FindBody(object));
}

//**************************************
//Destroys the joint from within the device
bool PhysicsDevice::DestroyJoint(b2Body* body)
//**************************************
{
	bool jointFound{ false };
	//destroy joints associated with body.
	for(auto j = body -> GetJointList(); j != nullptr; j = j -> next)
	{
		world -> DestroyJoint(j -> joint);
		jointFound = true;
	}
	return jointFound;
}
GameObject* PhysicsDevice::findObject(b2Body* body) const
{
	auto foundBody{ std::find_if(bodyMap.begin(), bodyMap.end(), [&](std::pair<GameObject*, b2Body*> bodyPair)
		{
			return body == bodyPair.second;
		}) };
	if (foundBody != bodyMap.end())
	{
		return foundBody->first;
	}
	return nullptr;
}
//**************************************
//Uses the user data field in the body and compares the memory location
//to the memory location of the passed object pointer.
//Returns the body that has the same memory location in the user data field
b2Body* PhysicsDevice::FindBody(GameObject* object)
//**************************************
{

	if (auto body{ bodyMap.find(object) }; body != bodyMap.end())
	{
		return body->second;
	}

	return nullptr;
}

//**************************************
//Converts passed Vector2D to b2Vec2
b2Vec2 PhysicsDevice::GV2PV(Vector2D gameVec)
//**************************************
{
	return { RW2PW(gameVec.x), RW2PW(gameVec.y) };
}

//**************************************
//converts passed b2Vec2 to Vector2D
Vector2D PhysicsDevice::PV2GV(b2Vec2 physicsVec)
//**************************************
{
	return { (int)PW2RW(physicsVec.x), (int)PW2RW(physicsVec.y) };
}

//**************************************
//Creates a revolute joint using the passed objects and anchor points
bool PhysicsDevice::CreatRevolvingJoint(GameObject* object1, GameObject* object2, Vector2D anchor1, Vector2D anchor2)
//**************************************
{
	//find corresponding bodies for objects
	auto bodyA{ FindBody(object1)};
	auto bodyB{ FindBody(object2) };
	if(bodyA == nullptr || bodyB == nullptr)
	{
		return false;
	}

	//joint definition
	b2RevoluteJointDef revoluteJointDef{};
	revoluteJointDef.bodyA = bodyA;
	revoluteJointDef.bodyB = bodyB;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(RW2PW(anchor1.x), RW2PW(anchor1.y));
	revoluteJointDef.localAnchorB.Set(RW2PW(anchor2.x), RW2PW(anchor2.y));
	revoluteJointDef.referenceAngle = 0;

	//Add the joint to the world. If successful will return True, othewise false.
	return (b2RevoluteJoint*)world -> CreateJoint(&revoluteJointDef);
	
}

//**************************************
//adjusts postion based on the fact that SDL is top left and Box2d uses the center of an object for position.
Vector2D PhysicsDevice::AlignCenters(GameObject* object)
//**************************************
{
	
	auto physPosition{ FindBody(object)->GetPosition() };
	auto body{ object->getComponent<BodyComponent>() };
		return 
		{
			(int)(PW2RW(physPosition.x) - (body->getDimensions().x / 2.0f)),
			(int)(PW2RW(physPosition.y) - (body->getDimensions().y / 2.0f))
		};
}