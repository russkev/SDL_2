#include <sdl.h>
#include <cstdio>
#include <cstdlib>
#include <numeric>
#include <array>

namespace graphics {
	struct fps {

	private:
		SDL_Window* m_window = nullptr;
		double m_time_passed = 0.0f;
		double m_running_average = 60.0f;
		std::uint64_t m_frames_passed = 0u;

		double m_update_interval = 1.0 / 8.0;
		double m_alpha = 0.01;

	public:
		fps(SDL_Window* w, double update_interval = 1.0 / 8.0, double alpha = 0.01) :
			m_window(w), m_update_interval(update_interval), m_alpha(alpha)
		{};


		void think(double s_delta_time) {
			static thread_local char s_str_buffer[2048];

			m_time_passed += s_delta_time;
			++m_frames_passed;



			if (m_time_passed >= m_update_interval) {
				const auto s_fps = m_frames_passed / m_time_passed;
				m_time_passed = 0.0f;
				m_frames_passed = 0u;

				m_running_average = m_running_average * (1.0 - m_alpha) + s_fps * m_alpha;

				sprintf_s(s_str_buffer, "FPS : %0.2f, Running Average : %0.2f\n", float(s_fps), float(m_running_average));
				SDL_SetWindowTitle(m_window, s_str_buffer);
			}
		}

	};
}