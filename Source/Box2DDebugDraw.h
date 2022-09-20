//only include this header 1 time per compile
#pragma once
//so no include guards are needed

#include "box2d.h"
//#include "GameFunctions.h"
#include "ResourceManager.h"

class Box2DDebugDraw : public b2Draw {

	public:	
		Box2DDebugDraw();
		~Box2DDebugDraw();
		bool initialize(ResourceManager* devices);

		void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
		void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
		void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
		void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override;
		void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override;
		void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;
	    void DrawTransform(const b2Transform &xf);

	private:

		ResourceManager* devices;
		int circleAccuracy;

};