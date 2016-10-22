#include <vector>
#include <random>
#include <ctime>
#include <math.h>

#include "algorithm.hpp"
#include "tvec.hpp"
#include "canvas.hpp"
#include "view.hpp"


namespace graphics {
	struct starfield {

		starfield (int numStars, float speed, float spread, float fog_dist, float fov):
		m_speed(speed), m_spread(spread), m_fog_dist(fog_dist), m_tan_half_fov(tan(fov*0.5f*0.0174532f)){//m_tan_half_fov(tan((fov*0.5f)*0.0174532f)){ //pi/180;
			m_star.reserve(numStars);
			for (int i = 0; i < numStars; ++i) {
				m_star.push_back(init_start(i));
			}
		}
		
		template<typename _View>
		void render_stars(_View& s_view, double s_delta_time);

	private:
		float m_spread, m_speed, m_fog_dist, m_tan_half_fov;
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
			(distribution(generator)+1.000001)*0.5*m_spread);	//z start posotion

		return star;
	}

	template<typename _View>
	void starfield::render_stars(_View& s_view, double s_delta_time) {
		int half_width = s_view.size().x / 2;
		int half_height = s_view.size().y / 2;
		int count = 0;
		float alpha = 255, r = 255, g = 0, alpha_multiplier = alpha/m_fog_dist;
		
		tvec3<float> test_front_element = m_star.front();
		float test_delta_time_times_speed = s_delta_time*m_speed;
		std::cout << std::endl;
		for (auto& star : m_star) {
			++count;
			// Move star towards camera which is at 0 on 
			star.z -= s_delta_time * m_speed;


			if (star.z < 0) { //If star is behind camera, make new star
				star = init_start(count);
			}

			// // Render the star

			// // Multiplying the position by (size/2) and then adding (size/2)
			// // remaps the positions from range (-1, 1) to (0, size)
			float it_x = (star.x/(star.z*m_tan_half_fov)) * half_width + half_width;
			float it_y = (star.y/(star.z*m_tan_half_fov)) * half_height + half_height;

			// // If position is not within range of screen, generate new position;
			if (it_x < 0 || it_x > s_view.size().x-1 ||   //1024
				(it_y < 0 || it_y > s_view.size().y -1)) {   //768

				star = init_start(count);
			}
			else {
				// // It is safe to draw star on screen
				alpha = clamp(255 - star.z*alpha_multiplier, 0, 255);// *0.00392156862f); // * 1/255
				r     = clamp(star.z*alpha_multiplier, 0, 255);
				g     = clamp(255 - star.z*alpha_multiplier, 0, 90);

				blend_element(s_view, tvec2<int>(round(it_x), round(it_y)), tvec4<std::uint8_t>(0, g, r, alpha));
			}
		}
	}
}