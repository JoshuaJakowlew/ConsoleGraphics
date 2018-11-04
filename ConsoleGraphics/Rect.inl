namespace cg
{
	template <typename T>
	Rect<T>::Rect() noexcept :
		x(0), y(0), width(0), height(0)
	{}

	template <typename T>
	Rect<T>::Rect(T x, T y, T width, T height) noexcept :
		x(x), y(y), width(width), height(height)
	{}

	template <typename T>
	Rect<T>::Rect(const Vec2<T>& position, const Vec2<T>& size) noexcept :
		x(position.x), y(position.y),
		width(size.x), height(size.y)
	{}

	template <typename T>
	template <typename U>
	Rect<T>::Rect(const Rect<U>& rectangle) noexcept :
		x(static_cast<T>(rectangle.x)),
		y(static_cast<T>(rectangle.y)),
		width(x(static_cast<T>(rectangle.width))),
		height(x(static_cast<T>(rectangle.height)))
	{}

	template <typename T>
	inline bool operator ==(const Rect<T>& left, const Rect<T>& right) noexcept
	{
		return (left.x == right.x) && (left.width == right.width) &&
			(left.y == right.y) && (left.height == right.height);
	}

	template <typename T>
	inline bool operator !=(const Rect<T>& left, const Rect<T>& right) noexcept
	{
		return !(left == right);
	}

	template <typename T>
	void sizeToCoords(T& width, T& height)
	{
		if (width > 0) width = width - 1;
		if (height > 0) height = height - 1;

		if (width < 0) width = width + 1;
		if (height < 0) height = height + 1;
	}

	template <typename T>
	Rect<T> rectToCoords(const Rect<T>& rect)
	{
		T width = rect.width;
		T height = rect.height;

		sizeToCoords(width, height);

		return Rect<T>(rect.x, rect.y, width, height);
	}
} // namespace cg