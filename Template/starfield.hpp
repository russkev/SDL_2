#include <vector>
#include <random>
#include <ctime>

#include "algorithm.hpp"
#include "tvec.hpp"
#include "canvas.hpp"
#include "view.hpp"


namespace graphics {
	struct starfield {

		starfield (int numStars, float speed, float spread):
		m_speed(speed), m_spread(spread){
			for (int i = 0; i < numStars; ++i) {
				m_star.push_back(init_start(i));
			}
		}
		
		template<typename _View>
		void render_stars(SDL_Surface& s_surface, _View& s_view, double s_delta_time);

	private:
		float m_spread, m_speed;
		tvec3<float> init_start(int seed);
	public:
		std::vector<tvec3<float> > m_star;
	};

	tvec3<float> starfield::init_start (int seed) {
		std::mt19937 generator(seed);
		std::uniform_real_distribution<float> distribution(-1.0, 1.0);
		
		tvec3<float> star(
			distribution(generator) * m_spread,					//x start position
			distribution(generator) * m_spread,					//y start position
			(distribution(generator)+1.000001f)*0.5*m_spread);	//z start posotion

		return star;
	}

	template<typename _View>
	void starfield::render_stars(SDL_Surface& s_surface, _View& s_view, double s_delta_time) {
		int half_width = s_surface.w / 2;
		int half_height = s_surface.h / 2;
		int count = 0;
		
		tvec3<float> test_front_element = m_star.front();
		float test_delta_time_times_speed = s_delta_time*m_speed;
		std::cout << std::endl;
		for (auto it = m_star.begin(); it != m_star.end(); ++it) {
			++count;
			// Move star towards camera which is at 0 on 
			it->z -= s_delta_time * m_speed;


			if (it->z < 0) { //If star is behind camera, make new star
				*it = init_start(count);
			}

			// // Render the star

			// // Multiplying the position by (size/2) and then adding (size/2)
			// // remaps the positions from range (-1, 1) to (0, size)
			float it_x = (it->x/it->z) * half_width + half_width;
			float it_y = (it->y/it->z) * half_height + half_width;

			// // If position is not within range of screen, generate new position;
			if (it_x < 0 || it_x > s_surface.w-1 ||   //1024
				(it_y < 0 || it_y > s_surface.h-1)) {   //768

				*it = init_start(count);
				//*it = tvec3<float>(0, 0, 0);
				it->x;
			}
			else {
				// // It is safe to draw star on screen
				blend_element(s_view, tvec2<int>(round(it_x), round(it_y)), tvec4<std::uint8_t>(0, 0, 255, 255));
			}
		}
	}
}