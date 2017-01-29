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
		typedef std::vector<vertex> face_type;


	private:
		// // MEMBER VARIABLES // //
		std::vector<std::string> m_file_contents;
		std::vector<obj_vertex_type> m_vertices;
		std::vector<obj_vertex_type> m_normals;
		std::vector<obj_uv_type> m_uvs;
		std::vector<std::vector<vertex> > m_faces;
		
	public:
		// // INITIALIZE // //
		obj(const char* file_name) {
			// // Declare function variables
			std::ifstream file(file_name);
			std::string line_str;
			std::istringstream line_ss;
			std::string line_segment;


			if (!file)
			{
				perror("File does not compute");
			}

			while (file.good()) {
				std::getline(file, line_str);
				line_ss.str(line_str);
				line_ss >> line_segment;

				// // Parse vertices
				if (line_segment == "v") {
					obj_vertex_type temp_vertex;
					line_ss >> temp_vertex.x >> temp_vertex.y >> temp_vertex.z;
					m_vertices.push_back(temp_vertex);
				}

				// // Parse UV coordinates
				else if (line_segment == "vt") {
					obj_uv_type temp_uv;
					line_ss >> temp_uv.s >> temp_uv.t;
					m_uvs.push_back(temp_uv);
				}

				// // Parse normals
				else if (line_segment == "vn") {
					obj_vertex_type temp_normal;
					line_ss >> temp_normal.x >> temp_normal.y >> temp_normal.z;
					m_normals.push_back(temp_normal);
				}

				// // Parse faces
				else if (line_segment == "f") {

					// // Declare temp variables
					std::string temp_string, temp_string_ints;
					std::istringstream temp_ss;
					face_type temp_face;
					std::vector<int> temp_location;

					while (line_ss.good()) {
						//temp_location.clear();
						// // Extract just one of the vertices
						line_ss >> temp_string;
						temp_ss.str(temp_string);

						// // Extract individual ints
						while (std::getline(temp_ss, temp_string_ints, '/')) {
							temp_location.push_back(std::stoi(temp_string_ints));
						}
						// // Create vertex
						temp_face.push_back(vertex(
							m_vertices.at(temp_location.at(0) - 1), 
							m_uvs.at(temp_location.at(1) - 1), 
							m_normals.at(temp_location.at(2) - 1)
						));
						//temp_location.clear();
						temp_location.erase(temp_location.begin(), temp_location.end());
						temp_ss.str(std::string());
						temp_ss.clear();
					}
					m_faces.push_back(temp_face);
				}


				// // debugging //
				bool temp_break = false;
				if (line_segment == "f" && m_faces.size() == 5) {
					temp_break = true;
					//__debugbreak();
				}
				// // end debug // //

				// // Reset string stream
				line_ss.str(std::string());
				line_ss.clear();

			}
		}

		// // GETTER // //
		face_type get_face(int face) { return m_faces.at(face); }
	};
}