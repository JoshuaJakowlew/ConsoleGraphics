#ifndef API_H
#define API_H

#include <mutex>

#include <thirdparty/sol.hpp>

#include <application/Application.h>
#include <resources/ReourceHolder.h>
#include <animations/Animator.h>
#include <animations/FrameAnimation.h>

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
	
	template <typename K, typename V>
	void ResourceHolder(const std::string& name, sol::state& lua)
	{
		using HolderType = cg::ResourceHolder<K, V>;
		auto type = lua.new_usertype <HolderType>(
			name,
			sol::constructors<HolderType()>(),

			sol::meta_function::index,
			[](HolderType& holder, std::string key)
			{
				return holder[std::move(key)];
			});

		type["acquire"] = &HolderType::acquire;
		type["release"] = &HolderType::release;
	}

	extern void FrameAnimation(const std::string& name, sol::state& lua);

	template <typename Animated>
	void Animator(const std::string& name, sol::state& lua)
	{
		using AnimatorType = cg::Animator<Animated>;
		auto type = lua.new_usertype<AnimatorType>(
			name,
			sol::constructors<AnimatorType()>());

		type["addAnimation"] = &AnimatorType::addAnimation;
		type["removeAnimation"] = &AnimatorType::removeAnimation;
		
		type["play"] = &AnimatorType::play;
		type["stop"] = &AnimatorType::stop;
		type["update"] = &AnimatorType::update;

		type["playAnimation"] = &AnimatorType::playAnimation;
		type["isPlayingAnimation"] = &AnimatorType::isPlayingAnimation;
		type["getPlayingAnimation"] = &AnimatorType::getPlayingAnimation;
	}

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

	template <>
	constexpr auto name<cg::ResourceHolder<std::string, cg::Texture>> = "TextureHolder";

	template <>
	constexpr auto name<cg::ResourceHolder<std::string, cg::Sprite>> = "SpriteHolder";

	template <>
	constexpr auto name<cg::FrameAnimation> = "FrameAnimation";

	template <>
	constexpr auto name<cg::Animator<cg::FrameAnimation>> = "FrameAnimator";


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

	inline void TextureHolder(sol::state& lua)
	{
		detail::ResourceHolder<std::string, cg::Texture>(
			type::name<cg::ResourceHolder<std::string, cg::Texture>>, lua);
	}

	inline void SpriteHolder(sol::state& lua)
	{
		detail::ResourceHolder<std::string, cg::Sprite>(
			type::name<cg::ResourceHolder<std::string, cg::Sprite>>, lua);
	}

	inline void FrameAnimation(sol::state& lua)
	{
		detail::FrameAnimation(type::name<cg::FrameAnimation>, lua);
	}

	inline void FrameAnimator(sol::state& lua)
	{
		detail::Animator<cg::FrameAnimation>(
			type::name<cg::Animator<cg::FrameAnimation>>, lua);
	}

} // namespace cg::lua

#endif
