#pragma once

namespace cg
{
	template <typename T>
	struct Vec2
	{
		Vec2() noexcept;
		Vec2(T x, T y) noexcept;

		template <typename U>
		explicit Vec2(const Vec2<U>& vector) noexcept;

		T x;
		T y;
	};

	template <typename T>
	[[nodiscard]]
	inline Vec2<T> operator-(const Vec2<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T>& operator +=(Vec2<T>& left, const Vec2<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T>& operator -=(Vec2<T>& left, const Vec2<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T> operator +(const Vec2<T>& left, const Vec2<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T> operator -(const Vec2<T>& left, const Vec2<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T> operator *(const Vec2<T>& left, T right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T> operator *(T left, const Vec2<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T>& operator *=(Vec2<T>& left, T right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T> operator /(const Vec2<T>& left, T right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline Vec2<T>& operator /=(Vec2<T>& left, T right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline bool operator ==(const Vec2<T>& left, const Vec2<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline bool operator !=(const Vec2<T>& left, const Vec2<T>& right) noexcept;

	typedef Vec2<int>			Vec2i;
	typedef Vec2<unsigned int>	Vec2u;
	typedef Vec2<float>			Vec2f;
} // namespace cg

#include "Vec2.inl"