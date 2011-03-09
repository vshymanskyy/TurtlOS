#pragma once
#include "CliWindow.h"
#include <std/List.h>

class CliPanel :
	public CliWindow
{
private:
	List<CliWindow*> _children;

public:
	CliPanel(const Rect& bounds)
		: CliWindow(bounds)
	{}

	virtual ~CliPanel() {};

	void Redraw(const Rect& region) {
		CliWindow::Redraw(region);
		for(List<CliWindow*>::It it = _children.First(); it != _children.End(); ++it) {
			Rect b = _children[it]->Bounds();
			if (b.Intersects(region)) {
				Rect bc(b.Intersect(region));
				bc.Move(Vec2d(-b.Left(), -b.Top()));
				_canvas.Copy(_children[it]->_canvas, bc, bc.TopLeft()+b.TopLeft());
			}
		}
	}

	void AddChild(CliWindow* win) {
		_children.Append(win);
		win->Parent(this);
		win->Invalidate();
	}

	void RemoveChild(CliWindow* win) {
		int index = 0;
		for(List<CliWindow*>::It it = _children.First(); it != _children.End(); ++it) {
			if (_children[it] == win) {
				_children.Remove(it);
			}
			index++;
		}
		win->Parent(NULL);
		Invalidate(win->Bounds());
	}

};

