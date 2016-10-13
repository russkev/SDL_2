#include <vector>

#include "view.hpp"

namespace graphics {
	template<typename _View>
	struct renderContext{
		typedef _View view_type;

		renderContext(view_type& s_view) :
			s_view(m_view)
		{}

	private:
		view_type& m_view;
		std::vector<int> scanBuffer;// (5);//m_view.size().y*2);


	public:
		renderContext(int width, int height) {
			//DO something
		}
	};

}