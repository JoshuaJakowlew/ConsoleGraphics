#ifndef API_H
#define API_H

#include <mutex>

#include <thirdparty/sol.hpp>

#include <application/Application.h>

namespace cg::lua::detail
{
	template <typename T>
	void Vec2(const std::string& name, sol::state& lua)
	{
		#pragma region TypeDefinition
		auto type = lua.new_usertype<cg::Vec2<T>>(
			name,
			sol::constructors<cg::Vec2<T>(), cg::Vec2<T>(T, T)>(),
			
			sol::meta_function::unary_minus,
			[](const cg::Vec2<T>& right)
			{
				return -right;
			},

			sol::meta_function::addition,
			[](const cg::Vec2<T>& left, const cg::Vec2<T>& right)
			{
				return left + right;
			},

			sol::meta_function::subtraction,
			[](const cg::Vec2<T>& left, const cg::Vec2<T>& right)
			{
				return left - right;
			},

			sol::meta_function::multiplication,
			[](const cg::Vec2<T>& left, T right)
			{
				return left * right;
			},

			sol::meta_function::division,
			[](const cg::Vec2<T>& left, T right)
			{
				return left / right;
			});
		#pragma endregion
		
		type["x"] = sol::property(
			[](const cg::Vec2<T>& vec) { return vec.x; },
			[](cg::Vec2<T>& vec, T val) { vec.x = val; });

		type["y"] = sol::property(
			[](const cg::Vec2<T>& vec) { return vec.y; },
			[](cg::Vec2<T>& vec, T val) { vec.y = val; });
	}

	extern void Clock(const std::string& name, sol::state& lua);
	extern void Texture(const std::string& name, sol::state& lua);
	extern void Color(const std::string& name, sol::state& lua);
	extern void Sprite(const std::string& name, sol::state& lua);
	extern void RenderSurface(const std::string& name, sol::state& lua);

} // namespace cg::lua::detail

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

	template <>
	constexpr auto name<cg::Sprite> = "Sprite";

	template <>
	constexpr auto name<cg::RenderSurface> = "RenderSurface";

} // namespace cg::lua::type

namespace cg::lua
{
	inline void Vec2(sol::state& lua)
	{
		detail::Vec2<int>(type::name<cg::Vec2<int>>, lua);
		detail::Vec2<unsigned>(type::name<cg::Vec2<unsigned>>, lua);
		detail::Vec2<float>(type::name<cg::Vec2<float>>, lua);
	}

	inline void Clock(sol::state& lua)
	{
		detail::Clock(type::name<cg::Clock>, lua);
	}

	inline void Texture(sol::state& lua)
	{
		detail::Texture(type::name<cg::Texture>, lua);
	}

	inline void Color(sol::state& lua)
	{
		detail::Color(type::name<cg::Color>, lua);
	}

	inline void Sprite(sol::state& lua)
	{
		detail::Sprite(type::name<cg::Sprite>, lua);
	}

	inline void RenderSurface(sol::state& lua)
	{
		detail::RenderSurface(type::name<cg::RenderSurface>, lua);
	}

} // namespace cg::lua

#endif
