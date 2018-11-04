namespace cg
{
	template <typename T>
	Vec2<T>::Vec2() noexcept :
		x(0), y(0) {}

	template <typename T>
	Vec2<T>::Vec2(T x, T y) noexcept :
		x(x), y(y) {}

	template <typename T>
	template <typename U>
	Vec2<T>::Vec2(const Vec2<U>& vector) noexcept :
		x(static_cast<T>(vector.x)),
		y(static_cast<T>(vector.y))
	{}

	template <typename T>
	inline Vec2<T> operator-(const Vec2<T>& right) noexcept
	{
		return Vec2<T>(-right.x, -right.y);
	}

	template <typename T>
	inline Vec2<T>& operator +=(Vec2<T>& left, const Vec2<T>& right) noexcept
	{
		left.x += right.x;
		left.y += right.y;

		return left;
	}

	template <typename T>
	inline Vec2<T>& operator -=(Vec2<T>& left, const Vec2<T>& right) noexcept
	{
		left.x -= right.x;
		left.y -= right.y;

		return left;
	}

	template <typename T>
	inline Vec2<T> operator +(const Vec2<T>& left, const Vec2<T>& right) noexcept
	{
		return Vec2<T>(left.x + right.x, left.y + right.y);
	}

	template <typename T>
	inline Vec2<T> operator -(const Vec2<T>& left, const Vec2<T>& right) noexcept
	{
		return Vec2<T>(left.x - right.x, left.y - right.y);
	}

	template <typename T>
	inline Vec2<T> operator *(const Vec2<T>& left, T right) noexcept
	{
		return Vec2<T>(left.x * right, left.y * right);
	}

	template <typename T>
	inline Vec2<T> operator *(T left, const Vec2<T>& right) noexcept
	{
		return Vec2<T>(right.x * left, right.y * left);
	}

	template <typename T>
	inline Vec2<T>& operator *=(Vec2<T>& left, T right) noexcept
	{
		left.x *= right;
		left.y *= right;

		return left;
	}

	template <typename T>
	inline Vec2<T> operator /(const Vec2<T>& left, T right) noexcept
	{
		return Vec2<T>(left.x / right, left.y / right);
	}

	template <typename T>
	inline Vec2<T>& operator /=(Vec2<T>& left, T right) noexcept
	{
		left.x /= right;
		left.y /= right;

		return left;
	}

	template <typename T>
	inline bool operator ==(const Vec2<T>& left, const Vec2<T>& right) noexcept
	{
		return (left.x == right.x) && (left.y == right.y);
	}

	template <typename T>
	inline bool operator !=(const Vec2<T>& left, const Vec2<T>& right) noexcept
	{
		return (left.x != right.x) || (left.y != right.y);
	}
} // namespace cg