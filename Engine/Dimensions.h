#pragma once

template<typename T>
struct Dimensions {
	constexpr Dimensions() = default;
	constexpr Dimensions(T width0, T height0)
		:width(width0) , height(height0)
	{
	}
	Dimensions operator/ (float val) const
	{
		return Dimensions<T>( width / val , height / val );
	}
	Dimensions operator* (float val) const
	{
		return Dimensions<T>(width * val, height * val);
	}
	T GetArea() const {
		return width * height;
	}
	T width;
	T height;
};