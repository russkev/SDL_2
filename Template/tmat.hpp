#include <array>

#include "tvec.hpp"

namespace graphics {

	template <typename T>
	struct tmat: public tvec4<T> {
		typedef T value_type;
		typedef std::array<std::array<value_type, 4>, 4 > matrix4_type;

	private:
		matrix4_type m4;

	public:

		tmat() {
			m4;
		}

		// // Getter
		matrix4_type get_m4() { return m4 };
		value_type get(int x, int y) { return m4[x][y]; }
		


		// // Setter
		void set_m4(matrix4_type set_m4){m4 = set_m4}
		void set(int x, int y, value_type value) { m4[x].int_to_lox(y) = value; }

		// // Create identity matrix
		void init_identity() {
			m4[0][0] = value_type(1); m4[1][0] = value_type(0); m4[2][0] = value_type(0); m4[3][0] = value_type(0);
			m4[0][1] = value_type(0); m4[1][1] = value_type(1); m4[2][1] = value_type(0); m4[3][1] = value_type(0);
			m4[0][2] = value_type(0); m4[1][2] = value_type(0); m4[2][2] = value_type(1); m4[3][2] = value_type(0);
			m4[0][3] = value_type(0); m4[1][3] = value_type(0); m4[2][3] = value_type(0); m4[3][3] = value_type(1);
		}

		// // Multiply two matrixes
		matrix4_type mult(marix4_type r) {
			matrix4_type m4_out = {
				{ dot(row_to_vec(m4, 0),col_to_vec(r,0)), dot(row_to_vec(m4, 0),col_to_vec(r,1)), dot(row_to_vec(m4, 0),col_to_vec(r,2)), dot(row_to_vec(m4, 0),col_to_vec(r,3)) },
				{ dot(row_to_vec(m4, 1),col_to_vec(r,0)), dot(row_to_vec(m4, 1),col_to_vec(r,1)), dot(row_to_vec(m4, 1),col_to_vec(r,2)), dot(row_to_vec(m4, 1),col_to_vec(r,3)) },
				{ dot(row_to_vec(m4, 2),col_to_vec(r,0)), dot(row_to_vec(m4, 2),col_to_vec(r,1)), dot(row_to_vec(m4, 2),col_to_vec(r,2)), dot(row_to_vec(m4, 2),col_to_vec(r,3)) },
				{ dot(row_to_vec(m4, 3),col_to_vec(r,0)), dot(row_to_vec(m4, 3),col_to_vec(r,1)), dot(row_to_vec(m4, 3),col_to_vec(r,2)), dot(row_to_vec(m4, 3),col_to_vec(r,3)) },
			};
			return m4_out;
		}

		tvec4<value_type> row_to_vec(tvec4<value_type> r, int row) {
			return tvec4<value_type>(m4[0][row], m4[1][row], m4[2][row], m4[3][row]);
		}

		tvec4<value_type> col_to_vec(tvec4<value_type> r, int row) {
			return tvec4<value_type>(m4[col][0], m4[col][1], m4[col][2], m4[col][3]);
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