#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "math.hpp"


namespace graphics {
	struct obj {
		typedef vec3 obj_vertex_type;
	private:
		std::vector<obj_vertex_type>* vertices;
	public:
		obj(const char* file_name) {
			// // Open the file
			FILE* file = fopen(file_name, "r");
			fseek(file, 0L, SEEK_END);
			long size = ftell(file);
			fseek(file, 0L, SEEK_END);
		}
	};
}