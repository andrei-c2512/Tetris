#pragma once
#include <cmath>

template<typename T>
class Vec2_ {
public:
	constexpr Vec2_() = default;
	constexpr Vec2_(T x0, T y0)
		:
		x(x0),
		y(y0)
	{
	}
	Vec2_ operator+(const Vec2_<T>& dumb) const {
		return Vec2_(x + dumb.x, y + dumb.y);
	}
	Vec2_ operator-(const Vec2_<T>& dumb) const {
		return Vec2_(x - dumb.x, y - dumb.y);
	}
	Vec2_& operator-=(const Vec2_<T>& dumb) const {
		return *this = *this - dumb;
	}
	Vec2_& operator+=(const Vec2_<T>& dumb) {
		return *this = *this + dumb;
	}
	Vec2_ operator*(T val) const {
		return Vec2_(x * val, y * val);
	}
	Vec2_& operator*=(T val) {
		return *this = *this * val;
	}
	float GetLenght(Vec2_<T>& point1)const {
		const float x0 = x - point1.x;
		const float y0 = y - point1.y;
		return float(sqrt(x0 * x0 + y0 * y0));
	}
	Vec2_<float> GetNormalizedVector() const {
		float lenght = GetLenght();
		return Vec2_<float>(float(x) / lenght, float(y) / lenght);
	}
	Vec2_& operator+=(T val)
	{
		return *this = Vec2_<T>(x + val, y + val);
	}
	Vec2_& operator/=(T val)
	{
		return *this = Vec2_<T>(x / val, y / val);
	}

	Vec2_ operator/(T val)
	{
		return Vec2_<T>(x / val, y / val);
	}
	bool operator !=(Vec2_<T>& vec) const
	{
		return (vec.x != x || vec.y != y);
	}
	bool operator ==(Vec2_<T>& vec) const
	{
		return (vec.x == x && vec.y == y);
	}
	void Normalize()
	{
		float lenght = GetLenght();
		x /= lenght;
		y /= lenght;
	}
	float GetSlope(Vec2_<T>& pos)
	{
		float difX = pos.x - x;
		if (difX == 0)
		{
			return 0;
		}
		return float((float(pos.y - y)) / (float(pos.x - x)));
	}
	//get the slope perpendicular to the slope of the points
	float GetPPSlope(Vec2_<T>& pos)
	{
		// m stands for slopes;
		//m1 * m2 = -1;
		//m1 = -1 / m2;

		return -1.0f / GetSlope(pos);
	}
	float GetSin(Vec2_<T>& pos) const
	{
		float hyphotenuse = GetLenght(pos);
		return (-(y - pos.y)) / hyphotenuse;     //putting the minus because the screen isn't read in a cartezian method
	}
	float GetCos(Vec2_<T>& pos) const
	{
		float hyphotenuse = GetLenght(pos);
		return (-(x - pos.x)) / hyphotenuse;
	}
	float GetSin(Vec2_<T>& pos, float hyphotenuse) const
	{
		return (-(y - pos.y)) / hyphotenuse;
	}
	float GetCos(Vec2_<T>& pos, float hyphotenuse) const
	{
		return (-(x - pos.x)) / hyphotenuse;
	}
	float operator * (Vec2_<T> vec)
	{
		return x * vec.x + y * vec.y;
	}
	float GetLenght() const {
		return sqrt(x * x + y * y);
	}
	Vec2_& Rotate(float angle)
	{
		const float cosTheta = cos(angle);
		const float sinTheta = sin(angle);

		//be careful , you will get 0.999 which in int is still 0
		const float new_x = float(float(x) * cosTheta) - float(float(y) * sinTheta);
		const float new_y = float(float(y) * cosTheta) + float(float(x) * sinTheta);

		x = T(std::round(new_x));
		y = T(std::round(new_y));

		return *this;
	}
	Vec2_& Rotate(float cos, float sin)
	{
		const float new_x = float(float(x) * cos) - float(float(y) * sin);
		const float new_y = float(float(y) * cos) + float(float(x) * sin);

		x = T(new_x);
		y = T(new_y);

		return *this;
	}
public:
	T y;
	T x;
};

typedef Vec2_<float>  Vec2D;
typedef Vec2_<int> Vec2I;