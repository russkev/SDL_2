#include <vector>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "math.hpp"
#include "view.hpp"
#include "algorithm.hpp"


namespace graphics {
	template<typename _View>
	struct renderContext{
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
		void draw_scan_buffer(int y_coord, int x_min, int x_max) {
			m_scan_buffer[y_coord] = { x_min, x_max };
		}

		void fill_shape(int y_min, int y_max) {
			for (int j = y_min; j < y_max; ++j) {
				const auto& s_line = m_scan_buffer[j];
				for (int i = s_line.first; i < s_line.second; ++i) {
					blend_element(m_view, tvec2<int>(i, j), bgra_color_type(0, 0, 255, 255));
				}
			}
		}

		void fill_triangle(const point_type& p1, const point_type& p2, const point_type& p3) {

			mat4 screen_space_transform = init_screen_space_transform(float( m_view.size().x), float(m_view.size().y));

			auto min_y_vert = p1*screen_space_transform / p1.w;
			auto mid_y_vert = p2*screen_space_transform / p2.w;
			auto max_y_vert = p3*screen_space_transform / p3.w;
			
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

			// // If area of triangle is negative, handedness is 0. // //
			int handedness = (triangle_area(min_y_vert, mid_y_vert, max_y_vert) >= 0 ? 1 : 0);
			scan_convert_triangle(min_y_vert, mid_y_vert, max_y_vert, handedness);
			fill_shape(int(min_y_vert.y), int(max_y_vert.y));
		}

		void scan_convert_triangle(const vec4& min_y_vert, const vec4& mid_y_vert, const vec4& max_y_vert, int handedness) {
			scan_convert_line(min_y_vert, max_y_vert, 0 + handedness);
			scan_convert_line(min_y_vert, mid_y_vert, 1 - handedness);
			scan_convert_line(mid_y_vert, max_y_vert, 1 - handedness);
		}


	private:
		void scan_convert_line(const vec4& min_y_vert, const vec4& max_y_vert, int which_side) {
			const auto y_start = int(round(min_y_vert.y));
			const auto y_end   = int(round(max_y_vert.y));
			const auto x_start = int(round(min_y_vert.x));
			const auto x_end   = int(round(max_y_vert.x));

			const auto y_dist = y_end - y_start;
			const auto x_dist = x_end - x_start;

			if (y_dist <= 0) {
				return;
			}

			const auto x_step = float(x_dist) / float(y_dist);
			auto cur_x = float(x_start);

			for (int j = y_start; j < y_end; ++j) {
				(which_side == 0 ? m_scan_buffer[j].first = int(round(cur_x)) : m_scan_buffer[j].second = int(round(cur_x)));
				cur_x += x_step;
				
			}
		}

	};


}