#pragma once
#include "Vec2D.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Dimensions.h"
#include <vector>
template <typename T, typename D>
class Rect_ {
public:
	constexpr Rect_()
	{
		left = bottom = width = height = T(0);
	}
	constexpr Rect_(T left0, T top0, T right0, T bottom0)
		:left(left0),
		bottom(top0),
		width(D(std::abs(right0 - left0))),
		height(D(std::abs(top0 - bottom0)))
	{
	}

	constexpr Rect_(Vec2_<T>& TopLeft0, Vec2_<T>& BottomRight0)
		:left(TopLeft0.x),
		bottom(TopLeft0.y),
		width(T(std::abs(TopLeft0.x - BottomRight0.x))),
		height(T(std::abs(TopLeft0.y - BottomRight0.y)))
	{
		if (TopLeft0.x > BottomRight0.x)
		{
			left = BottomRight0.x;
		}
		else
			left = TopLeft0.x;

		if (TopLeft0.y > BottomRight0.y)
		{
			bottom = BottomRight0.y;
		}
		else
			bottom = TopLeft0.y;

	}

	constexpr Rect_(D width0, D height0, Vec2_<T>& pos)
		:left(pos.x), bottom(pos.y),
		width(width0), height(height0)
	{
	}
	constexpr Rect_(D width0, D height0, const Vec2_<T>& pos)
		: left(pos.x), bottom(pos.y),
		width(width0), height(height0)
	{
	}
	constexpr Rect_(Dimensions<D> dim, Vec2_<T>& pos)
		: left(pos.x), bottom(pos.y),
		width(dim.width), height(dim.height)
	{
	}

	bool Collision(Rect_& rect) const
	{
		return (left + width >= rect.left && left < rect.left + rect.width &&
			bottom + height >= rect.bottom && bottom < rect.bottom + rect.height);
	}

	bool Hovered(Mouse& mouse)
	{
		const T x = mouse.GetPosX(), y = mouse.GetPosY();
		return (x <= left + width && x >= left &&
			y <= bottom + height && y >= bottom);
	}

	bool operator==(Rect_& rect) const
	{
		return (rect.left == left && rect.bottom == bottom && rect.width == width && rect.height == height);
	}

	bool PointInRect(Vec2_<T>& point) const {
		return (point.x <= left + width && point.x >= left &&
			point.y >= bottom && point.y <= bottom + height);
	}

	Rect_ operator* (T number) {
		return Rect_(width * number, height * number,
			Vec2_<T>(T(left * number), T(bottom * number)));
	}

	Rect_ operator/ (T number) {
		return Rect_(width / number, height / number,
			Vec2_<T>(T(left / number), T(bottom / number)));
	}

	void SwapPositions(Rect_& rect)
	{
		Vec2I pos = { left , bottom };
		left = rect.left;
		bottom = rect.bottom;

		rect.left = pos.x;
		rect.bottom = pos.y;
	}

	T right() const
	{
		return width + left;
	}
	T top() const
	{
		return height + bottom;
	}
	Vec2_<T> pos()const {
		return Vec2_<T>(left, bottom);
	}
	Dimensions<D> GetDimensions() const {
		return Dimensions<D>(D(width), D(height));
	}
	Vec2_<T> GetCenter()
	{
		return Vec2_<T>(left + width / 2, bottom + height / 2);
	}
	Vec2_<T> GetPos() const
	{
		return Vec2_<T>(left, bottom);
	}

	void SetPos(Vec2_<T> newpos)
	{
		left = newpos.x;
		bottom = newpos.y;
	}
	// get the rectangle for a poligon
	static Rect_<int, short> GetRect(const std::vector<Vec2I>& model)
	{
		int left, right, top, bottom;
		left = right = model[0].x;
		top = bottom = model[0].y;

		for (const Vec2I& vertices : model)
		{
			if (left > vertices.x)
			{
				left = vertices.x;
			}

			if (right < vertices.x)
			{
				right = vertices.x;
			}

			if (top < vertices.y)
			{
				top = vertices.y;
			}

			if (bottom > vertices.y)
			{
				bottom = vertices.y;
			}
		}

		RectI rect;
		rect.width = right - left;
		rect.height = top - bottom;
		rect.left = std::move(left);
		rect.bottom = std::move(bottom);

		return rect;
	}

	void MoveBy(Vec2_<T> offset)
	{
		left += offset.x;
		bottom += offset.y;
	}
public:
	T left = T(0);
	T bottom = T(0);

	D width = D(0);
	D height = D(0);
};

typedef Rect_<float, short> Rect;
typedef Rect_<int, short> RectI;
