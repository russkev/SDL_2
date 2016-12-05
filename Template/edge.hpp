#include "math.hpp"

// // This is a struct that essentially re-organises the scan buffer so it can handle multiple
// // elements (like colour, lighting, etc. ) at once.
namespace graphics {
	struct edge {
	public:
		float m_x;
		float m_x_step;
		int m_y_start;
		int m_y_end;

	public:
		edge(const vec4& start, const vec4& end) {
			m_y_start = int(ceil(start.y));
			m_y_end = int(ceil(end.y));

			const float y_dist = end.y - start.y;
			const float x_dist = end.x - start.x;


			// // Calculate difference between point on line and the middle of the start pixel
			float y_prestep = m_y_start - start.y;
			// // Use the slope for x step
			m_x_step = x_dist / y_dist;

			m_x = start.x + y_prestep * m_x_step;
		}

		void step() {
			// // By having this as a seperate piece of code here, it makes it easy to add things like colour_step, etc.

			m_x += m_x_step;
		}
	};
}