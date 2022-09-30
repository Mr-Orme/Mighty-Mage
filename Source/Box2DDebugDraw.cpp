#include "Box2DDebugDraw.h"
#include "ResourceManager.h"
#include "GraphicsDevice.h"
#include "PhysicsDevice.h"

Box2DDebugDraw::Box2DDebugDraw(ResourceManager* devices) :
	pDevice(devices->getPhysicsDevice()),
	gDevice(devices->getGraphicsDevice())
{
	circleAccuracy = 9;
	
}

Box2DDebugDraw::~Box2DDebugDraw() {
	
}


void Box2DDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	//std::cout <<"Draw Polygon";
	gDevice->setDrawColor({ 0,0,255,255 });
		

	for(int i=0; i<(vertexCount-1); i++) {
		gDevice->drawLine(
			{ (int)pDevice->PW2RW(vertices[i].x),(int)pDevice->PW2RW(vertices[i].y) },
			{ (int)pDevice->PW2RW(vertices[i + 1].x),(int)pDevice->PW2RW(vertices[i + 1].y) },
			{ 255,255,255,100 }
		);
	}	
	//draw a line from the last to the first
    gDevice->drawLine(
		{ (int)pDevice->PW2RW(vertices[vertexCount - 1].x),(int)pDevice->PW2RW(vertices[vertexCount - 1].y) },
		{ (int)pDevice->PW2RW(vertices[0].x), (int)pDevice->PW2RW(vertices[0].y) }, 
		{ 255, 255, 255, 100 }
	);

    //reset color (black)
	gDevice->setDrawColor({ 0,0,0,255 });
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	//std::cout << "Draw Solid Polygon";
   	// Draw Bound Box Vertices using SDL2 Primitives (Green)
	gDevice->setDrawColor({ 0,255,0,255 });

	// Draw Bound Box Vertices using SDL2 Primitives
	for (int i = 0; i < (vertexCount - 1); i++) {
		gDevice->drawLine(
			{ (int)pDevice->PW2RW(vertices[i].x),(int)pDevice->PW2RW(vertices[i].y) },
			{ (int)pDevice->PW2RW(vertices[i + 1].x),(int)pDevice->PW2RW(vertices[i + 1].y) },
			{ 255,255,255,100 }
		);
	}
	//draw a line from the last to the first
	gDevice->drawLine(
		{ (int)pDevice->PW2RW(vertices[vertexCount - 1].x),(int)pDevice->PW2RW(vertices[vertexCount - 1].y) },
		{ (int)pDevice->PW2RW(vertices[0].x), (int)pDevice->PW2RW(vertices[0].y) },
		{ 255, 255, 255, 100 }
	);

    //reset color (black)
	gDevice->setDrawColor({ 0,0,0,255 });
}

void Box2DDebugDraw::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) 
{
	//Unimplemented
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
{
    //Unimplemented
}

void Box2DDebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) 
{
    //Unimplemented
}

void Box2DDebugDraw::DrawTransform(const b2Transform &xf) 
{
    //Unimplemented
}