#pragma once

#include "math.hpp"
#include "vertex.hpp"
#include "gradients.hpp"
// // This is a struct that essentially re-organises the scan buffer so it can handle multiple
// // elements (like colour, lighting, etc. ) at once.
namespace graphics {
	struct edge {
		typedef vec4 point_type;
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec2 coord_type;

	private:
		float m_x;
		float m_x_step;
		int m_y_start;
		int m_y_end;
		coord_type m_coord_over_z;
		coord_type m_coord_over_z_step;

		float m_one_over_z;
		float m_one_over_z_step;

	public:
		// // Getters:
		float x()					 { return m_x; }
		int y_start()				 { return m_y_start; }
		int y_end()					 { return m_y_end;   }
		coord_type coord_over_z()    { return m_coord_over_z; }
		float one_over_z()			 { return m_one_over_z;}



		// // Main Function:
		edge(
			gradients s_gradients,
			const vertex& start,
			const vertex& end,
			int min_y_vert_index
		) {
			m_y_start = int(ceil(start.m_pos.y));
			m_y_end =   int(ceil(  end.m_pos.y));

			const float y_dist = end.m_pos.y - start.m_pos.y;
			const float x_dist = end.m_pos.x - start.m_pos.x;


			// // Calculate difference between point on line and the middle of the start pixel
			float y_prestep = m_y_start - start.m_pos.y;
			// // Every time y is incremented by 1, x is incremented by this amount:
			m_x_step = x_dist / y_dist;
			// // m_x is the x component of m_y_start (the ceiling integer value where we are rounding to)
			m_x = start.m_pos.x + y_prestep * m_x_step;
			// // x_prestep is just the dustance between the above m_x and the rounded start point.
			float x_prestep = m_x - start.m_pos.x;
			// // Colour to start with after prestep offset has been taken into account:
			m_coord_over_z =
				coord_type(s_gradients.coord(min_y_vert_index)) +
				s_gradients.coord_over_z_s_step() * x_prestep +
				s_gradients.coord_over_z_t_step() * y_prestep;
			// // This is how much the coord will change by
			m_coord_over_z_step = s_gradients.coord_over_z_t_step() + s_gradients.coord_over_z_s_step()*m_x_step;


			m_one_over_z =
				s_gradients.one_over_z(min_y_vert_index) +
				s_gradients.one_over_z_x_step() * x_prestep +
				s_gradients.one_over_z_y_step() * y_prestep;
			m_one_over_z_step = s_gradients.one_over_z_y_step() + s_gradients.one_over_z_x_step()*m_x_step;

		}

		void step() {
			// // By having this as a seperate piece of code here, it makes it easy to add things like colour_step, etc.
			m_one_over_z += m_one_over_z_step;
			m_x += m_x_step;
			m_coord_over_z += m_coord_over_z_step;
		}
	};
}