#pragma once
#include "CliWindow.h"

/// Frame Console Window
class CliFrame
	: public CliWindow
{
private:
	/// Content
	CliWindow* mClient;

public:
	/// Constructor
	CliFrame(const Rect& bounds, CliWindow* client)
		: CliWindow(bounds)
		, mClient(client)
	{
		mClient->Parent(this);
		ClientOffset(Vec2d(1, 1));
	}

	/// Destructor
	~CliFrame() {
	}

	/// Overloaded
	void SetCursor (const Vec2d& p) {
		Rect r2 = mCanvas.GetDrawBounds();
		r2.Width(r2.Width() - 2);
		r2.Height(r2.Height() - 2);
		r2.Left(r2.Height() + 1);
		r2.Top(r2.Top() + 1);
		if (r2.Contains(p)) {
			if (mParent) {
				mParent->SetCursor(p + mOffset + mParent->ClientOffset());
			}
		} else {
			mParent->SetCursor(Vec2d(-1,-1));
		}
	}

	/// Overloaded
	void Redraw(const Rect& region) {
		CliWindow::Redraw(region);

		Rect b = mClient->Bounds();
		b.Move(ClientOffset());
		if (b.Intersects(region)) {
			Rect bc(b.Intersect(region));
			bc.Move(Vec2d(-b.Left(), -b.Top()));
			mCanvas.Copy(mClient->mCanvas, bc, bc.TopLeft()+b.TopLeft());
		}

		for(int i=1; i<mCanvas.Size().x-1; i++) {
			*mCanvas.OffsetOf(i,0) = GetAttr(205);
			*mCanvas.OffsetOf(i,mCanvas.Size().y-1) = GetAttr(205);
		}
		for(int i=1; i<mCanvas.Size().y-1; i++) {
			*mCanvas.OffsetOf(0,i) = GetAttr(186);
			*mCanvas.OffsetOf(mCanvas.Size().x-1,i) = GetAttr(186);
		}
		*mCanvas.OffsetOf(0, 0) = GetAttr(201);
		*mCanvas.OffsetOf(0, mCanvas.Size().y-1) = GetAttr(200);
		*mCanvas.OffsetOf(mCanvas.Size().x-1, 0) = GetAttr(187);
		*mCanvas.OffsetOf(mCanvas.Size().x-1, mCanvas.Size().y-1) = GetAttr(188);

		const char* capt = mCaption;
		uint16_t* cdst = mCanvas.OffsetOf(2, 0);
		*cdst++ = GetAttr(' ');
		while(*capt) {
			*cdst++ = GetAttr(*capt++);
		}
		*cdst++ = GetAttr(' ');

		*mCanvas.OffsetOf(mCanvas.Size().x-2, 0) = GetAttr('X');
		*mCanvas.OffsetOf(mCanvas.Size().x-3, 0) = GetAttr('.');
	}

};

