#pragma once
#include <vector>
#include <string>

#include "math.hpp"
#include "vertex.hpp"
#include "obj.hpp"

namespace graphics {
	struct mesh {
	private:
		std::vector<vertex> m_vertices;
		std::vector<int>    m_indices;

	public:
		mesh(const char* filename) {
			obj model(filename);
		}
	};
}