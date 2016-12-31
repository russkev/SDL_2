#pragma once

#include <vector>

#include "math.hpp"
#include "vertex.hpp"

namespace graphics {
	struct gradients {
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;
		typedef vec2 coord_type;

	private:
		// // Mmeber variables
		coord_type m_coord_x_step;
		coord_type m_coord_y_step;
		std::vector<coord_type> m_coords_collection;

	public:
		// // Getters
		coord_type coord_x_step() { return m_coord_x_step; }
		coord_type coord_y_step() { return m_coord_y_step; }
		coord_type coord(int index) { return m_coords_collection.at(index); }


		gradients(
			const vertex& min_y_vert,
			const vertex& mid_y_vert,
			const vertex& max_y_vert
		) {

			m_coords_collection.push_back(min_y_vert.m_tex_coords);
			m_coords_collection.push_back(mid_y_vert.m_tex_coords);
			m_coords_collection.push_back(max_y_vert.m_tex_coords);

			// // Interpolation equation
			vec2 d_coord_x =
				(vec2(mid_y_vert.m_tex_coords) - vec2(max_y_vert.m_tex_coords))*
				(min_y_vert.m_pos.y - max_y_vert.m_pos.y);
			d_coord_x -=
				(vec2(min_y_vert.m_tex_coords) - vec2(max_y_vert.m_tex_coords))*
				(mid_y_vert.m_pos.y - max_y_vert.m_pos.y);


			vec2 d_coord_y = (vec2(mid_y_vert.m_tex_coords) - vec2(max_y_vert.m_tex_coords))*(min_y_vert.m_pos.x - max_y_vert.m_pos.x);
			d_coord_y     -= (vec2(min_y_vert.m_tex_coords) - vec2(max_y_vert.m_tex_coords))*(mid_y_vert.m_pos.x - max_y_vert.m_pos.x);

			float one_over_dx = (mid_y_vert.m_pos.x - max_y_vert.m_pos.x);
			one_over_dx *= (min_y_vert.m_pos.y - max_y_vert.m_pos.y);
			one_over_dx -= (min_y_vert.m_pos.x - max_y_vert.m_pos.x) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y);
			//one_over_dx        = 1/(one_over_dx);

			float one_over_dy = -one_over_dx;



			m_coord_x_step = d_coord_x / one_over_dx;
			m_coord_y_step = d_coord_y / one_over_dy;
		}

		
	};


}