#include <vector>

#include "view.hpp"

namespace graphics {
	template<typename _View>
	struct renderContext{
		typedef _View view_type;
		typedef std::vector<int> buffer_type;
		typedef tvec4<std::uint8_t> bgra_color_type;

		renderContext(view_type& s_view) :
			m_view(s_view), 
			m_scan_buffer (s_view.size().y * 2, 0)
		{}

	private:
		view_type& m_view;
		std::vector<int> m_scan_buffer;
		//buffer_type scanBuffer;// ((m_view.size().y * 2), 0);


	public:
		void draw_scan_buffer(int y_coord, int x_min, int x_max) {
			m_scan_buffer[y_coord * 2    ] = x_min;
			m_scan_buffer[y_coord * 2 + 1] = x_max;
		}

		void fill_shape(int y_min, int y_max) {
			for (int j = y_min; j < y_max; ++j) {
				int x_min = m_scan_buffer[j * 2];
				int x_max = m_scan_buffer[j * 2 + 1];
				for (int i = x_min; i < x_max; ++i) {
					blend_element(m_view, tvec2<int>(i, j), bgra_color_type(0, 0, 255, 255));
				}
			}
		}

	};

}