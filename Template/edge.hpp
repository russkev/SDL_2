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
	
		// // TRIAL BIT // //
		float m_y_percent;
		float m_y_percent_step;
		float m_z;
		float m_y;
		float m_start_z;
		float m_end_z;
		vertex m_start;
		vertex m_end;
		coord_type m_start_coord;
		coord_type m_end_coord;
		coord_type m_coord_test;
		// // END TRIAL // //


		point_type m_color;
		point_type m_col_step;
		coord_type m_coord;
		coord_type m_coord_step;

	public:
		// // Getters:
		float x()               { return m_x; }
		int y_start()           { return m_y_start; }
		int y_end()             { return m_y_end;   }
		bgra_color_type col()   { return m_color; }
		coord_type coord()      { return m_coord; }
		// // TRIAL BIT // //
		coord_type coord_test() { return m_coord_test; }
		float z()				{ return m_z; }



		// // Main Function:
		edge(
			gradients s_gradients,
			const vertex& start,
			const vertex& end,
			int min_y_vert_index
		):
		m_start(start), m_end(end)		
		{
			m_y_start = int(ceil(start.m_pos.y));
			m_y_end =   int(ceil(  end.m_pos.y));


			// // TRIAL BIT // // 
			m_y = float(m_y_start);

			m_start_z = start.m_pos.w;
			m_end_z = end.m_pos.w;

			m_start_coord = start.m_coord;
			m_end_coord = end.m_coord;

			m_y_percent_step = 1/(float(m_y_end) - float(m_y_start));
			float y_percent_prestep = (float(m_y_start) - start.m_pos.y) * m_y_percent_step;
			m_y_percent = y_percent_prestep;

			// // m_z is the scale factor
			m_z = (start.m_pos.w * (1 - y_percent_prestep)) + (end.m_pos.w * y_percent_prestep);
			// // END TRIAL // //
			
			
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

			// // TRIAL BIT // //
			float z_inverted = 1 / m_z;
			vec2 coordinates_in_3D = { m_x * (z_inverted), m_y_start * (z_inverted) };
			// // END TRIAL // //


			// // Colour to start with after prestep offset has been taken into account:
			m_color =
				point_type(s_gradients.color(min_y_vert_index)) +
				s_gradients.col_x_step() * x_prestep +
				s_gradients.col_y_step() * y_prestep;
			m_coord =
				coord_type(s_gradients.coord(min_y_vert_index)) +
				s_gradients.coord_x_step() * x_prestep +
				s_gradients.coord_y_step() * y_prestep;
			// // This is how much the colour changes when you step one y unit along the edge.
			m_col_step = s_gradients.col_y_step() + s_gradients.col_x_step()*m_x_step;
			// // This is how much the coord will change by
			m_coord_step = s_gradients.coord_y_step() + s_gradients.coord_x_step()*m_x_step;

		}

		void step() {
			// // By having this as a seperate piece of code here, it makes it easy to add things like colour_step, etc.

			m_x         += m_x_step;
			m_color     += m_col_step;
			m_coord     += m_coord_step;

			// // TRIAL BIT // //
			m_y_percent += m_y_percent_step;
			++m_y;
			m_z = lerp(m_start_z, m_end_z, m_y_percent);
			m_coord_test.x = lerp(m_start_coord.s, m_end_coord.s,
				(m_x*m_z - m_start.m_pos.x*m_z) /
				(m_end.m_pos.x*m_z - m_start.m_pos.x*m_z)
			);
			m_coord_test.y = lerp(m_start_coord.t, m_end_coord.t,
				(m_y*m_z - m_start.m_pos.y*m_z) /
				(m_end.m_pos.y*m_z - m_start.m_pos.y*m_z)
			);
			// // END TRIAL // //
		}
	};
}