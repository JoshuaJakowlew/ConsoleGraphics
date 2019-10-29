#include <iostream>
#include <string>
#include <chrono>

#include "cg.h"

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

	cg::Texture bgTex{ "assets/bg.bmp", palette };
	cg::Texture marioTex{ "assets/sprite.bmp", palette };

	cg::Sprite bg{ bgTex };
	cg::Sprite sprite{ marioTex };
	cg::Sprite sprite1{ marioTex, { 0, 0 }, { 15, 7 } };

	float speed = 25.f;
};

int main()
{
	using namespace std;

	TestApp app{ { 200, 100 }, { 4, 4 } };
	app.start();
}

void TestApp::setup() noexcept
{
	m_console.setPalette(palette);

	bg.setOrigin({ 0, 0 });

	sprite.setPos({ 25, 25 });
	sprite.setTransparent(true);

	sprite1.setPos({ 25, 25 });
	sprite1.setTransparent(true);
}

void TestApp::update(float dt) noexcept
{
	sprite1.move(cg::Vec2f{ speed * dt, 0.f });
}

void TestApp::processEvent(const cg::Event& e) noexcept
{
	if (e.type == cg::EventType::KeyPressed || e.type == cg::EventType::MouseClick)
		speed = -speed;
	if (e.type == cg::EventType::MouseMove)
		sprite.setPos({ e.mouseMove.position.X, e.mouseMove.position.Y });
}

void TestApp::draw() noexcept
{
	m_surface.drawSprite(bg);
	m_surface.drawSprite(sprite1);
	m_surface.drawSprite(sprite);
}
