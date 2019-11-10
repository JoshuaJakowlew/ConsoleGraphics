#ifndef API_H
#define API_H

#include <thirdparty/sol.hpp>

#include <utils/Vec2.h>

namespace cg::lua::detail
{
	template <typename T>
	void Vec2(const std::string& name, sol::state& lua)
	{
		auto type = lua.new_usertype<cg::Vec2<T>>(
			name,
			sol::constructors<cg::Vec2<T>(), cg::Vec2<T>(T, T)>());

		type["x"] = sol::property(
			[](const cg::Vec2<T>& vec) { return vec.x; },
			[](cg::Vec2<T>& vec, unsigned val) { vec.x = val; });

		type["y"] = sol::property(
			[](const cg::Vec2<T>& vec) { return vec.y; },
			[](cg::Vec2<T>& vec, unsigned val) { vec.y = val; });

		type["neg"] = [](const cg::Vec2<T>& right) { return -right; };

		type["add"] = [](const cg::Vec2<T>& left, const cg::Vec2<T>& right) { return left + right; };

		type["sub"] = [](const cg::Vec2<T>& left, const cg::Vec2<T>& right) { return left - right; };

		type["mul"] = sol::overload(
			[](T left, const cg::Vec2<T>& right) { return left * right; },
			[](const cg::Vec2<T>& left, T right) { return left * right; });

		type["div"] = [](const cg::Vec2<T>& left, T right) { return left / right; };
	}
}

namespace cg::lua::type
{
	template <typename T = void>
	constexpr auto name = {};

	template <>
	constexpr auto name<cg::Vec2i> = "Vec2i";

	template <>
	constexpr auto name<cg::Vec2u> = "Vec2u";

	template <>
	constexpr auto name<cg::Vec2f> = "Vec2f";
}

namespace cg::lua
{
	void Vec2(sol::state& lua)
	{
		detail::Vec2<int>(type::name<cg::Vec2<int>>, lua);
		detail::Vec2<unsigned>(type::name<cg::Vec2<unsigned>>, lua);
		detail::Vec2<float>(type::name<cg::Vec2<float>>, lua);
	}

} // namespace cg::lua

#endif
