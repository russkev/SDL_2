#pragma once

#include "math.hpp"

namespace graphics {
	struct vertex {
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;
		typedef vec2 coord_type;
	public:
		point_type m_pos;
		coord_type m_tex_coords;

		// // Constructors
		vertex(const point_type& s_pos, const coord_type& s_coord) :m_pos(s_pos), m_tex_coords(s_coord) {}
		vertex(const point_type& s_pos) :vertex(s_pos, coord_type (0,0)) {}
		vertex(float x, float y, float z, float w) : vertex(point_type(x,y,z,w)) {}

		

	};
	template <typename multiplier>
	vertex operator * (multiplier const& a, vertex const& b) {
		return vertex(b.m_pos*a, b.m_tex_coords);
	}

	template <typename multiplier>
	vertex operator * (vertex const& a, multiplier const& b) {
		return b*a;
	}

}