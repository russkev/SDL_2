#include "tvec.hpp"

namespace graphics {

	template<typename T>
	struct vertex{
	private:
		tvec4<T> m_pos;

	public:
		vertex(T x, T y, T z) : m_pos(tvec4<T>(x,y,z,T(1))) {};

		//T get_xy() {
		//	return m_pos.x*m_pos.y;
		//}
	};
}