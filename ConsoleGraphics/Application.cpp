#include "Application.h"

namespace cg
{
	Application::Application(Vec2u resolution, Vec2u fontSize) :
		m_console{ std::move(resolution), std::move(fontSize) },
		m_surface{ std::move(resolution) }
	{
		if (m_console.isOpen())
		{
			m_eventManager.setHandle(m_console.getInputHandle());
		}
	}

	void Application::start()
	{
		setup();

		Clock clock;

		while (m_console.isOpen())
		{
			// Process all the events
			Event e;
			while (m_eventManager.pollEvent(e))
				processEvent(e);

			// Update everything with elapsed time
			update(clock.restart());

			// Clear surface from previous frame
			m_surface.fill(CHAR_INFO{ L' ', 0x00 });
			// Draw all the stuff
			draw();
			// Display frame
			bool _ = m_console.display(m_surface.getBuffer());
		}
	}

} // namespace cg
