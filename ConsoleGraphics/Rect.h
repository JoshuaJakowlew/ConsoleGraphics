#pragma once
#include <algorithm>
#include "Vec2.h"

namespace cg
{
	template <typename T>
	class Rect
	{
	public:
		Rect() noexcept;
		Rect(T x, T y, T width, T height) noexcept;
		Rect(const Vec2<T>& position, const Vec2<T>& size) noexcept;

		template <typename U>
		explicit Rect(const Rect<U>& rectangle) noexcept;

		T x;
		T y;
		T width;
		T height;
	};

	template <typename T>
	[[nodiscard]]
	inline bool operator ==(const Rect<T>& left, const Rect<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	inline bool operator !=(const Rect<T>& left, const Rect<T>& right) noexcept;

	template <typename T>
	[[nodiscard]]
	void sizeToCoords(T& width, T& height);

	template <typename T>
	[[nodiscard]]
	Rect<T> rectToCoords(const Rect<T>& rect);

	typedef Rect<int>			IntRect;
	typedef Rect<unsigned int>	UintRect;
	typedef Rect<float>			FloatRect;
} // namespace cg

#include "Rect.inl"