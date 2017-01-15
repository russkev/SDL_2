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
		coord_type m_coord_x_step;
		coord_type m_coord_y_step;
		float m_one_over_z_x_step;
		float m_one_over_z_y_step;


		std::vector<coord_type> m_coords;
		std::vector<float> m_one_over_zs;



	public:
		// // Getters
		coord_type coord_x_step() { return m_coord_x_step; }
		coord_type coord_y_step() { return m_coord_y_step; }    
		float one_over_z_x_step() { return m_one_over_z_x_step; }
		float one_over_z_y_step() { return m_one_over_z_y_step; }


		coord_type      coord(int index) { return m_coords.at(index); }
		float	   one_over_z(int index) { return m_one_over_zs.at(index); }

		// // Constructor
		gradients(
			const vertex& min_y_vert,
			const vertex& mid_y_vert,
			const vertex& max_y_vert
		) {
			// // Constructor variables
			float		one_over_dx;
			float		one_over_dy;

			// // Make vectors
			m_coords.push_back(min_y_vert.m_coord);
			m_coords.push_back(mid_y_vert.m_coord);
			m_coords.push_back(max_y_vert.m_coord);

			m_one_over_zs.push_back(1 / min_y_vert.m_pos.w);
			m_one_over_zs.push_back(1 / mid_y_vert.m_pos.w);
			m_one_over_zs.push_back(1 / max_y_vert.m_pos.w);


			// // Interpolation equation
			// // Basically the x and y steps can be pre-calculated so that you don't have to calculate it every time for every edge

			one_over_dx =
				((mid_y_vert.m_pos.x - max_y_vert.m_pos.x) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y)) - 
				((min_y_vert.m_pos.x - max_y_vert.m_pos.x) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y));
			one_over_dy = -one_over_dx;



			m_coord_x_step = calc_x_step(m_coords, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
			m_coord_y_step = calc_y_step(m_coords, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);

			m_one_over_z_x_step = calc_x_step(m_one_over_zs, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
			m_one_over_z_y_step = calc_y_step(m_one_over_zs, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);
		}

		template <typename interpolant>
		auto calc_x_step(const std::vector < interpolant >& values, const vertex& min_y_vert, const vertex& mid_y_vert, const vertex& max_y_vert, float one_over_dx) {
			return
				((values.at(1) - values.at(2)) * (min_y_vert.m_pos.y - max_y_vert.m_pos.y) - 
				 (values.at(0) - values.at(2)) * (mid_y_vert.m_pos.y - max_y_vert.m_pos.y)) / 
				one_over_dx;
		}

		template <typename interpolant>
		auto calc_y_step(const std::vector < interpolant >& values, const vertex& min_y_vert, const vertex& mid_y_vert, const vertex& max_y_vert, float one_over_dy) {
			return
				((values.at(1) - values.at(2)) * (min_y_vert.m_pos.x - max_y_vert.m_pos.x) -
				(values.at(0) - values.at(2)) * (mid_y_vert.m_pos.x - max_y_vert.m_pos.x)) /
				one_over_dy;
		}
	};


}