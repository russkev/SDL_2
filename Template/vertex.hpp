#pragma once

#include "math.hpp"

namespace graphics {
	struct vertex {
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;
		typedef vec2 coord_type;

	public:
		point_type m_pos;
		coord_type m_coord;


		// // Constructors
		vertex(const point_type& s_pos, const coord_type& s_coord) :m_pos(s_pos), m_coord(s_coord) {}
		vertex(const point_type& s_pos) : vertex(s_pos, coord_type(0, 0)) {}

		

	};
	template <typename multiplier>
	vertex operator * (multiplier const& a, vertex const& b) {
		return vertex(a*b.m_pos, b.m_coord);
	}

	template <typename multiplier>
	vertex operator * (vertex const& a, multiplier const& b) {
		return vertex(a.m_pos*b, b.m_coord);
	}

}