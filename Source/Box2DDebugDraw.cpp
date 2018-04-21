#include "Box2DDebugDraw.h"
#include "SDL2_gfxPrimitives.h"

Box2DDebugDraw::Box2DDebugDraw() {
	circleAccuracy = 9;
}

Box2DDebugDraw::~Box2DDebugDraw() {
	devices = NULL;
}

bool Box2DDebugDraw::Initialize(ResourceManager* devices) {
	if(devices == NULL)
	{
		return false;
	}
	this->devices = devices;
	return true;
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) 
{
	//std::cout <<"Draw Polygon";
   	// Draw Shape Vertices using SDL2_gfx (white)
	SDL_SetRenderDrawColor(devices -> GetGraphicsDevice() -> GetRenderer(),0,0,255,255);

	// Draw Bound Box Vertices using SDL2 Primitives
	for(int i=0; i<(vertexCount-1); i++) {
		lineRGBA(devices -> GetGraphicsDevice() ->GetRenderer(), devices -> GetPhysicsDevice() -> PW2RW(vertices[i].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[i].y),devices -> GetPhysicsDevice() -> PW2RW(vertices[i+1].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[i+1].y),255,255,255,100);
		//SDL_RenderDrawLine(devices -> GetGraphicsDevice() ->GetRenderer(),devices -> GetPhysicsDevice() -> PW2RW(vertices[i].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[i].y),devices -> GetPhysicsDevice() -> PW2RW(vertices[i+1].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[i+1].y));
	}	
	//draw a line from the last to the first
    lineRGBA(devices -> GetGraphicsDevice() ->GetRenderer(),devices -> GetPhysicsDevice() -> PW2RW(vertices[vertexCount-1].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[vertexCount-1].y),devices -> GetPhysicsDevice() -> PW2RW(vertices[0].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[0].y),255,255,255,100);

    //Reset color (black)
    SDL_SetRenderDrawColor(devices -> GetGraphicsDevice() ->GetRenderer(),0,0,0,255);
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	//std::cout << "Draw Solid Polygon";
   	// Draw Bound Box Vertices using SDL2 Primitives (Green)
    SDL_SetRenderDrawColor(devices -> GetGraphicsDevice() ->GetRenderer(),0,255,0,255);

	// Draw Bound Box Vertices using SDL2 Primitives
	for(int i=0; i<(vertexCount-1); i++) {
        lineRGBA(devices -> GetGraphicsDevice() ->GetRenderer(),devices -> GetPhysicsDevice() -> PW2RW(vertices[i].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[i].y),devices -> GetPhysicsDevice() -> PW2RW(vertices[i+1].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[i+1].y),255,0,0,100);
		//SDL_RenderDrawLine(devices -> GetGraphicsDevice() ->GetRenderer(),devices -> GetPhysicsDevice() -> PW2RW(vertices[i].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[i].y),devices -> GetPhysicsDevice() -> PW2RW(vertices[i+1].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[i+1].y));
	}	
	//draw a line from the last to the first
    lineRGBA(devices -> GetGraphicsDevice() ->GetRenderer(),devices -> GetPhysicsDevice() -> PW2RW(vertices[vertexCount-1].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[vertexCount-1].y),devices -> GetPhysicsDevice() -> PW2RW(vertices[0].x),devices -> GetPhysicsDevice() -> PW2RW(vertices[0].y),255,0,0,100);

    //Reset color (black)
    SDL_SetRenderDrawColor(devices -> GetGraphicsDevice() ->GetRenderer(),0,0,0,255);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) 
{
	//Unimplemented
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
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