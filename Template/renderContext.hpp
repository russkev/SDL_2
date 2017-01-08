#include <vector>
#include <thread>
#include <future>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "math.hpp"
#include "view.hpp"
#include "algorithm.hpp"
#include "edge.hpp"
#include "gradients.hpp"
#include "textures.hpp"

#define USE_MULTITHREADING 0


namespace graphics {
	template<typename _View>
	struct renderContext {
		typedef _View view_type;
		typedef std::vector<std::pair<int, int> > buffer_type;
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;
		typedef xor_texture texture_type;
		typedef vec2 coord_type;

		renderContext(view_type& s_view) :
			m_view(s_view),
			m_scan_buffer(s_view.size().y * 2, { 0, 0 })
		{}

	private:
		view_type& m_view;
		buffer_type m_scan_buffer;



	public:

		void fill_triangle(
			const vertex& p1, 
			const vertex& p2, 
			const vertex& p3,
			const texture_type& s_texture
		) {

			mat4 screen_space_transform = init_screen_space_transform(float(m_view.size().x), float(m_view.size().y));
			// // These coordinates are the coordinates compared to the screen before perspective divide
			auto p1_pos = screen_space_transform*p1.m_pos;
			auto p2_pos = screen_space_transform*p2.m_pos;
			auto p3_pos = screen_space_transform*p3.m_pos;


			// // Assign max, mid and min y vert arbitrarily, they will be sorted in next step
			auto min_y_vert = vertex(perspective_divide(p1_pos), p1.m_coord, p1.m_col);
			auto mid_y_vert = vertex(perspective_divide(p2_pos), p2.m_coord, p2.m_col);
			auto max_y_vert = vertex(perspective_divide(p3_pos), p3.m_coord, p3.m_col);

			// // Sort points so min, mid and max contain the correct values.
			if (max_y_vert.m_pos.y < min_y_vert.m_pos.y) { std::swap(min_y_vert, max_y_vert); }
			if (mid_y_vert.m_pos.y < min_y_vert.m_pos.y) { std::swap(min_y_vert, mid_y_vert); }
			if (mid_y_vert.m_pos.y > max_y_vert.m_pos.y) { std::swap(max_y_vert, mid_y_vert); }

			scan_triangle(min_y_vert, mid_y_vert, max_y_vert, s_texture);

		}

		void scan_square(
			const vertex& tl,
			const vertex& tr,
			const vertex& bl,
			const vertex& br
			) {
			gradients m_gradients(tl, tr, bl);
			edge left (m_gradients, tl, bl, 0);
			edge right(m_gradients, tr, br, 0);

			scan_edges(m_gradients, left, right, left);
		}


	private:
		void scan_triangle(const vertex& min_y_vert, const vertex& mid_y_vert, const vertex& max_y_vert, const texture_type& s_texture) {
#if USE_MULTITHREADING
			const auto s_num_threads = std::thread::hardware_concurrency();
			std::vector<std::future<void>> s_threads;
			s_threads.reserve(s_num_threads);
			
			for (uint i = 0; i < s_num_threads; ++i) {
				s_threads.emplace_back(std::async(std::launch::async, [&, i]() {
					gradients m_gradients (min_y_vert, mid_y_vert, max_y_vert);
					edge top_to_bottom    (m_gradients, min_y_vert, max_y_vert, 0);
					edge top_to_middle    (m_gradients, min_y_vert, mid_y_vert, 0);
					edge middle_to_bottom (m_gradients, mid_y_vert, max_y_vert, 1);

					// // If triangle is left handed, lead with top to middle, else lead with top to bottom
					if (triangle_area(min_y_vert.m_pos, mid_y_vert.m_pos, max_y_vert.m_pos) >= 0) { 
						scan_edges(top_to_middle,     top_to_bottom,     top_to_middle,    s_num_threads, i, s_texture);
						scan_edges(middle_to_bottom,  top_to_bottom,     middle_to_bottom, s_num_threads, i, s_texture);
					} else { 										
						scan_edges(top_to_bottom,     top_to_middle,     top_to_middle,    s_num_threads, i, s_texture);
						scan_edges(top_to_bottom,     middle_to_bottom,  middle_to_bottom, s_num_threads, i, s_texture);
					}
				}));
			}

			for (auto&t : s_threads) t.get();
#else

			gradients m_gradients(min_y_vert, mid_y_vert, max_y_vert);
			edge top_to_bottom   (m_gradients, min_y_vert, max_y_vert, 0);
			edge top_to_middle   (m_gradients, min_y_vert, mid_y_vert, 0);
			edge middle_to_bottom(m_gradients, mid_y_vert, max_y_vert, 1);

			// // If triangle is left handed, lead with top to middle, else lead with top to bottom
			if (triangle_area(min_y_vert.m_pos, mid_y_vert.m_pos, max_y_vert.m_pos) >= 0) {
				scan_edges(top_to_middle,    top_to_bottom,    top_to_middle   , s_texture);
				scan_edges(middle_to_bottom, top_to_bottom,    middle_to_bottom, s_texture); 
			} else {																		
				scan_edges(top_to_bottom,    top_to_middle,    top_to_middle   , s_texture);
				scan_edges(top_to_bottom,    middle_to_bottom, middle_to_bottom, s_texture);
			}
#endif

		}

		void scan_edges(edge& left, edge& right, edge& lead
#if USE_MULTITHREADING
			,unsigned every_nth, unsigned plus_i
#endif
			,const texture_type& s_texture
			)
		{
			for (int j = lead.y_start(); j < lead.y_end(); ++j) {
#if USE_MULTITHREADING
				if (((j - lead.y_start()) % every_nth) == plus_i) {
					draw_scan_line(s_gradients, left, right, j, s_texture);
				}
#else
				draw_scan_line(left, right, j, s_texture);
#endif
				left.step();
				right.step();
			}
		}

		void draw_scan_line(edge& left, edge& right, int j, const texture_type& s_texture) {

			auto x_min  = int(ceil(left.x()));
			auto x_max  = int(ceil(right.x()));
			if (x_min > x_max) std::swap(x_min, x_max);
			
			float x_prestep = x_min - left.x();

			// // TRIAL BIT // //
			float percentage_along_screen_line = 0;
			float percentage_along_3d_line_x = 0;
			float percentage_along_3d_line_y = 0;
			float z = 0;
			coord_type coord_test;
			// // END TRIAL // //
			
			
			// // Work out what the next step for each calculation to reduce errors to do with precision
			coord_type coord_x_step = (right.coord() - left.coord()) / (right.x() - left.x()); 
			// // Calculate start coordinate
			coord_type coord = left.coord_test() + coord_x_step * x_prestep;

			// // USE FOR COLOUR GRADIENT // //
			//vec4 float_color      = vec4(left.col());
			//bgra_color_type color = bgra_color_type(float_color);
			// // END COLOUR GRADIENT     // //
			//auto test_width = s_texture.width();


			for (int i = x_min; i < x_max; ++i) {
				// // TRIAL BIT // //
				percentage_along_screen_line = (i - left.x())/(right.x() - left.x());
				z = lerp(left.z(), right.z(), percentage_along_screen_line);
				percentage_along_3d_line_x = (i*z - left.x()*left.z()) / (right.x()*right.z() - left.x()*left.z());
				percentage_along_3d_line_y = (j*z - j*left.z()) / (j*right.z() - j*left.z());
				coord_test.s = lerp(left.coord().s, right.coord().s, percentage_along_3d_line_x);
				coord_test.t = lerp(left.coord().t, right.coord().t, percentage_along_3d_line_y);

				
				if (coord_test.s > 255 || coord_test.t > 255){//float(s_texture.width())){// || coord_test.t > float(s_texture.height())) {
					__debugbreak();
				}

				// // END TRIAL // //

				// // i is screen x, j is screen y
				m_view[j][i] = s_texture.get_texture(int(coord_test.s), int(coord_test.t));
				//coord += coord_x_step;

				// // USE FOR COLOUR GRADIENT // //
				//float_color += s_gradients.col_x_step();
				//color = bgra_color_type(clamp(float_color, vec4(0,0,0,0), vec4(255,255,255,255)));
				// // END COLOUR GRADIENT     // //
			}
		}
	};


}