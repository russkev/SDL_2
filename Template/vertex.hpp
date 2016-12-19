#pragma once

namespace graphics {
	struct vertex{
	public:
		typedef u8vec4 bgra_color_type;
		typedef vec4 point_type;

		point_type m_pos;
		bgra_color_type m_color;

		vertex(const point_type& s_pos, const bgra_color_type& s_color) :m_pos(s_pos), m_color(s_color) {}
		vertex(const point_type& s_pos) :m_pos(s_pos), m_color(bgra_color_type(0,0,0,0)) {}
		

	};

	template <typename multiplier>
	vertex operator * (multiplier const& a, vertex const& b) {
		return b.m_pos*a;
	}
}