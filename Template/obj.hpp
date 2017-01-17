#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "math.hpp"


namespace graphics {
	struct obj {
		typedef vec3 obj_vertex_type;
	private:
		std::vector<std::string> m_file_contents;
		std::vector<obj_vertex_type>* m_vertices;
		std::string m_buffer;
	public:
		obj(const char* file_name) {
			// // Open the file, "r" means read
			FILE* file = fopen(file_name, "r");
			if (file == NULL) perror("Couldn't open file!");
			else {
				fseek(file, 0L, SEEK_END);
				long size = ftell(file);
				fseek(file, 0L, SEEK_SET);

				while (std::getline(file, m_buffer)) {

				}

			}

		}
	};
}