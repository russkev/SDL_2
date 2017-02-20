#pragma once

#include "math.hpp"
#include "vertex.hpp"
#include "gradients.hpp"
// // This is a struct that essentially re-organises the scan buffer so it can handle multiple
// // elements (like colour, lighting, etc. ) at once.
namespace graphics {
	struct edge {
		// // TYPE DEFINITIONS // //
		typedef vec4 point_type;
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec2 coord_type;

	private:
		// // MEMBER VARIABLES // //
		float m_x, m_x_step;
		int m_y_start, m_y_end;
		coord_type m_coord_over_z, m_coord_over_z_step;
		float m_one_over_z, m_one_over_z_step;
		float m_x_prestep, m_y_prestep;
		float m_depth, m_depth_step;
		float m_light_amt, m_light_step;



	public:
		// // GETTERS // //
		float x()					 { return m_x; }
		int y_start()				 { return m_y_start; }
		int y_end()					 { return m_y_end;   }
		coord_type coord_over_z()    { return m_coord_over_z; }
		float one_over_z()			 { return m_one_over_z; }
		float depth()				 { return m_depth; }
		float light_amt()			 { return m_light_amt; }



		// // MAIN FUNCTION // //
		edge(
			gradients& s_gradients,
			const vertex& start,
			const vertex& end,
			int min_y_vert_index
		) {
			m_y_start = int(ceil(start.m_pos.y));
			m_y_end =   int(ceil(  end.m_pos.y));

			const float y_dist = end.m_pos.y - start.m_pos.y;
			const float x_dist = end.m_pos.x - start.m_pos.x;


			// // Calculate difference between point on line and the middle of the start pixel
			m_y_prestep = m_y_start - start.m_pos.y;
			// // Every time y is incremented by 1, x is incremented by this amount:
			m_x_step = x_dist / y_dist;
			// // m_x is the x component of m_y_start (the ceiling integer value where we are rounding to)
			m_x = start.m_pos.x + m_y_prestep * m_x_step;
			// // x_prestep is just the dustance between the above m_x and the rounded start point.
			m_x_prestep = m_x - start.m_pos.x;

			// // Start positions:
			m_coord_over_z = step_start(s_gradients.coord(min_y_vert_index), s_gradients.coord_over_z_s_step(), s_gradients.coord_over_z_t_step());
			m_one_over_z   = step_start(s_gradients.one_over_z(min_y_vert_index), s_gradients.one_over_z_x_step(), s_gradients.one_over_z_y_step());
			m_depth        = step_start(s_gradients.depth(min_y_vert_index), s_gradients.depth_x_step(), s_gradients.depth_y_step());
			m_light_amt    = step_start(s_gradients.light_amt(min_y_vert_index), s_gradients.light_x_step(), s_gradients.light_y_step());
			

			// // Amount to change by
			m_coord_over_z_step = step_amount(s_gradients.coord_over_z_s_step(), s_gradients.coord_over_z_t_step());
			m_one_over_z_step   = step_amount(s_gradients.one_over_z_x_step(), s_gradients.one_over_z_y_step());
			m_depth_step        = step_amount(s_gradients.depth_x_step(), s_gradients.depth_y_step());
			m_light_step        = step_amount(s_gradients.light_x_step(), s_gradients.light_y_step());

			

		}

		void step() {
			// // By having this as a seperate piece of code here, it makes it easy to add things like colour_step, etc.
			m_one_over_z += m_one_over_z_step;
			m_x += m_x_step;
			m_coord_over_z += m_coord_over_z_step;
			m_depth += m_depth_step;
			m_light_amt += m_light_step;
		}

		template<typename T>
		T step_start(T s_interpolant, T s_x_step, T s_y_step) {
			return s_interpolant + s_x_step * m_x_prestep + s_y_step * m_y_prestep;
		}

		template<typename T>
		T step_amount(T s_x_step, T s_y_step) {
			return s_y_step + s_x_step*m_x_step;
		}
	};
}