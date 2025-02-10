#pragma once
#include "Rect.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Dimensions.h"
#include "Camera.h"

class Button {
public:
	Button(RectI& rect);
	Button(RectI& rect, Color& c);
	Button(RectI& rect, Sprite& sprite);
	virtual void Draw(Graphics& gfx , Camera& ct);
	virtual void SelectSprite(){};
	bool         IsPressed(Mouse& mouse) const;
	bool         IsHovered(Mouse& mouse) const;
protected:
	bool Selected;
	RectI HitBox;
	Sprite sprite;
	Color c;

};



class GameSpeedButtonV1 : public Button
{
public:
	GameSpeedButtonV1( const Color& default, const Color& filler ,RectI& rect)
		:Button(rect) , default_color(default) , filler_color(filler)
	{
		MaxSpeed = 5.0f;
		MinSpeed = 0.0f;
		

		IncreaseFactor = (MaxSpeed - MinSpeed) / HitBox.height;
		last_pressY = (BaseSpeed / IncreaseFactor);
		BaseY = last_pressY;
		Speed = 1.0f;
	}
	void  Update(Mouse& mouse);
	void  DetermineSpeed();
	void  Draw(Graphics& gfx, Camera& ct) override;
	void  Go(Mouse& mouse);
	float GetSpeed() const;

private:
	float MaxSpeed = 5.0f;
	float MinSpeed = 0.0f;
	static constexpr float BaseSpeed = 1.0f;
	
	float Speed = 1.0f;
	float IncreaseFactor = 0.1f;
	Color default_color = Colors::Gray;
	Color filler_color  = Colors::White;

	int BaseY;
	int last_pressY;
};

class GameSpeedButton : public Button {
public:
	GameSpeedButton(Button& button, float Speed0)
		:Speed(Speed0) , Button(button)
	{}
	void Draw(Graphics& gfx, Camera& ct) override;
	float GetSpeed()const { return Speed; };
private:
	float Speed;
};