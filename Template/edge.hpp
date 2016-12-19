#include "math.hpp"
#include "gradients.hpp"

// // This is a struct that essentially re-organises the scan buffer so it can handle multiple
// // elements (like colour, lighting, etc. ) at once.
namespace graphics {
	struct edge {
	private:
		float m_x;
		float m_x_step;
		int m_y_start;
		int m_y_end;
		u8vec4 m_color;
		vec4 m_color_step;


	public:
		// // Getters
		float x() { return m_x; }
		int y_start() { return m_y_start; }
		int y_end() { return m_y_end; }
		u8vec4 color() { return m_color; }

		// // Main function	
		edge(gradients s_gradients, const vec4& start, const vec4& end, int min_y_vert_index) {
			m_y_start = int(ceil(start.y));
			m_y_end = int(ceil(end.y));

			const float y_dist = end.y - start.y;
			const float x_dist = end.x - start.x;


			// // Calculate difference between point on line and the middle of the start pixel
			float y_prestep = m_y_start - start.y;
			// // Use the slope for x step
			m_x_step = x_dist / y_dist;
			m_x = start.x + y_prestep * m_x_step;
			float x_prestep = m_x - start.x;
		}

		void step() {
			// // By having this as a seperate piece of code here, it makes it easy to add things like colour_step, etc.

			m_x += m_x_step;
		}
	};
}

/*		// // This is the old scan convert line code	


		void scan_convert_line(const vec4& min_y_vert, const vec4& max_y_vert, int which_side) {

			// // Work out the pixels to start and finish the line
			const auto y_start = int(ceil(min_y_vert.y));
			const auto y_end   = int(ceil(max_y_vert.y));
			const auto x_start = int(ceil(min_y_vert.x));
			const auto x_end   = int(ceil(max_y_vert.x));

			const auto y_dist = max_y_vert.y - min_y_vert.y;
			const auto x_dist = max_y_vert.x - min_y_vert.x;

			if (y_dist <= 0) {
				return;
			}

			// // Use the slope for x step
			auto x_step = float(x_dist) / float(y_dist);
			// // Calculate difference between point on line and the middle of the start pixel
			auto y_prestep = y_start - min_y_vert.y;
			auto cur_x = min_y_vert.x + y_prestep * x_step;

			// // Convert line to pixel numbers
			batch_line(y_start, y_end, cur_x, x_step, which_side);

		}*/