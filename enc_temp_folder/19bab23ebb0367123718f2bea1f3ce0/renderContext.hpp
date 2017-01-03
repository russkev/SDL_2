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
		typedef vec2 coord_type;
		typedef std::vector<std::vector<bgra_color_type> > texture_type;

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
			xor_texture& texture
		) {

			mat4 screen_space_transform = init_screen_space_transform(float(m_view.size().x), float(m_view.size().y));

			// // Assign max, mid and min y vert arbitrarily, they will be sorted in next step
			auto min_y_vert = vertex(point_type(screen_space_transform*p1.m_pos) / point_type(p1.m_pos.w), p1.m_tex_coords);
			auto mid_y_vert = vertex(point_type(screen_space_transform*p2.m_pos) / point_type(p2.m_pos.w), p2.m_tex_coords);
			auto max_y_vert = vertex(point_type(screen_space_transform*p3.m_pos) / point_type(p3.m_pos.w), p3.m_tex_coords);

			// // Sort points so min, mid and max contain the correct values.
			if (max_y_vert.m_pos.y < min_y_vert.m_pos.y) { std::swap(min_y_vert, max_y_vert); }
			if (mid_y_vert.m_pos.y < min_y_vert.m_pos.y) { std::swap(min_y_vert, mid_y_vert); }
			if (mid_y_vert.m_pos.y > max_y_vert.m_pos.y) { std::swap(max_y_vert, mid_y_vert); }

			scan_triangle(min_y_vert, mid_y_vert, max_y_vert, texture);

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

		void draw_test_texture() {
			xor_texture test_xor_texture(256, 256);
			for (auto x = 0; x < 256; ++x) {
				for (auto y = 0; y < 256; ++y) {
					m_view[x][y] = test_xor_texture.get_texture(x, y);
				}
			}
		}


	private:
		void scan_triangle(const vertex& min_y_vert, const vertex& mid_y_vert, const vertex& max_y_vert, xor_texture& texture) {
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
					if (triangle_area(min_y_vert.m_pos, mid_y_vert.m_pos, max_y_vert.m_pos) >= 0) { 
						scan_edges(m_gradients, top_to_middle,     top_to_bottom,     top_to_middle,    texture, s_num_threads, i);
						scan_edges(m_gradients, middle_to_bottom,  top_to_bottom,     middle_to_bottom, texture, s_num_threads, i);
					}			   																						
					else { // // If triangle is right handed															
						scan_edges(m_gradients, top_to_bottom,     top_to_middle,     top_to_middle,    texture, s_num_threads, i);
						scan_edges(m_gradients, top_to_bottom,     middle_to_bottom,  middle_to_bottom, texture, s_num_threads, i);
					}
				}));
			}

			for (auto&t : s_threads) t.get();
#else

			gradients m_gradients(min_y_vert, mid_y_vert, max_y_vert);
			edge top_to_bottom   (m_gradients, min_y_vert, max_y_vert, 0);
			edge top_to_middle   (m_gradients, min_y_vert, mid_y_vert, 0);
			edge middle_to_bottom(m_gradients, mid_y_vert, max_y_vert, 1);

			if (triangle_area(min_y_vert.m_pos, mid_y_vert.m_pos, max_y_vert.m_pos) >= 0) { // // If triangle is left handed
				scan_edges(m_gradients, top_to_middle,    top_to_bottom,    top_to_middle,    texture);
				scan_edges(m_gradients, middle_to_bottom, top_to_bottom,    middle_to_bottom, texture);
			} else {                      // // If triangle is right handed					  
				scan_edges(m_gradients, top_to_bottom,    top_to_middle,    top_to_middle,    texture);
				scan_edges(m_gradients, top_to_bottom,    middle_to_bottom, middle_to_bottom, texture);
			}
#endif

		}

		void scan_edges(gradients& s_gradients, edge& left, edge& right, edge& lead, xor_texture& texture
#if USE_MULTITHREADING
			,unsigned every_nth, unsigned plus_i
#endif
			)
		{
			// // Edge a is the left side, edge b is the right side and lead is the edge used
			// // to decide the start and end points of the scanning

			//auto* left = &a;
			//auto* right = &b;
			//if (handedness) {
			//	std::swap(left, right);
			//}

			for (int j = lead.y_start(); j < lead.y_end(); ++j) {
#if USE_MULTITHREADING
				if (((j - lead.y_start()) % every_nth) == plus_i) {
					draw_scan_line(s_gradients, left, right, j);
				}
#else
				draw_scan_line(s_gradients, left, right, j, texture);
#endif
				left.step();
				right.step();
			}
		}

		void draw_scan_line(gradients& s_gradients, edge& left, edge& right, int j, xor_texture& texture) {

			auto x_min  = int(ceil(left.x()));
			auto x_max  = int(ceil(right.x()));
			if (x_min > x_max) std::swap(x_min, x_max);
			
			float x_prestep       = x_min-left.x();

			//float x_dist                = right.x() - left.x(); // // Screen distance between left and right pixels
			//coord_type tex_coord_x_step = (right.coord() - left.coord()) / x_dist; // // Step by this amount along texture coordinates to get from left to right side
			// // tex_coord = coordinate of the left side of texture + adjustment for prestep.
			coord_type tex_coord = left.coord() + s_gradients.coord_x_step() * x_prestep;

			for (int i = x_min; i < x_max; ++i) {
				//if (i >= m_view.size().x || i < 0 || j >= m_view.size().y || j < 0)
				//	continue;
				//blend_element(m_view, tvec2<int>(i, j), bgra_color_type(0, 0, 255, 255)); // Solid colour so blend element not needed


				// // !!! THIS IS THE NEXT STEP !!! // //
				// // Work out how to do texture coordinates // //
				//ivec2 src = (
				//	int(tex_coord.x * (texture.width() - 1) + 0.5f),
				//	int(tex_coord.y * (texture.height() - 1) + 0.5f)
				//	);


				m_view[j][i] = texture.get_texture(tex_coord.x, tex_coord.y);

				tex_coord += s_gradients.coord_x_step();
				//float_color += s_gradients.col_x_step();
				//color = bgra_color_type(clamp(float_color, vec4(0,0,0,0), vec4(255,255,255,255)));
			}
		}
	};


}