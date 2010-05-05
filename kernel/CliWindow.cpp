#include "CliWindow.h"

CliWindow::CliWindow (const Rect& bounds)
	: mOffset(bounds.TopLeft())
	, mBackground(BLACK)
	, mForeground(WHITE)
	, mCanvas(new uint16_t[bounds.Width()*bounds.Height()], bounds.Width(), bounds.Height())
{
}

CliWindow::~CliWindow()
{
	if (mCanvas.Buffer())
		delete[] mCanvas.Buffer();
}

void
CliWindow::Redraw(const Rect& region)
{
	mCanvas.Fill(region, GetAttr(0));
}

void
CliWindow::Invalidate()
{
	Rect bounds = mCanvas.GetDrawBounds();
	Redraw(bounds);
	if (mParent) {
		bounds = Bounds();
		bounds.Move(mParent->ClientOffset());
		mParent->Invalidate(bounds);
	}
}

void
CliWindow::Invalidate (const Rect& region)
{
	Rect bounds = mCanvas.GetDrawBounds();
	if (bounds.Intersects(region)) {
		bounds = bounds.Intersect(region);
		Redraw(bounds);
		if (mParent) {
			bounds.Move(Bounds().TopLeft()+mParent->ClientOffset());
			mParent->Invalidate(bounds);
		}
	}
}

void
CliWindow::Move(const Vec2d& p)
{
	Rect prev_b = Bounds();
	mOffset = p;
	if (mParent) {
		prev_b.Move(mParent->ClientOffset());
		mParent->Invalidate(prev_b);

		prev_b = Bounds();
		prev_b.Move(mParent->ClientOffset());
		mParent->Invalidate(prev_b);
	}
}

void
CliWindow::SetCursor (const Vec2d& p)
{
	if (mCanvas.GetDrawBounds().Contains(p)) {
		if (mParent) {
			mParent->SetCursor(p + mOffset + mParent->ClientOffset());
		}
	} else {
		mParent->SetCursor(Vec2d(-1,-1));
	}
}

void
CliWindow::Resize (int w, int h)
{
	mCanvas.Size().x = w;
	mCanvas.Size().y = h;
	if (mCanvas.Buffer()) {
		delete[] mCanvas.Buffer();
		mCanvas.Buffer(new uint16_t[mCanvas.Size().x*mCanvas.Size().y]);
	}
	if (mParent) {
		mParent->Invalidate();
	}
}

