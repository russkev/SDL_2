#pragma once

#include <iostream>
#include "algorithm.hpp"
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

		// // GETTERS // //
		float get(int index) const {
			switch (index) {
			case 0: return m_pos.x; 
				break;
			case 1: return m_pos.y;
				break;
			case 2: return m_pos.z;
				break;
			case 3: return m_pos.w;
				break;
			default: 
				std::cout << "Invalid get value" << std::endl;
				__debugbreak();
				return -1;
			}
		}


		// // CONSTRUCTORS // //
		// // Full
		vertex(const point_type& s_pos, const coord_type& s_coord, const point_type& s_normal) :
			m_pos(s_pos), m_coord(s_coord), m_normal(s_normal) {}

		// // Full with vec3 for point types
		vertex(const vec3& s_pos, const coord_type& s_coord, const vec3& s_normal) :
			m_pos(point_type(s_pos.x, s_pos.y, s_pos.z, 1)), m_coord(s_coord), m_normal(point_type(s_normal.x, s_normal.y, s_normal.z, 0.0f)){}

		// // Just position and coordinates
		vertex(const point_type& s_pos, const coord_type& s_coord) :
			vertex(s_pos, s_coord, point_type(0, 0, 0, 0)) {}

		// // Just position
		vertex(const point_type& s_pos) : 
			vertex(s_pos, coord_type(0, 0), point_type(0,0,0,0)) {}

		// // Empty
		vertex() : 
			vertex(point_type(0, 0, 0, 0)) {}

		 // FUNCTIONS // //
		template <typename _Qtype>
		auto vertex_lerp(const vertex& b, const _Qtype& q) {
			//return lerp(this->m_pos, b.m_pos, 0.5);
			return vertex(
				lerp(this->m_pos, b.m_pos, q),
				lerp(this->m_coord, b.m_coord, q),
				lerp(this->m_normal, b.m_normal, q)
			);
		}

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