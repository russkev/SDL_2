#include <vector>

#include "view.hpp"

namespace graphics {
	template<typename _View>
	struct renderContext{
		typedef _View view_type;
		typedef std::vector<int> buffer_type;

		renderContext(view_type& s_view) :
			s_view(m_view)
		{}

	private:
		view_type& m_view;
		buffer_type scanBuffer ((m_view.size().y*2),0);


	public:
		renderContext(int width, int height) {
			//DO something
		}
	};

}