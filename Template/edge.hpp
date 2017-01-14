#pragma once

#include "math.hpp"
#include "vertex.hpp"
#include "gradients.hpp"
// // This is a struct that essentially re-organises the scan buffer so it can handle multiple
// // elements (like colour, lighting, etc. ) at once.
namespace graphics {
	struct edge {
		typedef vec4 point_type;
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec2 coord_type;

	private:


	public:
		// // GETTERS // //



		// // Main Function:
		edge(
			gradients s_gradients,
			const vertex& start,
			const vertex& end,
			int min_y_vert_index
		){
			//some code here
		}

		void step() {
			//some code here

			}

		}
	};
}