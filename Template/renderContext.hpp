#include <vector>

#include "view.hpp"

namespace graphics {
	template<typename _View>
	struct renderContext{
		typedef _View view_type;
		typedef std::vector<std::pair<int, int> > buffer_type;
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef tvec2<float> point_type;

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

		void scan_convert_triangle(point_type min_y_vert, point_type mid_y_vert, point_type max_y_vert, int handedness) {
			scan_convert_line(min_y_vert, max_y_vert, 0 + handedness);
			scan_convert_line(min_y_vert, mid_y_vert, 1 - handedness);
			scan_convert_line(mid_y_vert, max_y_vert, 1 - handedness);
		}

	private:
		void scan_convert_line(point_type min_y_vert, point_type max_y_vert, int which_side) {
			int y_start = int(round(min_y_vert.y));
			int y_end   = int(round(max_y_vert.y));
			int x_start = int(round(min_y_vert.x));
			int x_end   = int(round(max_y_vert.x));

			int y_dist = y_end - y_start;
			int x_dist = x_end - x_start;

			if (y_dist <= 0) {
				return;
			}

			float x_step = float(x_dist) / float(y_dist);
			float cur_x = float(x_start);

			for (int j = y_start; j < y_end; ++j) {
				if (which_side == 0) {
					m_scan_buffer[j].first = int(round(cur_x));
				}
				else {
					m_scan_buffer[j].second = int(round(cur_x));
				}
				cur_x += x_step;
				
			}
		}

	};


}