#pragma once
#include "CoordinateTransformer.h"
#include "Mouse.h"
class Camera {
public:
	Camera(CoordinateTransformer& ct0)
		:ct(ct0)
	{
		//Vec2I camera_pos = Vec2I(Graphics::ScreenWidth / 2 , Graphics::ScreenHeight / 2 );
		WorldPos = { 0, 0 };
		ZonePos = Vec2D(0.0f, 0.0f);
		OffLimitsZone = RectI(Graphics::ScreenWidth + 2 * OffLimitDist, Graphics::ScreenHeight + 2 * OffLimitDist,
			Vec2I(-OffLimitDist, -OffLimitDist));

		TargetPos = { 0 , 0 };
		vel = { 0.0f , 0.0f };
	}
	RectI GetRect() const
	{
		return RectI(Graphics::ScreenWidth, Graphics::ScreenHeight, GetPos());
	}
	int TransformX(int x)
	{
		return ct.TransformX((x - WorldPos.x) * Scale);
	}
	int TransformY(int y)
	{
		return ct.TransformY((y - WorldPos.y) * Scale);
	}
	template <typename T>
	Vec2I Transform(Vec2_<T> pos0)
	{
		Vec2I cPos = Vec2I(pos0.x - GetX(), pos0.y - GetY());
		cPos = Vec2I(cPos.x * Scale, cPos.y * Scale);

		return ct.Transform(std::move(cPos));
	}
	Vec2I MouseTransformation(Mouse& mouse)
	{
		return ct.TransformMousePos(mouse);
	}
	template<typename T, typename D>
	RectI Transform(Rect_<T, D> rect)
	{
		return RectI(rect.width * Scale, rect.height * Scale,
			Vec2I(ct.TransformX((rect.left - GetX()) * Scale), ct.TransformY((rect.top() - GetY()) * Scale)));
	}
	//transforms only to the camera
	template <typename T>
	Vec2I CameraTransformation(Vec2_<T> pos0)
	{
		Vec2I cPos = Vec2I(pos0.x, pos0.y);
		return Vec2I(cPos - GetPos());
	}
	template <typename T>
	void MoveBy(Vec2_<T> pos0)
	{
		Vec2I cPos = Vec2I(pos0.x, pos0.y);
		WorldPos = WorldPos + cPos;
	}
	void MoveLeft(int x)
	{
		assert(x < 0);
		if (WorldPos.x + x >= Graphics::WorldArea.left)
		{
			WorldPos.x += x;
		}
	}
	void MoveRight(int x)
	{
		assert(x > 0);
		if (WorldPos.x + Graphics::ScreenWidth + x < Graphics::WorldArea.right())
		{
			WorldPos.x += x;
		}
	}
	void MoveUp(int y)
	{
		assert(y > 0);
		if (WorldPos.y + Graphics::ScreenHeight + y < Graphics::WorldArea.top())
		{
			WorldPos.y += y;
		}
	}
	void MoveDown(int y)
	{
		assert(y < 0);
		if (WorldPos.y + y >= Graphics::WorldArea.bottom)
		{
			WorldPos.y += y;
		}
	}
	void ChangeZoom(Mouse& mouse)
	{
		Mouse::Event event = mouse.Read();

		if (event.GetType() == Mouse::Event::Type::WheelDown)
		{
			Scale *= 0.95f;
		}
		else if (event.GetType() == Mouse::Event::Type::WheelUp)
		{
			Scale *= 1.05f;
		}
	}

	template<typename T>
	bool WithinWorldX(T x) const {
		return int(x) >= Graphics::WorldArea.left && int(x) + Graphics::ScreenWidth < Graphics::WorldArea.right();
	}
	template<typename T>
	bool WithinWorldY(T y) const {
		return int(y) >= Graphics::WorldArea.bottom && int(y) + Graphics::ScreenHeight < Graphics::WorldArea.top();
	}
	template <typename T>

	void SetPos(Vec2_<T> pos0)
	{
		//checking if in bounds on x axis
		if (WithinWorldX(pos0.x))
		{
			WorldPos.x = std::move(pos0.x);
		}
		else
		{
			int dist_left = std::abs(Graphics::WorldArea.left - int(pos0.x));
			int dist_right = std::abs(Graphics::WorldArea.right() - int(pos0.x));

			if (dist_left < dist_right)
			{
				WorldPos.x = Graphics::WorldArea.left;
			}
			else
			{
				WorldPos.x = Graphics::WorldArea.right() - Graphics::ScreenWidth;
			}
		}

		//checking if in bounds on y axis
		if (WithinWorldY(pos0.y))
		{
			WorldPos.y = std::move(pos0.y);
		}
		else
		{
			int dist_top = std::abs(Graphics::WorldArea.bottom - int(pos0.y));
			int dist_bottom = std::abs(Graphics::WorldArea.top() - int(pos0.y));

			if (dist_top < dist_bottom)
			{
				WorldPos.y = Graphics::WorldArea.bottom;
			}
			else
			{
				WorldPos.y = Graphics::WorldArea.top() - Graphics::ScreenHeight;
			}
		}
		CheckIfOffBounds();
	}
	void MouseMovement(Mouse& mouse)
	{
		//converting mouse pos to the cartezian system , making the center of the screen the origin
		Vec2I mouse_pos = mouse.GetPos();
		mouse_pos.y = Graphics::ScreenHeight / 2 - mouse_pos.y;
		mouse_pos.x -= Graphics::ScreenWidth / 2;
		//converting the mouse pos tothe pos within the zone
		mouse_pos = mouse_pos + Vec2I(int(ZonePos.x), int(ZonePos.y));

		int distX = (-OffLimitDist + OffLimitsZone.width / 2) - mouse_pos.x;

		//idk why i put minus before dist , when I did the camera system when looking left it shifted to the right and vice versa
		//it just works
		//the parantheses basically calculates the x of the OfflimitsZone

		
		TargetPos.x = -distX / MoveRatioX;

		vel.x = (TargetPos.x - ZonePos.x) / MoveFactor;

		int distY = (- OffLimitDist) + OffLimitsZone.height / 2 - mouse_pos.y;


		TargetPos.y = -distY / MoveRatioY;

		vel.y = (TargetPos.y - ZonePos.y) / MoveFactor;
	}
	void CameraDrag(Mouse& mouse, MouseLastFrameStats& stats)
	{
		if (stats.LeftIsPressed() && mouse.LeftIsPressed())
		{
			Vec2I New = ct.TransformMousePos(mouse);
			Vec2I Old = ct.TransformScrPos(stats.GetPos());

			Vec2I OffSet = Old - New;

			MoveBy(OffSet);
		}
	}
	void UpdateMovement(float time)
	{
		float AddX = time * vel.x * 60.0f;
		if (std::abs(ZonePos.x + AddX) < OffLimitDist)
		{
			if (WithinWorldX(GetX() + AddX) && std::abs(ZonePos.x + AddX) < OffLimitDist)
			{
				ZonePos.x += AddX;
			}
		}
		vel.x *= DecreaseFactor * (1.0f - time);

		float AddY = time * vel.y * 60.0f;
		if (WithinWorldY(GetY() + AddY) && std::abs(ZonePos.y + AddY) < OffLimitDist)
		{
			ZonePos.y += AddY;
		}
		vel.y *= DecreaseFactor * (1.0f - time);
	}
	//this checks if the camera is outside the camera boundaries because of its position in the zone
	void CheckIfOffBounds()
	{
		if (GetX() + Graphics::ScreenWidth >= Graphics::WorldArea.right())
		{
			ZonePos.x -= GetX() + Graphics::ScreenWidth - Graphics::WorldArea.right();
		}

		if (GetY() + Graphics::ScreenHeight >= Graphics::WorldArea.top())
		{
			ZonePos.y -= GetY() + Graphics::ScreenHeight - Graphics::WorldArea.top();
		}

		if (GetX() < Graphics::WorldArea.left)
		{
			ZonePos.x += Graphics::WorldArea.left - GetX() - 1;
		}

		if (GetY() < Graphics::WorldArea.bottom)
		{
			ZonePos.y += Graphics::WorldArea.bottom - GetY() - 1;
		}
	}
	template <typename T>
	Vec2I ToCamera(Vec2_<T> pos0)
	{
		Vec2I pos = { int(std::move(pos0.x)) , int(std::move(pos0.y)) };
		return ct.Transform(pos);
	}
	Vec2I GetPos()const {
		return Vec2I(GetX(), GetY());
	}
	int GetX() const {
		return (WorldPos.x + ZonePos.x);
	}
	int GetY() const {
		return (WorldPos.y + ZonePos.y);
	}
	void UpdateZone()
	{
		OffLimitsZone.SetPos(Vec2I(WorldPos.x - OffLimitDist, WorldPos.y - OffLimitDist));
	}
	float GetScale() const {
		return Scale;
	}

	static constexpr  int OffLimitDist = Graphics::ScreenWidth / 20;
	static constexpr  float MoveFactor = 100.0f;
	static constexpr  float DecreaseFactor = 0.6f;
	static constexpr float MoveRatioX = (Graphics::ScreenWidth / 2) / OffLimitDist;
	static constexpr float MoveRatioY = (Graphics::ScreenHeight / 2) / OffLimitDist;
private:
	Vec2I WorldPos;
	Vec2D ZonePos;

	//taget pos in the offlimits zone
	Vec2I TargetPos;
	RectI OffLimitsZone;
	float Scale = 1.0f;
	Vec2D vel;

	CoordinateTransformer& ct;
};