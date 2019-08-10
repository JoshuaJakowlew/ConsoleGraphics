#ifndef VEC2_H
#define VEC2_H

namespace cg
{
	template <typename T>
	struct Vec2
	{
		constexpr Vec2() noexcept :
			x{ 0 }, y{ 0 }
		{};
		constexpr Vec2(T x, T y) noexcept :
			x{ x }, y{ y }
		{}

		template <typename U>
		constexpr explicit Vec2(const Vec2<U>& vec) noexcept :
			x{ static_cast<T>(vec.x) },
			y{ static_cast<T>(vec.y) }
		{}

		T x;
		T y;
	};

	template <typename T>
	[[nodiscard]]
	constexpr auto operator-(const Vec2<T>& right) noexcept -> Vec2<T>
	{
		return {
			-right.x,
			-right.y
		};
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator +=(Vec2<T>& left, const Vec2<T>& right) noexcept -> Vec2<T>&
	{
		left.x += right.x;
		left.y += right.y;

		return left;
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator -=(Vec2<T>& left, const Vec2<T>& right) noexcept -> Vec2<T>&
	{
		left.x -= right.x;
		left.y -= right.y;

		return left;
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator +(const Vec2<T>& left, const Vec2<T>& right) noexcept -> Vec2<T>
	{
		return {
			left.x + right.x,
			left.y + right.y
		};
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator -(const Vec2<T>& left, const Vec2<T>& right) noexcept -> Vec2<T>
	{
		return {
			left.x - right.x,
			left.y - right.y
		};
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator *(const Vec2<T>& left, T right) noexcept -> Vec2<T>
	{
		return {
			left.x * right,
			left.y * right
		};
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator *(T left, const Vec2<T>& right) noexcept -> Vec2<T>
	{
		return {
			right.x * left,
			right.y * left
		};
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator *=(Vec2<T>& left, T right) noexcept -> Vec2<T>&
	{
		left.x *= right;
		left.y *= right;

		return left;
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator /(const Vec2<T>& left, T right) noexcept -> Vec2<T>
	{
		return {
			left.x / right,
			left.y / right
		};
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator /=(Vec2<T>& left, T right) noexcept -> Vec2<T>&
	{
		left.x /= right;
		left.y /= right;

		return left;
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator ==(const Vec2<T>& left, const Vec2<T>& right) noexcept -> bool
	{
		return (left.x == right.x) && (left.y == right.y);
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto operator !=(const Vec2<T>& left, const Vec2<T>& right) noexcept -> bool
	{
		return (left.x != right.x) || (left.y != right.y);
	}

	using Vec2i = Vec2<int>;
	using Vec2u = Vec2<unsigned int>;
	using Vec2f = Vec2<float>;

} // namespace cg

#endif
