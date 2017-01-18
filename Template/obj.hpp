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
		std::vector<obj_vertex_type> m_vertices;
		std::string m_buffer;
	public:
		obj(const char* file_name) {
			std::ifstream file(file_name);
			if (!file)
			{
				perror("File does not compute");
			}

			while (std::getline(file, m_buffer)) {
				if (m_buffer[0] != '#' || m_buffer[0] != ' ')
				{
					m_file_contents.push_back(m_buffer);
				}
			}

			if (m_file_contents.size() == 0) {
				perror("File no size! Bad file!");
			}

			for (unsigned int n = 0; n < m_file_contents.size(); n++)
			{
				if (m_file_contents[n].c_str()[0] == 'v' && m_file_contents[n].c_str()[1] == ' ')
				{
					float x, y, z;
					sscanf(m_file_contents[n].c_str(), "v %f %f %f", &x, &y, &z);
					m_vertices.push_back(obj_vertex_type(x, y, z));
				}
			}

		}
	};
}