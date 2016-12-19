#include <vector>

#include "math.hpp"
#include "vertex.hpp"

namespace graphics{
	struct gradients {
		typedef u8vec4 bgra_color_type;

	private:
		vec4 m_col_x_step;
		vec4 m_col_y_step;
		std::vector<u8vec4> m_color;

	public:
		gradients(const vertex& min_y_vert, const vertex& mid_y_vert, const vertex& max_y_vert) {

			m_color.push_back(min_y_vert.m_color);
			m_color.push_back(mid_y_vert.m_color);
			m_color.push_back(max_y_vert.m_color);

			float one_over_dx = 1.0f /
				(mid_y_vert.m_pos.x - max_y_vert.m_pos.x) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y) -
				(min_y_vert.m_pos.x - max_y_vert.m_pos.x) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y);
			float one_over_dy = -one_over_dx;

			vec4 d_color = (
				vec4(mid_y_vert.m_color - max_y_vert.m_color) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y) -
				vec4(min_y_vert.m_color - max_y_vert.m_color) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y)
				);

			m_col_x_step = d_color * one_over_dx;
			m_col_y_step = d_color * one_over_dy;
		}

		vec4 col_x_step() { return m_col_x_step; };
		vec4 col_y_step() { return m_col_y_step; };
		u8vec4 color(int index) { return m_color.at(index); }
	};
}