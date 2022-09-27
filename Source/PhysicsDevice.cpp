#include "PhysicsDevice.h"
#include "RendererComponent.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "Texture.h"
#include "Box2DDebugDraw.h"
#include "ResourceManager.h"
#include <cmath>


PhysicsDevice::PhysicsDevice(Vector2D gravity, ResourceManager* devices):
	gravity(RW2PW(gravity.x), RW2PW(gravity.y)),
	debugDrawer(std::make_unique<Box2DDebugDraw>(devices)),
	listner(std::make_unique<ContactListener>())
{
	world =std::make_unique<b2World>(this->gravity);
	world->SetContactListener(listner.get());

	debugDrawer->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit);  //Turn on shape (red color) and aabb (green) 
	world->SetDebugDraw(debugDrawer.get());
	
}


//**************************************
//Steps the physics world
bool PhysicsDevice::Update(float dt)
//**************************************
{	
	world -> Step(dt, 8, 3);
	return true;
}

void PhysicsDevice::debugDraw()
{
	world->DebugDraw();
}

//**************************************
//Moves body to a set location & angle
bool PhysicsDevice::SetTransform(GameObject* object, Vector2D position, float angle)
//**************************************
{
	//finds which body this object is attached to.
	//based on value set in UserData field of body
	b2Body* body = FindBody(object);

	//Applies' Box2D transform.
	body -> SetTransform
		(
			GV2PV(position), 
			RW2PWAngle(angle)
		);
	return true;
	
}
bool PhysicsDevice::SetAngle(GameObject* object, float angle)
{
	b2Body* body = FindBody(object);
	body -> SetTransform(body -> GetPosition(), RW2PWAngle(angle));
	return true;
}


//**************************************
//Sets angular velocity
bool PhysicsDevice::SetAngularVelocity(GameObject* object, float angularVelocity)
//**************************************
{
	b2Body* body = FindBody(object);
	body -> SetAngularVelocity(RW2PW(angularVelocity));
	return true;
	
}

//**************************************
//Sets angular velocity
bool PhysicsDevice::SetLinearVelocity(GameObject* object, Vector2D linearVelociy)
//**************************************
{
	b2Body* body = FindBody(object);
	b2Vec2 b2LVelocity;
	b2LVelocity.x = RW2PW(linearVelociy.x);
	b2LVelocity.y = RW2PW(linearVelociy.y);
	body -> SetLinearVelocity(b2LVelocity);
	return true;
}

//**************************************
//Sets a linear impulse based on passed force vector and center vector
bool PhysicsDevice::SetLinearImpulse(GameObject* object, Vector2D forceVec, Vector2D forceCenter)
//**************************************
{
	b2Body* body = FindBody(object);
	body -> ApplyLinearImpulse
			(
			GV2PV(forceVec),
			body -> GetWorldPoint(GV2PV(forceCenter)),
			true
			);
	return true;
}

//**************************************
//Sets torque on body based on passed values
bool PhysicsDevice::SetTorque(GameObject* object, float torque)
//**************************************
{
	b2Body* body = FindBody(object);
	body -> ApplyTorque(RW2PW(torque), true);
	return true;
}

bool PhysicsDevice::SetStatic(GameObject* object)
{
	b2Body* body = FindBody(object);
	body -> SetType(b2_staticBody);
	return true;
}

bool PhysicsDevice::SetStopPhysics(GameObject* object)
{
	b2Body* body = FindBody(object);
	//body -> SetActive(false);
	body->SetEnabled(false);
	return true;	
}

//**************************************
//Gets Angular velocity of body
float PhysicsDevice::GetAngularVelocity(GameObject* object)
//**************************************
{
	b2Body* body = FindBody(object);
	return (PW2RWAngle(body -> GetAngularVelocity()));
}
Vector2D PhysicsDevice::GetLinearVelocity(GameObject* object)
{
	b2Body* body = FindBody(object);
	return(PV2GV(body -> GetLinearVelocity()));
}

//**************************************
//Gets bodies' position
Vector2D PhysicsDevice::GetPosition(GameObject* object)
//**************************************
{
	//b2Body* body = FindBody(object);
	////PV2GV = Physics vector to Game vector (b2vec2 to Vector2D)
	//return (PV2GV(body -> GetPosition()));
	return (AlignCenters(object));
}

//**************************************
//Gets bodies' angle
float PhysicsDevice::getAngle(GameObject* object)
//**************************************
{
	b2Body* body = FindBody(object);
	return (PW2RWAngle(body ->GetAngle()));
}

Vector2D PhysicsDevice::GetVelocity(GameObject* object)
{
	b2Body* body = FindBody(object);
	return (PV2GV(body -> GetLinearVelocity()));
}

//**************************************
//Creates a body and fixture for a passed in object
//based on passed in values
bool PhysicsDevice::createFixture(GameObject* object, BodyPresets presets)
//**************************************
{
	RendererComponent* compRenderer{ object->getComponent<RendererComponent>() };
	//new body definition
	b2BodyDef* bd = new b2BodyDef;
	//made need one or the other, depending on what was passed.
	b2PolygonShape pShape;
	b2CircleShape cShape;

	//Fixture definition
	b2FixtureDef shapefd;
	
	//Set userData field with passed in object pointer.
	bd -> userData.pointer = reinterpret_cast<uintptr_t>(object);
	

	//set body type
	switch (presets.physics.bodyType)
	{
	case BodyType::Static:
		bd -> type = b2_staticBody;
		break;
	case BodyType::Kinematic:
		bd -> type = b2_kinematicBody;
		break;
	case BodyType::Dynamic:
		bd -> type = b2_dynamicBody;
		break;
	}

	//********Adjust postion because SDL is top left, while box2d is center*************
	Texture* texture = object -> getComponent<RendererComponent>() -> getTexture();
	//subtract off half the width.
	presets.position.x += (texture -> getDimensions().x/2);
	//subtract off half the height
	presets.position.y += (texture -> getDimensions().y/2);
	//**********************************************************************************

	// set starting position & angle
	bd -> position.Set(RW2PW(presets.position.x), RW2PW(presets.position.y));
	bd -> angle = RW2PWAngle(presets.angle);

	//add the body to the world
	b2Body* body = world -> CreateBody(bd);

	//Set damping values on the body
	body -> SetAngularDamping(presets.physics.angularDamping);
	body -> SetLinearDamping(presets.physics.linearDamping);

	//Set fixture's shape
	switch (presets.physics.bodyShape)
	{
	case BodyShape::Rectangle:
		//rectangle's dimensions
		pShape.SetAsBox(RW2PW(compRenderer -> getTexture() -> getDimensions().x/2.0f), RW2PW(compRenderer -> getTexture() -> getDimensions().y/2.0f));
		shapefd.shape = &pShape;
		break;
	case BodyShape::Circle:
		//circle radius based on object's width.
		float width = compRenderer -> getTexture() -> getDimensions().x/2.0f;
		float height = compRenderer -> getTexture() -> getDimensions().y/2.0f;

		if (width > height)	cShape.m_radius = (RW2PW(width));
		else cShape.m_radius = (RW2PW(height));
		shapefd.shape = &cShape;
		break;
	}

	//set fixture values based on passed in values.
	shapefd.density = presets.physics.density;
	shapefd.friction = presets.physics.friction;
	shapefd.restitution = presets.physics.restitution;

	//create the fixture on the body.
	body -> CreateFixture(&shapefd);
	//body -> SetActive(physics.physicsOn);
	body->SetEnabled(presets.physics.physicsOn);
	return true;
	
}

//**************************************
//Removes an object from the World
bool PhysicsDevice::RemoveObject(GameObject* object)
//**************************************
{
	//Find the body based on the passed object
	b2Body* body = FindBody(object);
	DestroyJoint(body);
	
	//check to see if we have a body.
	if(body == nullptr)
	{return false;}
	else
	{
		//delete the body from the world
		world -> DestroyBody(body);
		return true;
	}

}

//**************************************
//Destroys a  physics joint from outside the physics device
bool PhysicsDevice::DestroyJoint(GameObject* object)
//**************************************
{
	b2Body* body = FindBody(object);
	
	//destroy joints associated with body.
	return DestroyJoint(body);
}

//**************************************
//Destroys the joint from within the device
bool PhysicsDevice::DestroyJoint(b2Body* body)
//**************************************
{
	bool jointFound = false;
	//destroy joints associated with body.
	for(b2JointEdge* j = body -> GetJointList(); j != nullptr; j = j -> next)
	{
		world -> DestroyJoint(j -> joint);
		jointFound = true;
	}
	return jointFound;
}

//**************************************
//Uses the user data field in the body and compares the memory location
//to the memory location of the passed object pointer.
//Returns the body that has the same memory location in the user data field
b2Body* PhysicsDevice::FindBody(GameObject* object)
//**************************************
{
	//loop through the bodies
	for (b2Body* body = world -> GetBodyList(); body; body = body -> GetNext())
	{
		//when we have a match, return it.
		if(object == reinterpret_cast<GameObject*>(body -> GetUserData().pointer))
		{
			return body;
		}
	}
	//if there was no match, return nullptr
	return nullptr;
}

//**************************************
//Converts passed Vector2D to b2Vec2
b2Vec2 PhysicsDevice::GV2PV(Vector2D gameVec)
//**************************************
{
	return b2Vec2(RW2PW(gameVec.x), RW2PW(gameVec.y));
}

//**************************************
//converts passed b2Vec2 to Vector2D
Vector2D PhysicsDevice::PV2GV(b2Vec2 physicsVec)
//**************************************
{
	Vector2D temp;
	temp.x = (int)PW2RW(physicsVec.x);
	temp.y = (int)PW2RW(physicsVec.y);
	return temp;
}

//**************************************
//Creates a revolute joint using the passed objects and anchor points
bool PhysicsDevice::CreatRevolvingJoint(GameObject* object1, GameObject* object2, Vector2D anchor1, Vector2D anchor2)
//**************************************
{
	//find corresponding bodies for objects
	b2Body* bodyA = FindBody(object1);
	b2Body* bodyB = FindBody(object2);
	if(bodyA == nullptr || bodyB == nullptr)
	{
		return false;
	}

	//joint definition
	b2RevoluteJointDef revoluteJointDef;

	//assign bodies to definition
	revoluteJointDef.bodyA = bodyA;
	revoluteJointDef.bodyB = bodyB;

	//They cannot collide
	revoluteJointDef.collideConnected = false;

	//set anchor points
	revoluteJointDef.localAnchorA.Set(RW2PW(anchor1.x), RW2PW(anchor1.y));
	revoluteJointDef.localAnchorB.Set(RW2PW(anchor2.x), RW2PW(anchor2.y));

	//Joint starts at angle zero
	revoluteJointDef.referenceAngle = 0;

	//Add the joint to the world. If successful will return True, othewise false.
	return (b2RevoluteJoint*)world -> CreateJoint(&revoluteJointDef);
	
}

//**************************************
//adjusts postion based on the fact that SDL is top left and Box2d uses the center of an object for position.
Vector2D PhysicsDevice::AlignCenters(GameObject* object)
//**************************************
{
	b2Body* body = FindBody(object);
	b2Vec2 physPosition = body -> GetPosition();
	Vector2D position;
	Texture* texture = object -> getComponent<RendererComponent>() -> getTexture();

		//subtract off half the width.
		position.x = (int)(PW2RW(physPosition.x) - (texture -> getDimensions().x/2.0f));
		//subtract off half the height
		position.y = (int)(PW2RW(physPosition.y) - (texture -> getDimensions().y/2.0f));
	

	return (position);
}