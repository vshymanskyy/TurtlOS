#pragma once

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

/// Represents integer size or a point
struct Vec2d
{
	/// X coordinate
	int x;
	/// Y coordinate
	int y;

	/// Default constructor
	Vec2d()
		: x(0)
		, y(0)
	{}

	/// Constructor
	Vec2d(int _x, int _y)
		: x(_x)
		, y(_y)
	{}

	/// Summ operator
	Vec2d operator +(const Vec2d& p) const{
		return Vec2d(x+p.x, y+p.y);
	}

	/// Substraction operator
	Vec2d operator -(const Vec2d& p) const{
		return Vec2d(x-p.x, y-p.y);
	}
};

