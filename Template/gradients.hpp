#pragma once

#include <vector>

#include "math.hpp"
#include "vertex.hpp"
#include "textures.hpp"

namespace graphics {
	struct gradients {
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;
		typedef vec2 coord_type;

	private:
		// // Member variables
		point_type m_col_x_step;
		point_type m_col_y_step;
		coord_type m_coord_x_step;
		coord_type m_coord_y_step;
		std::vector<bgra_color_type> m_colors;
		std::vector<coord_type> m_coords;



	public:
		// // Getters
		point_type col_x_step()   { return m_col_x_step; }
		point_type col_y_step()   { return m_col_y_step; }
		coord_type coord_x_step() { return m_coord_x_step; }
		coord_type coord_y_step() { return m_coord_y_step; }
		bgra_color_type color(int index) { return m_colors.at(index); }
		coord_type      coord(int index) { return m_coords.at(index); }

		// // Constructor
		gradients(
			const vertex& min_y_vert,
			const vertex& mid_y_vert,
			const vertex& max_y_vert
		) {
			// // Constructor variables
			point_type	d_color_x;
			point_type	d_color_y;
			coord_type	d_coord_x;
			coord_type	d_coord_y;
			float		one_over_dx;
			float		one_over_dy;

			// // Make vectors
			m_colors.push_back(min_y_vert.m_col);
			m_colors.push_back(mid_y_vert.m_col);
			m_colors.push_back(max_y_vert.m_col);

			m_coords.push_back(min_y_vert.m_coord);
			m_coords.push_back(mid_y_vert.m_coord);
			m_coords.push_back(max_y_vert.m_coord);


			// // Interpolation equation
			d_color_x =
				(vec4(mid_y_vert.m_col) - vec4(max_y_vert.m_col))*(min_y_vert.m_pos.y - max_y_vert.m_pos.y) - 
				(vec4(min_y_vert.m_col) - vec4(max_y_vert.m_col))*(mid_y_vert.m_pos.y - max_y_vert.m_pos.y);
			d_color_y = 
				(vec4(mid_y_vert.m_col) - vec4(max_y_vert.m_col))*(min_y_vert.m_pos.x - max_y_vert.m_pos.x) -
				(vec4(min_y_vert.m_col) - vec4(max_y_vert.m_col))*(mid_y_vert.m_pos.x - max_y_vert.m_pos.x);

			d_coord_x =
				(vec2(mid_y_vert.m_coord) - vec2(max_y_vert.m_coord))*(min_y_vert.m_pos.y - max_y_vert.m_pos.y) -
				(vec2(min_y_vert.m_coord) - vec2(max_y_vert.m_coord))*(mid_y_vert.m_pos.y - max_y_vert.m_pos.y);
			d_coord_y =			   							  
				(vec2(mid_y_vert.m_coord) - vec2(max_y_vert.m_coord))*(min_y_vert.m_pos.x - max_y_vert.m_pos.x) -
				(vec2(min_y_vert.m_coord) - vec2(max_y_vert.m_coord))*(mid_y_vert.m_pos.x - max_y_vert.m_pos.x);

			one_over_dx =
				((mid_y_vert.m_pos.x - max_y_vert.m_pos.x) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y)) - 
				((min_y_vert.m_pos.x - max_y_vert.m_pos.x) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y));
			one_over_dy = -one_over_dx;

			m_col_x_step = d_color_x / one_over_dx;
			m_col_y_step = d_color_y / one_over_dy;

			m_coord_x_step = d_coord_x / one_over_dx;
			m_coord_y_step = d_coord_y / one_over_dy;
		}

		
	};


}