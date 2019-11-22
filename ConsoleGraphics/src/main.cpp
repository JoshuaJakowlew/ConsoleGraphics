#include <iostream>
#include <string>
#include <chrono>

#include "cg.h"

using namespace std::literals;

class TestApp : public cg::Application
{
public:
	using Application::Application;

	void setup() noexcept override;
	void processEvent(const cg::Event& e) noexcept override;
	void update(float dt) noexcept override;
	void draw() noexcept override;

private:
	constexpr static cg::Palette palette = {
			RGB(53, 14, 88), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
			RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
			RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
			RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255)
	};

	cg::ResourceHolder<std::string, cg::Texture> m_textures;
	std::vector<cg::Sprite> m_sprites;
	cg::Animator<cg::FrameAnimation> m_animator;

	float speed = 25.f;
};

int main()
{
	using namespace std;

	cg::LuaApplication app{ { 200, 100 }, { 4, 4 } };
	app.start();


	//sol::state lua;
	//lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::string);

	//cg::lua::Vec2(lua);
	//cg::lua::Clock(lua);
	//cg::lua::Texture(lua);
	//cg::lua::Color(lua);
	//cg::lua::Sprite(lua);
	//// cg::lua::LuaApplication(lua);

	//auto mainScript = lua.script_file("assets/scripts/main.lua");
}

void TestApp::setup() noexcept
{
	m_console.setPalette(palette);

	m_textures.acquire("bg", { "assets/textures/bg.bmp", palette });
	m_textures.acquire("mario", { "assets/textures/sprite.bmp", palette });
	m_textures.acquire("animation", { "assets/textures/animation.bmp", palette });

	cg::Sprite bg{ m_textures["bg"] };
	bg.setOrigin({ 0, 0 });
	m_sprites.emplace_back(bg);

	cg::Sprite sprite{ m_textures["animation"], {0, 0}, {15, 15} };
	sprite.setPos({ 25, 25 });
	//sprite.setTransparent(true);
	m_sprites.emplace_back(sprite);

	cg::Sprite sprite1{ m_textures["mario"], { 0, 0 }, { 15, 7 } };
	sprite1.setPos({ 25, 25 });
	sprite1.setTransparent(true);
	m_sprites.emplace_back(sprite1);

	cg::FrameAnimation animation;
	animation.addFrame(0.5, { 0, 0 }, { 15, 15 });
	animation.addFrame(0.5, { 16, 0 }, { 31, 15 });
	m_animator.addAnimation("1"s, std::move(animation));

	animation = cg::FrameAnimation();
	animation.addFrame(0.5, { 0, 16 }, { 15, 31 });
	animation.addFrame(0.5, { 16, 16 }, { 31, 31 });
	m_animator.addAnimation("2"s, std::move(animation));

	animation = cg::FrameAnimation();
	animation.addFrame(0.5, { 0, 32 }, { 15, 47 });
	animation.addFrame(0.5, { 16, 32 }, { 31, 47 });
	m_animator.addAnimation("3"s, std::move(animation));

	m_animator.play();
}

void TestApp::update(float dt) noexcept
{
	m_sprites[2].move(cg::Vec2f{ speed * dt, 0.f });

	static auto frame = 0;
	frame++;
	static auto time = 0.f;
	time += dt;

	std::wstring title = L"Frame time: "s + std::to_wstring(dt) +
		L", FPS: "s + std::to_wstring(1.f / dt) +
		L" Avg FPS: "s + std::to_wstring(frame / time) +
		L" Frame ¹: "s + std::to_wstring(frame) +
		L" Elapsed time: "s + std::to_wstring(time);

	m_console.setTitle(title);

	if (frame % 100 == 0 && frame != 0)
		m_animator.playAnimation("2"s);
	if(frame > 300)
		m_animator.playAnimation("1"s);

	m_animator.update(m_sprites[1], dt);
	
}

void TestApp::processEvent(const cg::Event& e) noexcept
{
	if (e.type == cg::EventType::KeyPressed || e.type == cg::EventType::MouseClick)
		speed = -speed;
	if (e.type == cg::EventType::MouseMove)
		m_sprites[1].setPos({ e.mouseMove.position.X, e.mouseMove.position.Y });
}

void TestApp::draw() noexcept
{
	for (auto sprite : m_sprites)
		m_surface.drawSprite(sprite);
}
