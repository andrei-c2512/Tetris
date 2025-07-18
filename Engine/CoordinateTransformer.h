#pragma once
#include "Graphics.h"
#include "Mouse.h"
class CoordinateTransformer {
public:
	CoordinateTransformer(Graphics& gfx0)
		:gfx(gfx0)
	{

	}
	template<typename T>
	Vec2_<T> Transform(Vec2_<T> pos)
	{
		pos.x += Graphics::ScreenWidth / 2;
		pos.y -= Graphics::ScreenHeight / 2;
		pos.y = -pos.y;
		return pos;
	}
	template<typename T>
	Vec2_<T> Transform(T x, T y)
	{
		// reversing the y axis and making the bottom left the { 0 , 0 } point
		x += Graphics::ScreenWidth / 2;
		y -= Graphics::ScreenHeight / 2;
		y = -y;
		return Vec2_<T>(x, y);
	}
	int TransformX(int x)
	{
		return x + Graphics::ScreenWidth / 2;
	}
	int TransformY(int y)
	{
		return -y + Graphics::ScreenHeight / 2;
	}
	static Vec2I TransformMousePos(const Mouse& mouse)
	{
		Vec2I pos = mouse.GetPos();
		pos.x += Graphics::ScreenWidth / 2;
		pos.y -= Graphics::ScreenHeight / 2;
		pos.y = -pos.y;
		return pos;

	}
	static Vec2I TransformScrPos(Vec2I pos)
	{
		pos.x += Graphics::ScreenWidth / 2;
		pos.y -= Graphics::ScreenHeight / 2;
		pos.y = -pos.y;
		return pos;
	}
private:
	Graphics& gfx;
};