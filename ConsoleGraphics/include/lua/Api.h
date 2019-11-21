#ifndef API_H
#define API_H

#include <thirdparty/sol.hpp>

#include <utils/Vec2.h>
#include <utils/Clock.h>

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

	void Clock(const std::string& name, sol::state& lua)
	{
		auto type = lua.new_usertype<cg::Clock>(
			name,
			sol::constructors<cg::Clock()>());

		type["getElapsed"] = &cg::Clock::getElapsed;

		type["restart"] = &cg::Clock::restart;
	}

	void Texture(const std::string& name, sol::state& lua)
	{
		auto type = lua.new_usertype<cg::Texture>(
			name,
			sol::constructors<cg::Texture(), cg::Texture(std::string_view)>());

		type["getSize"] = &cg::Texture::getSize;

		type["loadFromBitmap"] = &cg::Texture::loadFromBitmap;
	}

	void Color(const std::string& name, sol::state& lua)
	{
		auto type = lua.new_usertype<cg::Color>(
			name,
			sol::constructors<
				cg::Color(),
				cg::Color(wchar_t, uint16_t),
				cg::Color(wchar_t, uint16_t, uint16_t)
			>());

		type["toCharInfo"] = &cg::Color::toCharInfo;

		type["glyph"] = sol::property(
			[](const cg::Color& color) { return color.glyph; },
			[](cg::Color& color, wchar_t glyph) { color.glyph =glyph; });

		type["bgColor"] = sol::property(
			[](const cg::Color& color) { return color.bgColor; },
			[](cg::Color& color, uint16_t bgColor) { color.bgColor = bgColor; });

		type["fgColor"] = sol::property(
			[](const cg::Color& color) { return color.fgColor; },
			[](cg::Color& color, uint16_t fgColor) { color.fgColor = fgColor; });
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

	template <>
	constexpr auto name<cg::Clock> = "Clock";

	template <>
	constexpr auto name<cg::Texture> = "Texture";

	template <>
	constexpr auto name<cg::Color> = "Color";
}

namespace cg::lua
{
	void Vec2(sol::state& lua)
	{
		detail::Vec2<int>(type::name<cg::Vec2<int>>, lua);
		detail::Vec2<unsigned>(type::name<cg::Vec2<unsigned>>, lua);
		detail::Vec2<float>(type::name<cg::Vec2<float>>, lua);
	}

	void Clock(sol::state& lua)
	{
		detail::Clock(type::name<cg::Clock>, lua);
	}

	void Texture(sol::state& lua)
	{
		detail::Texture(type::name<cg::Texture>, lua);
	}

	void Color(sol::state& lua)
	{
		detail::Color(type::name<cg::Color>, lua);
	}

} // namespace cg::lua

#endif
