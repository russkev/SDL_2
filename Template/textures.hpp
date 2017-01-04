# pragma once
#include <cstdint>
#include <vector>
#include "math.hpp"

namespace graphics {
	typedef tvec4<std::uint8_t> bgra_color_type;
	typedef std::int64_t int64;

	struct xor_texture {
	private:
		int64 m_width;
		int64 m_height;
		ivec2 m_dimensions;
		std::vector<std::vector<bgra_color_type> > m_out_texture;
	public:
		xor_texture(int s_width, int s_height) :m_width(int64(s_width)), m_height(int64(s_height)) {
			m_dimensions = ivec2(int(m_width), int(m_height));
			std::vector<bgra_color_type> temp_vector;
			for (int64 y = 0; y < m_height; ++y) {
				for (int64 x = 0; x < m_width; ++x) {
					auto col = uint8_t(x ^ y);
					temp_vector.push_back(bgra_color_type(col % 128, col % 64, col, 255));
				};
				m_out_texture.push_back(temp_vector);
				temp_vector.clear();
			};
		}
		// // Getter // //
		bgra_color_type get_texture(int x, int y) {
			return m_out_texture.at(x).at(y);
		}

		int64 width() { return m_width; }
		int64 height() { return m_height; }
		ivec2 dimensions() { return m_dimensions; }

		bgra_color_type copy_pixel(int dest_x, int dest_y, int src_x, int src_y) {
			// do something
		}
	};

}