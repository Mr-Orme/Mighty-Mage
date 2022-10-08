
#pragma once

#include "box2d.h"

#include "PhysicsDevice.h"
#include "GraphicsDevice.h"
class Box2DDebugDraw : public b2Draw {

	public:	
		Box2DDebugDraw(PhysicsDevice* pDevice, GraphicsDevice* gDevice);
		~Box2DDebugDraw();
		

		void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
		void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
		void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
		void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override;
		void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override;
		void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;
	    void DrawTransform(const b2Transform &xf);

	private:

		PhysicsDevice* pDevice;
		GraphicsDevice* gDevice;
		int circleAccuracy;

};