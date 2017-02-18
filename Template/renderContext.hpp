#include <vector>
#include <thread>
#include <future>
#include <cstdint>
#include <limits>
#include <array>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "math.hpp"
#include "view.hpp"
#include "algorithm.hpp"
#include "edge.hpp"
#include "gradients.hpp"
#include "textures.hpp"
#include "obj.hpp"

#define USE_MULTITHREADING 1


namespace graphics {
	template<typename _View>
	struct renderContext {
		// // TYPE DEFINITIONS // //
		typedef _View view_type;
		typedef std::vector<std::pair<int, int> > buffer_type;
		typedef tvec4<std::uint8_t> bgra_color_type;
		typedef vec4 point_type;
		typedef xor_texture texture_type;
		typedef vec2 coord_type;
		typedef obj mesh_type;

		// // CONSTRUCTOR // //
		renderContext(view_type& s_view) :
			m_view(s_view),
			m_scan_buffer (int(s_view.size().y) * 2, { 0, 0 }),
			m_total_pixels(int(s_view.size().x) * int(s_view.size().y))			
			//m_depth_buffer(std::make_unique<float[]>(s_view.size().x * s_view.size().y))
		{
			m_array_depth_buffer = std::make_unique<float[]>(m_total_pixels);
			//m_depth_buffer.resize(m_total_pixels, std::numeric_limits<float>::max());
		}



	private:
		// // MEMBER VARIABLES // //
		view_type& m_view;
		buffer_type m_scan_buffer;
		std::unique_ptr<float[]> m_array_depth_buffer;
		std::vector<float> m_depth_buffer;
		int m_total_pixels;
		//std::array<float> m_array_depth_buffer;


	public:

		// // MAIN FUNCTIONS // //
		void clear_depth_buffer() {
			//m_depth_buffer.clear();
			//m_depth_buffer.resize(m_total_pixels, std::numeric_limits<float>::max());
			for (int i = 0; i < m_total_pixels; ++i) {
				m_array_depth_buffer[i] = std::numeric_limits<float>::min();
			}
		}
		
	public: // Should be private
		bool clip_polygon_axis(std::vector<vertex>& vertex_list, int component, std::vector<vertex>& aux_vector) {
			clip_polygon_component(vertex_list, component, -1.0, aux_vector);
			vertex_list.clear();

			if (aux_vector.empty()) {
				return false;
			}
			clip_polygon_component(aux_vector, component, 1.0, vertex_list);
			aux_vector.clear();

			return !vertex_list.empty();
		}

		void clip_polygon_component(const std::vector<vertex>& vertex_list, int component, float factor, std::vector<vertex>& result) {
			vertex previous_vertex = vertex_list.back();
			float previous_component = previous_vertex.get(component) * factor;
			bool previous_inside = (previous_component <= previous_vertex.m_pos.w);

			for (auto& vertex_a : vertex_list) {
				vertex current_vertex = vertex_a;
				float current_component = float(vertex_a.get(component)) * factor;
				bool current_inside = (current_component <= current_vertex.m_pos.w);

				if (current_inside != previous_inside) {
					float lerp_factor =
						(previous_vertex.m_pos.w - previous_component) /
						((previous_vertex.m_pos.w - previous_component) -
						(current_vertex.m_pos.w - current_component));
					result.push_back(previous_vertex.vertex_lerp(current_vertex, lerp_factor));
				}

				if (current_inside) {
					result.push_back(current_vertex);
				}
				
				previous_vertex = current_vertex;
				previous_component = current_component;
				previous_inside = current_inside;
			}
		}
		
		void fill_triangle(
			const vertex& p1, 
			const vertex& p2, 
			const vertex& p3,
			const texture_type& s_texture
		) {

			//if (
			//	!is_inside_view_frustrum(p1) ||
			//	!is_inside_view_frustrum(p2) ||
			//	!is_inside_view_frustrum(p3)) {
			//	std::cout << "outside of clipping area";
			//	__debugbreak();
			//}

			mat4 screen_space_transform = init_screen_space_transform(float(m_view.size().x), float(m_view.size().y));

			// // Assign max, mid and min y vert arbitrarily, they will be sorted in next step
			auto min_y_vert = vertex(perspective_divide(screen_space_transform*p1.m_pos), p1.m_coord);
			auto mid_y_vert = vertex(perspective_divide(screen_space_transform*p2.m_pos), p2.m_coord);
			auto max_y_vert = vertex(perspective_divide(screen_space_transform*p3.m_pos), p3.m_coord);

			// // Backface culling
			if (triangle_area(min_y_vert.m_pos, mid_y_vert.m_pos, max_y_vert.m_pos) >= 0) {
				return;
			}

			// // Sort points so min, mid and max contain the correct values.
			if (max_y_vert.m_pos.y < min_y_vert.m_pos.y) { std::swap(min_y_vert, max_y_vert); }
			if (mid_y_vert.m_pos.y < min_y_vert.m_pos.y) { std::swap(min_y_vert, mid_y_vert); }
			if (mid_y_vert.m_pos.y > max_y_vert.m_pos.y) { std::swap(max_y_vert, mid_y_vert); }

			scan_triangle(min_y_vert, mid_y_vert, max_y_vert, s_texture);
		}

	public:
		void draw_triangle(
			const vertex& p1,
			const vertex& p2,
			const vertex& p3,
			const texture_type& s_texture
		) {
			if (is_inside_view_frustrum(p1) && is_inside_view_frustrum(p2) && is_inside_view_frustrum(p3)) {
				fill_triangle(p1, p2, p3, s_texture);
				return;
			}

			if (!is_inside_view_frustrum(p1) && !is_inside_view_frustrum(p2) && !is_inside_view_frustrum(p3)) {
				return;
			}


			std::vector<vertex> vertices = { p1, p2, p3 };
			std::vector<vertex> aux_vector;

			bool x_clip = clip_polygon_axis(vertices, 0, aux_vector);
			bool y_clip = clip_polygon_axis(vertices, 1, aux_vector);
			bool z_clip = clip_polygon_axis(vertices, 2, aux_vector);

			if (x_clip && y_clip && z_clip) {

				for (int i = 1; i < vertices.size() - 1; ++i) {
					fill_triangle(vertices.at(0), vertices.at(i), vertices.at(i + 1), s_texture);
				}
			}
		}


		void draw_mesh(const obj& mesh, const mat4& transform, const texture_type& texture) {
			for (int i = 0; i < mesh.size(); ++i) {

				draw_triangle(
					transform*mesh.face(i).at(0),
					transform*mesh.face(i).at(1),
					transform*mesh.face(i).at(2),
					texture);
			}
		}


	private:
		void scan_triangle(const vertex& min_y_vert, const vertex& mid_y_vert, const vertex& max_y_vert, const texture_type& s_texture) {
#if USE_MULTITHREADING
			const auto s_num_threads = std::thread::hardware_concurrency();
			std::vector<std::future<void>> s_threads;
			s_threads.reserve(s_num_threads);
			
			for (uint i = 0; i < s_num_threads; ++i) {
				s_threads.emplace_back(std::async(std::launch::async, [&, i]() {
					gradients m_gradients (min_y_vert, mid_y_vert, max_y_vert);
					edge top_to_bottom    (m_gradients, min_y_vert, max_y_vert, 0);
					edge top_to_middle    (m_gradients, min_y_vert, mid_y_vert, 0);
					edge middle_to_bottom (m_gradients, mid_y_vert, max_y_vert, 1);

					// // If triangle is left handed, lead with top to middle, else lead with top to bottom
					if (triangle_area(min_y_vert.m_pos, mid_y_vert.m_pos, max_y_vert.m_pos) >= 0) { 
						scan_edges(top_to_middle,     top_to_bottom,     top_to_middle,    s_num_threads, i, s_texture);
						scan_edges(middle_to_bottom,  top_to_bottom,     middle_to_bottom, s_num_threads, i, s_texture);
					} else { 										
						scan_edges(top_to_bottom,     top_to_middle,     top_to_middle,    s_num_threads, i, s_texture);
						scan_edges(top_to_bottom,     middle_to_bottom,  middle_to_bottom, s_num_threads, i, s_texture);
					}
				}));
			}

			for (auto&t : s_threads) t.get();
#else

			gradients m_gradients(min_y_vert, mid_y_vert, max_y_vert);
			edge top_to_bottom   (m_gradients, min_y_vert, max_y_vert, 0);
			edge top_to_middle   (m_gradients, min_y_vert, mid_y_vert, 0);
			edge middle_to_bottom(m_gradients, mid_y_vert, max_y_vert, 1);

			// // If triangle is left handed, lead with top to middle, else lead with top to bottom
			if (triangle_area(min_y_vert.m_pos, mid_y_vert.m_pos, max_y_vert.m_pos) >= 0) {
				scan_edges(top_to_middle,    top_to_bottom,    top_to_middle   , s_texture);
				scan_edges(middle_to_bottom, top_to_bottom,    middle_to_bottom, s_texture); 
			} else {																		
				scan_edges(top_to_bottom,    top_to_middle,    top_to_middle   , s_texture);
				scan_edges(top_to_bottom,    middle_to_bottom, middle_to_bottom, s_texture);
			}
#endif

		}

		void scan_edges(edge& left, edge& right, edge& lead
#if USE_MULTITHREADING
			,unsigned every_nth, unsigned plus_i
#endif
			,const texture_type& s_texture
			)
		{
			for (int j = lead.y_start(); j < lead.y_end(); ++j) {
#if USE_MULTITHREADING
				if (((j - lead.y_start()) % every_nth) == plus_i) {
					draw_scan_line(left, right, j, s_texture);
				}
#else
				draw_scan_line(left, right, j, s_texture);
#endif
				left.step();
				right.step();
			}
		}

		void draw_scan_line(edge& left, edge& right, int j, const texture_type& s_texture) {

			auto x_min  = int(ceil(left.x()));
			auto x_max  = int(ceil(right.x()));
			if (x_min > x_max) std::swap(x_min, x_max);
			
			float x_prestep         = x_min-left.x();
			float x_dist            = right.x() - left.x();
			
			// // Work out what the next step for each calculation to reduce errors to do with precision
			coord_type coord_over_z_x_step = ( right.coord_over_z() - left.coord_over_z() ) / x_dist;
			float one_over_z_x_step        = ( right.one_over_z()   - left.one_over_z()   ) / x_dist;
			float depth_x_step             = ( right.depth()        - left.depth()        ) / x_dist;
			// // Calculate start coordinate
			coord_type coord_over_z = left.coord_over_z() + coord_over_z_x_step * x_prestep;
			float one_over_z        = left.one_over_z()   + one_over_z_x_step   * x_prestep;
			float depth             = left.depth()        + depth_x_step        * x_prestep;
			



			for (int i = x_min; i < x_max; ++i) {
				int index = i + j * int(m_view.size().x);
				float depth_existing = m_array_depth_buffer[index];
				if (depth > depth_existing) {
					m_array_depth_buffer[index] = depth;
					float z = 1 / one_over_z;
					vec2 source = {
						(coord_over_z.s * z + 0.5),
						(coord_over_z.t * z + 0.5)
					};
					m_view[j][i] = s_texture.get_texture(source.x, source.y); // // j = y, i = x
				}
				coord_over_z += coord_over_z_x_step;
				one_over_z += one_over_z_x_step;
				depth += depth_x_step;
			}
		}
	};


}