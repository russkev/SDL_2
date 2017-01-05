#pragma once

namespace graphics {
	struct vertex{
	public:
		typedef u8vec4 bgra_color_type;
		typedef vec4 point_type;
		typedef vec2 coord_type;

		point_type m_pos;
		bgra_color_type m_color;
		coord_type m_coord;
		
		vertex(const point_type& s_pos, const coord_type& s_coord, const bgra_color_type& s_color) :m_pos(s_pos), m_color(s_color), m_coord(s_coord) {}
		vertex(const point_type& s_pos, const coord_type& s_coord) : vertex(s_pos, s_coord, bgra_color_type(0, 0, 0, 0)) {}
		vertex(const point_type& s_pos, const bgra_color_type& s_color) : vertex(s_pos, coord_type(0, 0), s_color) {}
		vertex(const point_type& s_pos) : vertex(s_pos, coord_type(0, 0)) {}


		template <typename multiplier>
		vertex operator * (multiplier const& a) {
			return this->m_pos*a;
		}
		

	};

	template <typename multiplier>
	vertex operator * (multiplier const& a, vertex const& b) {
		return b.m_pos*a;
	}
}