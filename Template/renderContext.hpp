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

#define USE_MULTITHREADING 0


namespace graphics {
	template<typename _View>
	struct renderContext {
		typedef _View view_type;
		typedef std::vector<std::pair<int, int> > buffer_type;
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;

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
			const vertex& p3
		) {

			mat4 screen_space_transform = init_screen_space_transform(float(m_view.size().x), float(m_view.size().y));

			// // Assign max, mid and min y vert arbitrarily, they will be sorted in next step
			auto min_y_vert = vertex(point_type(screen_space_transform*p1.m_pos) / point_type(p1.m_pos.w), p1.m_col);
			auto mid_y_vert = vertex(point_type(screen_space_transform*p2.m_pos) / point_type(p2.m_pos.w), p2.m_col);
			auto max_y_vert = vertex(point_type(screen_space_transform*p3.m_pos) / point_type(p3.m_pos.w), p3.m_col);

			// // Sort points so min, mid and max contain the correct values.
			if (max_y_vert.m_pos.y < min_y_vert.m_pos.y) { std::swap(min_y_vert, max_y_vert); }
			if (mid_y_vert.m_pos.y < min_y_vert.m_pos.y) { std::swap(min_y_vert, mid_y_vert); }
			if (mid_y_vert.m_pos.y > max_y_vert.m_pos.y) { std::swap(max_y_vert, mid_y_vert); }

			scan_triangle(min_y_vert, mid_y_vert, max_y_vert);

		}

	private:
		void scan_triangle(const vertex& min_y_vert, const vertex& mid_y_vert, const vertex& max_y_vert) {
#if USE_MULTITHREADING
			const auto s_num_threads = std::thread::hardware_concurrency();
			std::vector<std::future<void>> s_threads;
			s_threads.reserve(s_num_threads);
			
			for (auto i = 0; i < s_num_threads; ++i) {
				s_threads.emplace_back(std::async(std::launch::async, [&, i]() {
					edge top_to_bottom    (min_y_vert, max_y_vert);
					edge top_to_middle    (min_y_vert, mid_y_vert);
					edge middle_to_bottom (mid_y_vert, max_y_vert);
					if (triangle_area(min_y_vert, mid_y_vert, max_y_vert) >= 0) { 
						scan_edges(top_to_middle,     top_to_bottom,     top_to_middle,    s_num_threads, i);
						scan_edges(middle_to_bottom,  top_to_bottom,     middle_to_bottom, s_num_threads, i);
					}
					else { // // If triangle is right handed
						scan_edges(top_to_bottom,     top_to_middle,     top_to_middle,    s_num_threads, i);
						scan_edges(top_to_bottom,     middle_to_bottom,  middle_to_bottom, s_num_threads, i);
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
				                                                         // // First part of the triangle, top to the middle point. 1st arg is left side, 2nd is right side
				scan_edges(m_gradients, top_to_middle,    top_to_bottom,    top_to_middle);
				                                                        // // Same as above but for middle part down to bottom
				scan_edges(m_gradients, middle_to_bottom, top_to_bottom,    middle_to_bottom);
			} else {                                               // // If triangle is right handed
				scan_edges(m_gradients, top_to_bottom,    top_to_middle,    top_to_middle);
				scan_edges(m_gradients, top_to_bottom,    middle_to_bottom, middle_to_bottom);
			}
#endif

		}

		void scan_edges(gradients& s_gradients, edge& left, edge& right, const edge& lead
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

			for (int j = lead.m_y_start; j < lead.m_y_end; ++j) {
#if USE_MULTITHREADING
				if (((j - lead.m_y_start) % every_nth) == plus_i) {
					draw_scan_line(left, right, j);
				}
#else
				draw_scan_line(s_gradients, left, right, j);
#endif
				left.step();
				right.step();
			}
		}

		void draw_scan_line(gradients& s_gradients, edge& left, edge& right, int j) {
			//if (j < 0) { continue; }

			auto x_min  = int(ceil(left.m_x));
			auto x_max  = int(ceil(right.m_x));
			if (x_min > x_max) std::swap(x_min, x_max);
			
			auto x_prestep = x_min-left.x();
			vec4 float_color = vec4(left.col()) + vec4(s_gradients.col_x_step())*x_prestep;
			bgra_color_type color = bgra_color_type(float_color);



			for (int i = x_min; i < x_max; ++i) {
				//if (i < 0) i = 0;
				if (i >= m_view.size().x || i < 0 || j >= m_view.size().y || j < 0)
					continue;
				//blend_element(m_view, tvec2<int>(i, j), bgra_color_type(0, 0, 255, 255)); // Solid colour so blend element not needed
				m_view[j][i] = color;
				color += s_gradients.col_x_step();
			}
		}
	};


}