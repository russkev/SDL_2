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
		point_type m_normal;


		// // CONSTRUCTORS // //
		// // Full
		vertex(const point_type& s_pos, const coord_type& s_coord, const point_type& s_normal) :
			m_pos(s_pos), m_coord(s_coord), m_normal(s_normal) {}

		// // Full with vec3 for point types
		vertex(const vec3& s_pos, const coord_type& s_coord, const vec3& s_normal) :
			m_pos(point_type(s_pos.x, s_pos.y, s_pos.z, 1)), m_coord(s_coord), m_normal(point_type(s_normal.x, s_normal.y, s_normal.z, s_normal.z)){}

		// // Just position and coordinates
		vertex(const point_type& s_pos, const coord_type& s_coord) :
			vertex(s_pos, s_coord, point_type(0, 0, 0, 0)) {}

		// // Just position
		vertex(const point_type& s_pos) : 
			vertex(s_pos, coord_type(0, 0), point_type(0,0,0,0)) {}

		// // Empty
		vertex() : 
			vertex(point_type(0, 0, 0, 0)) {}

		

	};
	template <typename multiplier>
	vertex operator * (multiplier const& a, vertex const& b) {
		return vertex(a*b.m_pos, b.m_coord, b.m_normal);
	}

	template <typename multiplier>
	vertex operator * (vertex const& a, multiplier const& b) {
		return vertex(point_type(a.m_pos*b), a.m_coord, a.m_normal);
	}

	vertex operator + (vertex const& a, vertex const& b) {
		return vertex(a.m_pos + b.m_pos, a.m_coord, a.m_normal);
	}

	vertex operator - (vertex const& a, vertex const& b) {
		return vertex(a.m_pos - b.m_pos, a.m_coord, a.m_normal);
	}

}