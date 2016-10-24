#include "tvec.hpp"

namespace graphics {

	template<typename T>
	struct vertex :public tvec4<T> {
		vertex(T x, T y, T z, T w) :x(x), y(y), z(z), w(w) {};

		T get_xy() {
			return x*y;
		}
	};
}