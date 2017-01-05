#include <sdl.h>
#include <vector>



namespace graphics {
	struct fps {
	private:
		// // Variable devcleration:
		SDL_Window* m_window = nullptr;
		double m_time_passed = 0.0;
		double m_running_average = 60.0;
		double m_max = 0.0;
		double m_min = 1000000.0;
		std::uint64_t m_frames_passed = 0u;
		double m_update_interval = 1.0 / 8.0;
		double m_alpha = 0.01;
		double m_total_time_passed = 0.0;
		std::uint64_t m_total_frames_passed = 0u;

	public:
		// // Initialisation
		fps(SDL_Window*w, double update_interval = 1.0/8.0, double alpha = 0.01) :
			m_window(w), m_update_interval(update_interval), m_alpha(alpha)
		{};



	public:
		void think(double s_delta_time) {
			// // static character string of 2048 characters
			// // thread_local is to be safe in the case of being called with multiple threads
			static thread_local char s_str_buffer[2048];

			// // Increase m_time_passed by the change in time since last call
			m_time_passed += s_delta_time;
			++m_frames_passed;
			m_total_time_passed += s_delta_time;
			++m_total_frames_passed;

			if (m_time_passed >= m_update_interval) { // // If the time interval is long enough, do new calculation of fps
				// // Do fps calculation
				const auto s_fps = m_frames_passed / m_time_passed;
				// // Reset timer and frame count
				m_time_passed = 0.0f;
				m_frames_passed = 0u;

				if (s_fps > m_max) m_max = s_fps;
				if (s_fps < m_min) m_min = s_fps;

				// // Running averag is essentially a low pass filter
				//m_running_average = m_running_average * (1.0 - m_alpha) + s_fps * m_alpha;

				// // Running average as average over total time
				m_running_average = m_total_frames_passed / m_total_time_passed;

				sprintf_s(s_str_buffer, "FPS : %0.2f, Running Average : %0.2f\n, Max : %0.2f, Min : %0.2f", float(s_fps), float(m_running_average), float(m_max), float(m_min));
				SDL_SetWindowTitle(m_window, s_str_buffer);

			}
		}

	};
}