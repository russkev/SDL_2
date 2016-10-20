#include <array>

#include "tvec.hpp"

namespace graphics {

	template <typename T>
	struct tmat: public tvec4<T> {
		typedef T value_type;
		typedef std::array<tvec4<value_type>, 4 > matrix4_type;

	private:
		matrix4_type m4;

	public:

		tmat() {
			m4;
		}

		// // Getter
		matrix4_type get_m4() { return m4 };
		value_type get(int x, int y) { return m4[x].int_to_loc(y); }
		


		// // Setter
		void set_m4(matrix4_type set_m4){m4 = set_m4}
		void set(int x, int y, value_type value) { m4[x].int_to_lox(y) = value; }

		// // Create identity matrix
		void init_identity() {
			m4[0].x = value_type(1); m4[1].x = value_type(0); m4[2].x = value_type(0); m4[3].x = value_type(0);
			m4[0].y = value_type(0); m4[1].y = value_type(1); m4[2].y = value_type(0); m4[3].y = value_type(0);
			m4[0].z = value_type(0); m4[1].z = value_type(0); m4[2].z = value_type(1); m4[3].z = value_type(0);
			m4[0].w = value_type(0); m4[1].w = value_type(0); m4[2].w = value_type(0); m4[3].w = value_type(1);
		}

		// // Multiply two matrixes
		matrix4_type mult(marix_type r) {

		}




	private:
		char int_to_loc(int y) {
			char vec_loc;
			switch (y) {
			case 0: vec_loc = 'x';
			case 1: vec_loc = 'y';
			case 2: vec_loc = 'z';
			case 3: vec_loc = 'w';
			default: return;
			}
			return vec_loc;
		}
	};
}