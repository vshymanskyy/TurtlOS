#pragma once
#include "CliWindow.h"
#include <std/List.h>

class CliPanel :
	public CliWindow
{
private:
	List<CliWindow*> mChildren;

public:
	CliPanel(const Rect& bounds)
		: CliWindow(bounds)
	{}

	virtual ~CliPanel() {};

	void Redraw(const Rect& region) {
		CliWindow::Redraw(region);
		for(List<CliWindow*>::Iterator it = mChildren.First(); it != mChildren.End(); ++it) {
			Rect b = mChildren[it]->Bounds();
			if (b.Intersects(region)) {
				Rect bc(b.Intersect(region));
				bc.Move(Vec2d(-b.Left(), -b.Top()));
				mCanvas.Copy(mChildren[it]->mCanvas, bc, bc.TopLeft()+b.TopLeft());
			}
		}
	}

	void AddChild(CliWindow* win) {
		mChildren.Append(win);
		win->Parent(this);
		win->Invalidate();
	}

	void RemoveChild(CliWindow* win) {
		int index = 0;
		for(List<CliWindow*>::Iterator it = mChildren.First(); it != mChildren.End(); ++it) {
			if (mChildren[it] == win) {
				mChildren.RemoveAt(it);
			}
			index++;
		}
		win->Parent(NULL);
		Invalidate(win->Bounds());
	}

};

