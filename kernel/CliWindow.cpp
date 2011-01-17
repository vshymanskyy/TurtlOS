#include "CliWindow.h"

CliWindow::CliWindow (const Rect& bounds)
	: _offset(bounds.TopLeft())
	, _background(BLACK)
	, _foreground(WHITE)
	, _canvas(new uint16_t[bounds.Width()*bounds.Height()], bounds.Width(), bounds.Height())
{
}

CliWindow::~CliWindow()
{
	if (_canvas.Buffer())
		delete[] _canvas.Buffer();
}

void
CliWindow::Redraw(const Rect& region)
{
	_canvas.Fill(region, GetAttr(0));
}

void
CliWindow::Invalidate()
{
	Rect bounds = _canvas.GetDrawBounds();
	Redraw(bounds);
	if (_parent) {
		bounds = Bounds();
		bounds.Move(_parent->ClientOffset());
		_parent->Invalidate(bounds);
	}
}

void
CliWindow::Invalidate (const Rect& region)
{
	Rect bounds = _canvas.GetDrawBounds();
	if (bounds.Intersects(region)) {
		bounds = bounds.Intersect(region);
		Redraw(bounds);
		if (_parent) {
			bounds.Move(Bounds().TopLeft()+_parent->ClientOffset());
			_parent->Invalidate(bounds);
		}
	}
}

void
CliWindow::Move(const Vec2d& p)
{
	Rect prev_b = Bounds();
	_offset = p;
	if (_parent) {
		prev_b.Move(_parent->ClientOffset());
		_parent->Invalidate(prev_b);

		prev_b = Bounds();
		prev_b.Move(_parent->ClientOffset());
		_parent->Invalidate(prev_b);
	}
}

void
CliWindow::SetCursor (const Vec2d& p)
{
	if (_canvas.GetDrawBounds().Contains(p)) {
		if (_parent) {
			_parent->SetCursor(p + _offset + _parent->ClientOffset());
		}
	} else {
		_parent->SetCursor(Vec2d(-1,-1));
	}
}

void
CliWindow::Resize (int w, int h)
{
	_canvas.Size().x = w;
	_canvas.Size().y = h;
	if (_canvas.Buffer()) {
		delete[] _canvas.Buffer();
		_canvas.Buffer(new uint16_t[_canvas.Size().x*_canvas.Size().y]);
	}
	if (_parent) {
		_parent->Invalidate();
	}
}

