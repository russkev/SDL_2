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
		float m_y_alpha;
		float m_y_alpha_step;
		float m_alpha;
		float m_alpha_step;
		float m_z;
		int m_y;
		float m_start_z;
		float m_end_z;
		float m_length_line;
		float m_length_3d_line;
		float m_length_prestep;
		vertex m_start;
		vertex m_end;
		coord_type m_start_coord;
		coord_type m_end_coord;
		coord_type m_coord;
		// // END TRIAL // //


		point_type m_color;
		point_type m_col_step;
		//coord_type m_coord;
		//coord_type m_coord_step;

	public:
		// // GETTERS // //
		float x()               { return m_x; }
		int y_start()           { return m_y_start; }
		int y_end()             { return m_y_end;   }
		float x_start()			{ return m_start.m_pos.x; }
		float x_end()			{ return m_end.m_pos.x; }

		bgra_color_type col()   { return m_color; }
		coord_type coord()      { return m_coord; }
		// // TRIAL BIT // //
		float z()				{ return m_z; }
		// // END TRIAL // //


		// // Main Function:
		edge(
			gradients s_gradients,
			const vertex& start,
			const vertex& end,
			int min_y_vert_index
		):
			m_start(start), m_end(end), 
			m_y_start(int(ceil(start.m_pos.y))), m_y_end(int(ceil(end.m_pos.y)))
		{
			// // TRIAL BIT // // 
			if (m_start.m_pos.y > m_end.m_pos.y) {
				__debugbreak();
			}

			m_y = m_y_start;

			m_start_z = start.m_pos.w;
			m_end_z = end.m_pos.w;

			m_start_coord = start.m_coord;
			m_end_coord = end.m_coord;

			m_y_alpha_step = 1/(float(m_y_end) - float(m_y_start));
			float y_prestep = (float(m_y_start) - start.m_pos.y);

			const float y_dist = end.m_pos.y - start.m_pos.y;
			const float x_dist = end.m_pos.x - start.m_pos.x;

			float slope_inverted = x_dist / y_dist;
			float m_x_alpha_step   = slope_inverted / x_dist;

			m_alpha_step = length(vec2(slope_inverted, 1)) / length(vec2(x_dist, y_dist));

			float x_prestep_2 = -(start.m_pos.x - ( ( ( (start.m_pos.y + y_prestep) - start.m_pos.y) * slope_inverted) + start.m_pos.x));


			//float alpha_prestep = sqrt(x_prestep_2 * x_prestep_2 + y_prestep_2 * y_prestep_2);
			m_length_prestep  = distance(vec2(start.m_pos.x, start.m_pos.y), vec2(start.m_pos.x + x_prestep_2, start.m_pos.y + y_prestep));

			m_length_line          = distance(vec2(start.m_pos.x,     start.m_pos.y), vec2(end.m_pos.x,   end.m_pos.y));
			//float length_start_end = distance(vec2(m_start.m_pos.x, m_start.m_pos.y), vec2(m_end.m_pos.x, m_end.m_pos.y));


			m_length_3d_line      = distance(vec3(start.m_pos.x * start.m_pos.w, start.m_pos.y * start.m_pos.w, start.m_pos.w), vec3(end.m_pos.x * end.m_pos.w, end.m_pos.y * end.m_pos.w, end.m_pos.w));
			//m_length_3d_line = length
			m_alpha = m_length_prestep / m_length_line;

			// // m_z is the scale factor
			//m_z = (start.m_pos.w * (1 - y_alpha_prestep)) + (end.m_pos.w * y_alpha_prestep);
			m_z = lerp(start.m_pos.w, end.m_pos.w, m_alpha);
			// // END TRIAL // //
			
			



			// // Calculate difference between point on line and the middle of the start pixel
			//float y_prestep = m_y_start - start.m_pos.y;
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
			// // This is how much the colour changes when you step one y unit along the edge.
			m_col_step = s_gradients.col_y_step() + s_gradients.col_x_step()*m_x_step;
			// // This is how much the coord will change by


		}

		void step() {
			// // By having this as a seperate piece of code here, it makes it easy to add things like colour_step, etc.

			m_x         += m_x_step;
			m_color     += m_col_step;

			// // TRIAL BIT // //
			m_y_alpha += m_y_alpha_step;
			
			++m_y;

			//vec2 start_xy = { m_start.m_pos.x, m_start.m_pos.y };
			//float length_start_end    = distance(vec2(m_start.m_pos.x, m_start.m_pos.y), vec2(m_end.m_pos.x, m_end.m_pos.y));
			float length_start_middle = distance(vec2(m_start.m_pos.x, m_start.m_pos.y), vec2(m_x, m_y));
			float length_start_end    = m_length_line;
			m_alpha =  length_start_middle / m_length_line;
			
			


			m_z = lerp(m_start_z, m_end_z, m_alpha);

			vec3 v_a_vec3 = { m_start.m_pos.x*m_start.m_pos.w, m_start.m_pos.y*m_start.m_pos.w, m_start.m_pos.w };
			vec3 v_b_vec3 = { m_x*m_z, m_y*m_z, m_z };
			vec3 v_c_vec3 = { m_end.m_pos.x*m_end.m_pos.w, m_end.m_pos.y*m_end.m_pos.w, m_end.m_pos.w };


			float middle_length_3d = distance(vec3(m_start.m_pos.x * m_start.m_pos.w, m_start.m_pos.y * m_start.m_pos.w, m_start.m_pos.w), vec3(m_x*m_z, m_y*m_z, m_z));
			float alpha_3d = middle_length_3d / m_length_3d_line;

			m_coord.s = lerp(m_start_coord.s, m_end_coord.s, alpha_3d);
			m_coord.t = lerp(m_start_coord.t, m_end_coord.t, alpha_3d);


			if (m_start.m_pos.y > 500 && 
				m_start.m_pos.x < 490 &&
				m_y%20 == 0
				){
				__debugbreak();
			}
			// // END TRIAL // //
		}
	};
}