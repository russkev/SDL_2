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
		std::vector<std::vector<vertex> > m_faces;
		
	public:
		obj(const char* file_name) {
			std::ifstream file(file_name);
			std::string line_str;
			std::istringstream line_ss;
			std::string line_segment;
			int num_face_elements = 3;


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
			//while (file.good()) {
			//	std::getline(file, line_str);
			//	std::istringstream line_ss(line_str);
			//	
			//	//line_ss << line_str;
			//	line_ss >> line_segment;

			//	if (line_segment == "v") {
			//		obj_vertex_type temp_vertex;
			//		line_ss >> temp_vertex.x >> temp_vertex.y >> temp_vertex.z;
			//		m_vertices.push_back(temp_vertex);
			//	}
			//	//line_ss.str("");
			//}

			while (file.good()) {
				std::getline(file, line_str);
				line_ss.str(line_str);
				//line_ss.seekg(0);
				line_ss >> line_segment;

				if (line_segment == "v") {
					obj_vertex_type temp_vertex;
					line_ss >> temp_vertex.x >> temp_vertex.y >> temp_vertex.z;
					m_vertices.push_back(temp_vertex);
				}
				else if (line_segment == "vt") {
					obj_uv_type temp_uv;
					line_ss >> temp_uv.s >> temp_uv.t;
					m_uvs.push_back(temp_uv);
				}
				else if (line_segment == "vn") {
					obj_vertex_type temp_normal;
					line_ss >> temp_normal.x >> temp_normal.y >> temp_normal.z;
					m_normals.push_back(temp_normal);
				}
				else if (line_segment == "f") {
					std::string temp_string, temp_string_ints;
					std::istringstream temp_ss;
					//vertex temp_vertex;
					std::vector<vertex> temp_face;
					std::vector<int> temp_location;
					int temp_counter = 0;

					while (line_ss) {
						line_ss >> temp_string;
						temp_ss.str(temp_string);
						while (std::getline(temp_ss, temp_string_ints, '/')) {
							temp_location.push_back(std::stoi(temp_string));
						}
						temp_face.push_back(vertex(
							m_vertices.at(temp_location.at(0)), 
							m_uvs.at(temp_location.at(1)), 
							m_normals.at(temp_location.at(2))
						));
					}
					m_faces.push_back(temp_face);

					//while (std::getline(line_ss, temp_string, '/')) {
					//	vertex temp_vertex(m_vertices.at(stoi(temp_string)))
						
					//}

				}

				/*
				while (std::getline (yourstream, astring, '\\')) {
					// do stuff
				}
				
				*/
				line_ss.str(std::string());
				line_ss.clear();

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