#ifndef INC_RECT_HPP
#define INC_RECT_HPP

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include "Vec2d.h"

/// Rectangular region
class Rect
{
private:
	/// Left border
	int mLeft;
	/// Top border
	int mTop;
	/// Width
	int mWidth;
	/// Height
	int mHeight;

public:
	/// Default constructor
	Rect()
		: mLeft(0)
		, mTop(0)
		, mWidth(0)
		, mHeight(0)
	{}

	/// Constructor
	Rect(int l, int t, int w, int h)
		: mLeft(l)
		, mTop(t)
		, mWidth(w)
		, mHeight(h)
	{}

	/// @returns Left border
	int Left() const{   return mLeft; }
	/// @returns Top border
	int Top() const{    return mTop; }
	/// @returns Right border
	int Right() const{  return mLeft+mWidth; }
	/// @returns Bottom border
	int Bottom() const{ return mTop+mHeight; }

	/// @returns Width
	int Width() const{  return mWidth; }
	/// @returns Height
	int Height() const{ return mHeight; }

	/// @param l new value for mLeft border
	void Left(int l) {   mLeft = l; }
	/// @param t new value for mTop border
	void Top(int t) {    mTop = t; }
	/// @param r new value for right border
	void Right(int r) {  mWidth = r-mLeft; }
	/// @param b new value for bottom border
	void Bottom(int b) { mHeight = b-mTop; }

	/// @param w new value for mWidth
	void Width(int w) {  mWidth = w; }
	/// @param h new value for mHeight
	void Height(int h) { mHeight = h; }

	/// @returns Top-mLeft point
	Vec2d TopLeft() const{ return Vec2d(mLeft, mTop); }
	/// @returns Size
	Vec2d Size() const{ return Vec2d(mWidth, mHeight); }
	/// @returns Bottom-right point
	Vec2d BottomRight() const{ return TopLeft()+Size(); }

	/// Tests against point intersection
	bool Contains(const Vec2d& p) const{
		return !(Left() > p.x || Right() <= p.x || Top() > p.y || Bottom() <= p.y);
	}

	/// Tests against intersection with another Rect
	bool Intersects(const Rect& r) const{
		return !(Left() > r.Right() || Right() < r.Left() ||
			Top() > r.Bottom() || Bottom() < r.Top());
	}

	/// Moves a rectangle to a relative offset
	void Move(const Vec2d& d) {
		mLeft += d.x;
		mTop += d.y;
	}

	/// @param s new size
	void Size(const Vec2d& s) {
		mWidth = s.x;
		mHeight = s.y;
	}

	/// Computes the intersection of two rectangles
	Rect Intersect(const Rect& r) const{
		Rect result;
		result.Left(max(Left(), r.Left()));
		result.Top(max(Top(), r.Top()));
		result.Right(min(Right(), r.Right()));
		result.Bottom(min(Bottom(), r.Bottom()));
		return result;
	}

	/// Computes the union of two rectangles
	Rect Union(const Rect& r) const{
		Rect result;
		result.Left(min(Left(), r.Left()));
		result.Top(min(Top(), r.Top()));
		result.Right(max(Right(), r.Right()));
		result.Bottom(max(Bottom(), r.Bottom()));
		return result;
	}
};

#endif // INC_RECT_HPP
