#pragma once
#include "Sprite.h"
#include "Camera.h"
#include "Effects.h"

class Background {
public:
	Background()
	{
		pos = { -800 , -600 };
		sprite = { "cool_background.bmp" };
	}
	void Draw(Graphics& gfx , Camera& camera)
	{
		gfx.DrawSprite(camera.Transform(Vec2I(pos.x , pos.y + sprite.GetHeight())), sprite, sprite.GetRect(), Graphics::GetScreenRect(), Effects::Copy{});
	}

private:
	Vec2I pos;
	Sprite sprite;
};