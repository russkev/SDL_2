#include "math.hpp";

namespace graphics {
	struct vertex {
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;
	public:
		point_type m_pos;
		bgra_color_type m_col;

		// // Constructor
		vertex(const point_type& s_pos, const bgra_color_type& s_col) :m_pos(s_pos), m_col(s_col) {}

		vertex(float x, float y, float z, float w) : m_pos(point_type(x, y, z, w)), m_col(bgra_color_type(0, 0, 0, 255)) {}

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