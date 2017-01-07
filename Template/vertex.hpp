#pragma once

#include "math.hpp"

namespace graphics {
	struct vertex {
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;
		typedef vec3 coord_type;

	public:
		point_type m_pos;
		bgra_color_type m_col;
		coord_type m_coord;


		// // Constructors
		vertex(const point_type& s_pos, const coord_type& s_coord, const bgra_color_type& s_color) :m_pos(s_pos), m_col(s_color), m_coord(s_coord) {}
		vertex(const point_type& s_pos, const coord_type& s_coord) : vertex(s_pos, s_coord, bgra_color_type(0, 0, 0, 0)) {}
		vertex(const point_type& s_pos, const bgra_color_type& s_color) : vertex(s_pos, coord_type(0, 0, 0), s_color) {}
		vertex(const point_type& s_pos) : vertex(s_pos, coord_type(0, 0, 0)) {}

		

	};
	template <typename multiplier>
	vertex operator * (multiplier const& a, vertex const& b) {
		return vertex(b.m_pos*a, b.m_col);
	}

	template <typename multiplier>
	vertex operator * (vertex const& a, multiplier const& b) {
		return b*a;
	}

}