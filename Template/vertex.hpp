#include <glm/glm.hpp>

#include "tvec.hpp"

namespace graphics {
	template <typename T>
	struct vertex: public tvec4 {
		typedef T value_type;
	private:
		tvec4<value_type> m_pos;

	public:
		value_type get_x(){return m_pos.x}
		value_type get_y(){return m_pos.y}

		vertex(value_type x, value_type y, value_type z) : m_pos(tvec4<value_type>(x, y, z, value_type(1))) {};
		vertex(value_type x, value_type y) : vertex(x, y, value_type(0)) {};
		vertex(value_type x) : vertex(x, value_type(0)) {};
		vertex(tvec4<value_type> pos) : m_pos(pos) {};

		vertex transform(glm::mat4x4 transform_mat) {
			return vertex(transform.transform_mat(m_pos))
		}

		vertex perspectiveDivide() {
			return vertex(tvec4(m_pos.x/m_pos.w, m_pos.y/m_pos.w, m_pos.z/m_pos.w, m_pos.w))
		}

		friend float triangle_area(vertex min_y, vertex mid_y, vertex max_y) {

			tvec2<value_type> long_vec = tvec2<value_type>(max_y.get_x(), max_y.get_y()) - tvec2<value_type>(min_y.get_x(), min_y.get_y());
			tvec2<value_type> short_vec = tvec2<value_type>(mid_y.get_x(), mid_y.get_y()) - tvec2<value_type>(min_y.get_x(), min_y.get_y());
			return cross(long_vec, short_vec)*0.5;
		}

	};



}