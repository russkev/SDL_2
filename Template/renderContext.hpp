#include <vector>
#include <thread>
#include <future>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "math.hpp"
#include "view.hpp"
#include "algorithm.hpp"
#include "edge.hpp"


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
		void fill_shape(int y_min, int y_max) {
			for (int j = y_min; j < y_max; ++j) {
				if (j < 0) { continue; }
				const auto& s_line = m_scan_buffer[j];
				for (int i = s_line.first; i < s_line.second; ++i) {
					if (i < 0) i = 0;
					blend_element(m_view, tvec2<int>(i, j), bgra_color_type(0, 0, 255, 255));
				}
			}
		}

		void fill_triangle(const point_type& p1, const point_type& p2, const point_type& p3) {

			mat4 screen_space_transform = init_screen_space_transform(float(m_view.size().x), float(m_view.size().y));

			auto min_y_vert = (screen_space_transform*p1) / p1.w;
			auto mid_y_vert = (screen_space_transform*p2) / p2.w;
			auto max_y_vert = (screen_space_transform*p3) / p3.w;

			// // Sort points so min, mid and max contain the correct values.
			if (max_y_vert.y < min_y_vert.y) {
				auto temp = min_y_vert;
				min_y_vert = max_y_vert;
				max_y_vert = temp;
			}

			if (mid_y_vert.y < min_y_vert.y) {
				auto temp = min_y_vert;
				min_y_vert = mid_y_vert;
				mid_y_vert = temp;
			}

			if (mid_y_vert.y > max_y_vert.y) {
				auto temp = max_y_vert;
				max_y_vert = mid_y_vert;
				mid_y_vert = temp;
			}

			// // Initialize thread parameters
			//auto s_num_threads = std::thread::hardware_concurrency();
			//auto s_length = int(max_y_vert.y) - int(min_y_vert.y);
			//s_length = (s_length + s_num_threads - 1) / s_num_threads;
			//std::vector<std::thread> s_threads;
			//s_threads.reserve(s_num_threads);

			// // s_bound_fill is a pointer to a function that basically calls the fill_shape function
			// // auto... args is a list of parameters
			// // args... is filling the function with the parameters above.
			//auto s_bound_fill = [this](auto... args) { // args is a list of variables
			//	fill_shape(args...); 
			//};

			auto s_start = int(ceil(min_y_vert.y));


			// // If area of triangle is negative, handedness is 0. // //
			//int handedness = (triangle_area(min_y_vert, mid_y_vert, max_y_vert) >= 0 ? 1 : 0);
			scan_triangle(min_y_vert, mid_y_vert, max_y_vert, triangle_area(min_y_vert, mid_y_vert, max_y_vert) >= 0);

			//// // Do multi threaded fill shape
			//for (auto i = 0; i < s_num_threads; ++i) {
			//	s_threads.emplace_back(std::thread(s_bound_fill, s_start, min(s_start + s_length, int(ceil(max_y_vert.y)))));
			//	s_start += s_length;
			//}
			//// // Wait for all threads to finish
			//for (auto & s_thread: s_threads) {
			//	s_thread.join();
			//}
		}

	private:
		void scan_triangle(const vec4& min_y_vert, const vec4& mid_y_vert, const vec4& max_y_vert, bool handedness) {

			edge top_to_bottom(min_y_vert, max_y_vert);

			edge top_to_middle(min_y_vert, mid_y_vert);
			edge middle_to_bottom(mid_y_vert, max_y_vert);

			scan_edges(top_to_bottom, top_to_middle, handedness);
			scan_edges(top_to_bottom, middle_to_bottom, handedness);

		}

		void scan_edges(edge& a, edge& b, bool handedness) {

			auto* left = &a;
			auto* right = &b;
			if (handedness) {
				std::swap(left, right);
			}

			for (int j = b.m_y_start; j < b.m_y_end; ++j) {
				draw_scan_line(*left, *right, j);
				left->step();
				right->step();
			}
		}

		void draw_scan_line(edge& left, edge& right, int j) {
			//if (j < 0) { continue; }

			auto a = int(ceil(left.m_x));
			auto b = int(ceil(right.m_x));
			if (a > b) std::swap(a, b);

			for (int i = a; i < b; ++i) {
				//if (i < 0) i = 0;
				if (i >= m_view.size().x || i < 0 || j >= m_view.size().y || j < 0)
					continue;
				blend_element(m_view, tvec2<int>(i, j), bgra_color_type(0, 0, 255, 255));
			}
		}
		//void scan_convert_triangle(const vec4& min_y_vert, const vec4& mid_y_vert, const vec4& max_y_vert, int handedness) {
		//	scan_convert_line(min_y_vert, max_y_vert, 0 + handedness);
		//	scan_convert_line(min_y_vert, mid_y_vert, 1 - handedness);
		//	scan_convert_line(mid_y_vert, max_y_vert, 1 - handedness);
		//}

		//void scan_convert_line(const vec4& min_y_vert, const vec4& max_y_vert, int which_side) {

		//	// // Work out the pixels to start and finish the line
		//	const auto y_start = int(ceil(min_y_vert.y));
		//	const auto y_end   = int(ceil(max_y_vert.y));
		//	const auto x_start = int(ceil(min_y_vert.x));
		//	const auto x_end   = int(ceil(max_y_vert.x));

		//	const auto y_dist = max_y_vert.y - min_y_vert.y;
		//	const auto x_dist = max_y_vert.x - min_y_vert.x;

		//	if (y_dist <= 0) {
		//		return;
		//	}

		//	// // Use the slope for x step
		//	auto x_step = float(x_dist) / float(y_dist);
		//	// // Calculate difference between point on line and the middle of the start pixel
		//	auto y_prestep = y_start - min_y_vert.y;
		//	auto cur_x = min_y_vert.x + y_prestep * x_step;

		//	// // Convert line to pixel numbers
		//	batch_line(y_start, y_end, cur_x, x_step, which_side);

		//}

		//void batch_line(int y_start, int y_end, float cur_x, const float x_step, int which_side) {
		//	for (int j = y_start; j < y_end; ++j) {
		//		if (j < 0) continue;
		//		(which_side == 0 ? m_scan_buffer[j].first = int(round(cur_x)) : m_scan_buffer[j].second = int(ceil(cur_x)));
		//		cur_x += x_step;
		//	}
		//}

	};


}