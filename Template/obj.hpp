#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "math.hpp"


namespace graphics {
	struct obj {
		typedef vec3 obj_vertex_type;
		typedef vec2 obj_uv_type;
	private:
		std::vector<std::string> m_file_contents;
		std::vector<obj_vertex_type> m_vertices;
		std::vector<obj_vertex_type> m_normals;
		std::vector<obj_uv_type> m_uvs;
		std::vector<std::vector<ivec3>> m_faces;
		
	public:
		obj(const char* file_name) {
			std::ifstream file(file_name);
			std::string line_str;
			std::stringstream line_ss;
			std::string line_segment;


			if (!file)
			{
				perror("File does not compute");
			}
			//std::stringstream ss;
			//std::string buffer;
			//if (file) {
			//	ss << file.rdbuf();
			//	file.close();
			//}
			while (file.good()) {
				std::getline(file, line_str);
				
				line_ss << line_str;
				line_ss >> line_segment;

				if (line_segment == "v") {
					obj_vertex_type temp_vertex;
					line_ss >> temp_vertex.x >> temp_vertex.y >> temp_vertex.z;
					m_vertices.push_back(temp_vertex);
				}
				line_ss.str("");
			}


			//while (std::getline(file, buffer)) {
			//	if (buffer[0] != '#' || buffer[0] != ' ')
			//	{
			//		m_file_contents.push_back(buffer);
			//	}
			//}

			//if (m_file_contents.size() == 0) {
			//	perror("File no size! Bad file!");
			//}

			//for (unsigned int n = 0; n < m_file_contents.size(); n++)
			//{
			//	// // Add vertices // //
			//	if (m_file_contents[n].c_str()[0] == 'v' && m_file_contents[n].c_str()[1] == ' ')
			//	{
			//		float x, y, z;
			//		sscanf(m_file_contents[n].c_str(), "v %f %f %f", &x, &y, &z);
			//		m_vertices.push_back(obj_vertex_type(x, y, z));
			//	}
			//	// // Add UVs // //
			//	if (m_file_contents[n].c_str()[0] == 'v' && m_file_contents[n].c_str()[1] == 't')
			//	{
			//		float u, v;
			//		sscanf(m_file_contents[n].c_str(), "vt %f %f", &u, &v);
			//		m_uvs.push_back(obj_uv_type(u, v));
			//	}
			//	// // Add normals // //
			//	if (m_file_contents[n].c_str()[0] == 'v' && m_file_contents[n].c_str()[1] == 'n')
			//	{
			//		float x, y, z;
			//		sscanf(m_file_contents[n].c_str(), "vn %f %f %f", &x, &y, &z);
			//		m_normals.push_back(obj_vertex_type(x, y, z));
			//	}
			//	// // Add faces // //
			//	//if (m_file_contents[n].c_str()[0] == 'f' && m_file_contents[n].c_str()[1] == ' ')
			//	//{
			//	//	int p = 3;
			//	//	while (m_file_contents[n].c_str()[p] == '/') {

			//	//	}
			//	//	//float x, y, z;
			//	//	//sscanf(m_file_contents[n].c_str(), "f %f %f", &u, &v);
			//	//	//m_faces.push_back(obj_uv_type(u, v));
			//	//}
			//}

		}
	};
}