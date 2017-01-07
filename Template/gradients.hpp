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
		// // Member variables
		point_type m_col_x_step;
		point_type m_col_y_step;
		coord_type m_coord_x_step;
		coord_type m_coord_y_step;
		coord_type m_one_over_z_step;
		std::vector<vec4>			 m_colors;
		std::vector<coord_type>      m_coords;
		std::vector<float>           m_one_over_zs;



	public:
		// // Getters (steps)
		point_type col_x_step()      { return m_col_x_step; }
		point_type col_y_step()      { return m_col_y_step; }
		coord_type coord_x_step()    { return m_coord_x_step; }
		coord_type coord_y_step()    { return m_coord_y_step; }
		coord_type one_over_z_step() { return m_one_over_z_step; }
		// // Getters (value in vector at certain index)
		bgra_color_type color(int index) { return bgra_color_type(m_colors.at(index)); }
		coord_type      coord(int index) { return m_coords.at(index); }
		float      one_over_z(int index) { return m_one_over_zs.at(index); }

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
			//float       d_one_over_z_x;
			//float       d_one_over_z_y;
			float		one_over_dx;
			float		one_over_dy;

			// // Make vectors

			// // Note that the w componenet is the one we use for perspective, z is for occlusion
			m_one_over_zs.push_back(1 / min_y_vert.m_pos.w);
			m_one_over_zs.push_back(1 / mid_y_vert.m_pos.w);
			m_one_over_zs.push_back(1 / max_y_vert.m_pos.w);

			// // Divide by z here so calculations can be made and then teh figure multiplied by z later
			m_colors.push_back(vec4(min_y_vert.m_col) * m_one_over_zs.at(0));
			m_colors.push_back(vec4(mid_y_vert.m_col) * m_one_over_zs.at(1));
			m_colors.push_back(vec4(max_y_vert.m_col) * m_one_over_zs.at(2));

			m_coords.push_back(min_y_vert.m_coord * m_one_over_zs.at(0));
			m_coords.push_back(mid_y_vert.m_coord * m_one_over_zs.at(1));
			m_coords.push_back(max_y_vert.m_coord * m_one_over_zs.at(2));


			// // Interpolation equation
			one_over_dx =
				((mid_y_vert.m_pos.x - max_y_vert.m_pos.x) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y)) - 
				((min_y_vert.m_pos.x - max_y_vert.m_pos.x) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y));
			one_over_dy = -one_over_dx;

			m_col_x_step    = calc_x_step(m_colors, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
			m_col_y_step    = calc_y_step(m_colors, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);

			m_coord_x_step	= calc_x_step(m_coords, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
			m_coord_y_step	= calc_y_step(m_coords, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);

			m_one_over_z_step = {
				calc_x_step(m_one_over_zs, min_y_vert, mid_y_vert, max_y_vert, one_over_dx),
				calc_y_step(m_one_over_zs, min_y_vert, mid_y_vert, max_y_vert, one_over_dy) };

			m_coord_y_step = calc_y_step(m_coords, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);
		}

	private:
		// // WHEN CODE IS WORKING INCORPORATE THIS FUNCTION
		template<typename T>
		T calc_x_step(std::vector<T> values, vertex min_y_vert, vertex mid_y_vert, vertex max_y_vert, float one_over_dx) {
			return
				((values.at(1) - values.at(2)) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y) -
				(values.at(0) - values.at(2)) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y)) / 
				one_over_dx;
		}

		template<typename T>
		T calc_y_step(std::vector<T> values, vertex min_y_vert, vertex mid_y_vert, vertex max_y_vert, float one_over_dy) {
			return
				((values.at(1) - values.at(2)) * (min_y_vert.m_pos.x - max_y_vert.m_pos.x) -
				(values.at(0) - values.at(2)) * (mid_y_vert.m_pos.x - max_y_vert.m_pos.x)) / 
				one_over_dy;
		}
		
	};


}