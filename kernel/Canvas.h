#pragma once

#include <std/Rect.h>
#include <memory.h>

/// Rectangular canvas for software painting
template <typename T>
class Canvas {

public:
	/// @returns buffer
	T* Buffer() const { return _buffer; }
	/// @param val new buffer
	void Buffer(T* val) { _buffer = val; }

	/// @returns pointer to the end of buffer
	T* BufferEnd() const { return _buffer + _size.x*_size.y; }


	/// @returns Reference to size
	Vec2d& Size() { return _size; }
	/// @returns Reference to constant size
	const Vec2d& Size() const { return _size; }

	/// Computes the memory location of coordinates
	T* OffsetOf(int x, int y) const{
		return _buffer + _size.x * y + x;
	}

	/// Computes the memory location of coordinates
	T* OffsetOf(const Vec2d& p) const{
		return OffsetOf(p.x, p.y);
	}

	/// Computes the relative offset of specified memory location
	int GetOffset(T* ptr) const{
		return static_cast<int>(((size_t)ptr-(size_t)_buffer)/sizeof(T));
	}

	/// Computes the coordinates of specified memory location
	Vec2d OffsetLocation(T* ptr) const{
		return OffsetLocation(GetOffset(ptr));
	}

	/// Computes the coordinates of specified memory offset
	Vec2d OffsetLocation(int offset) const{
		return Vec2d(offset%_size.x, offset/_size.x);
	}

	/// Return local drawable bounds
	Rect GetDrawBounds() const{
		return Rect(0, 0, _size.x, _size.y);
	}

	Canvas(T* buffer, int w, int h)
		: _size(w, h)
		, _buffer(buffer)
	{}

	void SetPixel(const Vec2d& p, T color) {
		*OffsetOf(p) = color;
	}

	void Fill(T color) {
		const uint16_t* const e = _buffer + _size.x*_size.y;
		for (uint16_t* p = _buffer; p < e; p++) {
			*p = color;
		}
	}

	void Fill(const Rect& r, T color) {
		const Rect bounds = GetDrawBounds();
		if (r.Intersects(bounds)) {
			Rect clipped = r.Intersect(bounds);
			T* buffer = OffsetOf(clipped.TopLeft());
			for(int y = 0; y < clipped.Height(); y++) {
				for(int x = 0; x < clipped.Width(); x++) {
					*(buffer+x) = color;
				}
				buffer += _size.x;
			}
		}
	}

	void Copy(const Canvas<T>& canvas, const Rect& from, const Vec2d& to) {
		Rect src_clipped = canvas.GetDrawBounds().Intersect(from);
		Rect to_rect(to.x, to.y, src_clipped.Width(), src_clipped.Height());
		to_rect.Move(src_clipped.TopLeft() - from.TopLeft());
		Rect dst_clipped = GetDrawBounds().Intersect(to_rect);

		src_clipped.Move(dst_clipped.TopLeft() - to_rect.TopLeft());
		src_clipped.Size(src_clipped.Size() + to_rect.TopLeft() - dst_clipped.TopLeft());

		src_clipped.Width(min(src_clipped.Width(), dst_clipped.Width()));
		src_clipped.Height(min(src_clipped.Height(), dst_clipped.Height()));

		T* from_buff = canvas.OffsetOf(src_clipped.TopLeft());
		T* to_buff = OffsetOf(dst_clipped.TopLeft());

		if (from_buff == to_buff)
			return;

		if (from_buff > to_buff) {
			for(int y = 0; y < src_clipped.Height(); y++) {
				memmove(to_buff, from_buff, src_clipped.Width()*sizeof(T));
				to_buff += _size.x;
				from_buff += canvas._size.x;
			}
		} else {
			from_buff += (src_clipped.Height()-1)*canvas._size.x;
			to_buff += (src_clipped.Height()-1)*_size.x;
			for(int y = src_clipped.Height(); y > 0; y--) {
				memmove(to_buff, from_buff, src_clipped.Width()*sizeof(T));
				to_buff -= _size.x;
				from_buff -= canvas._size.x;
			}
		}
	}

private:
	Vec2d	_size;		/// Size of canvas
	T*		_buffer;	/// Memory buffer
};

