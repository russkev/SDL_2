#include <vector>

#include "math.hpp"
#include "vertex.hpp"

namespace graphics {
	struct gradients {
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;

	private:
		// // Mmeber variables
		point_type m_col_x_step;
		point_type m_col_y_step;
		std::vector<bgra_color_type> m_colors;

	public:
		gradients(
			const vertex& min_y_vert,
			const vertex& mid_y_vert,
			const vertex& max_y_vert
		){

			m_colors.push_back(min_y_vert.m_col);
			m_colors.push_back(mid_y_vert.m_col);
			m_colors.push_back(max_y_vert.m_col);

			// // Interpolation equation
			float one_over_dx = 1.0f /
				(mid_y_vert.m_pos.x - max_y_vert.m_pos.x) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y) -
				(min_y_vert.m_pos.x - max_y_vert.m_pos.x) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y);

			float one_over_dy = -one_over_dx;

			vec4 d_color = (
				vec4(mid_y_vert.m_col - max_y_vert.m_col) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y) -
				vec4(min_y_vert.m_col - max_y_vert.m_col) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y)
				);

			m_col_x_step = d_color * one_over_dx;
			m_col_y_step = d_color * one_over_dy;
		}

		// // Getters
		vec4 col_x_step() { return m_col_x_step; }
		vec4 col_y_step() { return m_col_y_step; }
		bgra_color_type color(int index) { return m_colors.at(index); }
	};


}