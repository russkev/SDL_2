#pragma once
//#pragma warning( disable : 4996 )
//#define _SCL_SECURE_NO_WARNINGS  

#include <cstdint>
#include <vector>
#include <memory>
#include "math.hpp"

namespace graphics {
	typedef tvec4<std::uint8_t> bgra_color_type;
	typedef std::int64_t int64;

	struct xor_texture {
	private:
		int64 m_width;
		int64 m_height;
		ivec2 m_dimensions;
		std::unique_ptr<bgra_color_type[]> m_out_texture; // // unique_pointer is a smart pointer that manages the pointer life time (deletes at right time etc.)
		//std::vector<std::vector<bgra_color_type> > m_out_texture;
	public:

		// // QUESTIONS FOR ALEX:
		// // Why have this constructor that takes in a const reference and itself
		// // How come it can access member variables like that
		// // I get a warning about std::copy being deprecated, do I just disable?
		xor_texture(const xor_texture& other) :
			m_width(other.m_width),
			m_height(other.m_height),
			m_dimensions(float(other.m_width), float(other.m_height)),
			m_out_texture(std::make_unique<bgra_color_type[]>(m_width * m_height))
		{
			//std::copy(other.m_out_texture.get(), other.m_out_texture.get() + m_width*m_height, m_out_texture.get());
			std::copy(other.m_out_texture.get(), other.m_out_texture.get() + m_width*m_height, m_out_texture.get());
		}



		xor_texture(int s_width, int s_height) :
			m_width(int64(s_width)), 
			m_height(int64(s_height)),
			m_dimensions(float(s_width), float(s_height)),
			m_out_texture(std::make_unique<bgra_color_type[]> (s_width * s_height))
		{
			//std::vector<bgra_color_type> temp_vector;
			for (int64 y = 0; y < m_height; ++y) {
				for (int64 x = 0; x < m_width; ++x) {
					auto col = uint8_t(x ^ y);
					m_out_texture[y * m_width + x] = bgra_color_type(col % 128, col % 64, col, 255);
				};
			};
		}
		// // Getter // //
		bgra_color_type get_texture(int x, int y) const {
			return m_out_texture[(y % m_height) * m_width + (x % m_width)]; // Finds correct coord, loops if necessary
		}

		int64 width() { return m_width; }
		int64 height() { return m_height; }
		ivec2 dimensions() { return m_dimensions; }
	};

}