#pragma once

#include "algorithm.hpp"
#include "math.hpp"

namespace graphics {
	template <typename _View, typename _Point>
	struct canvas {
		typedef _View view_type;
		typedef _Point point_type;
		typedef tvec2<int> ipoint_type;
		typedef typename _View::element_type color_type;
		typedef typename point_type::value_type value_type;

		canvas (view_type& s_view): 
			m_view (s_view) 
		{
			reset_transform ();
		}

		canvas<_View, _Point>& reset_transform () {
			m_pre_translate = point_type ();
			m_rotate = value_type ();
			m_scale = point_type (1);
			m_post_translate = point_type ();

			return *this;
		}

		canvas<_View, _Point>& pre_translate (const point_type& s_offset) {
			m_pre_translate = m_pre_translate + s_offset;
			return *this;
		}

		canvas<_View, _Point>& post_translate (const point_type& s_offset) {
			m_post_translate = m_post_translate + s_offset;
			return *this;
		}

		canvas<_View, _Point>& scale (const point_type& s_scale) {
			m_scale = m_scale * s_scale;
			return *this;
		}

		canvas<_View, _Point>& rotate (const value_type& s_angle) {
			m_rotate = m_rotate + s_angle;
			return *this;
		}

		canvas<_View, _Point>& stroke_color (const color_type& s_color) { 
			m_color = s_color; 
			return *this; 
		}

		canvas<_View, _Point>& move_to_abs (const point_type& s_pt0) {
			m_beginning = s_pt0;
			m_current = s_pt0;
			return *this;
		}

		canvas<_View, _Point>& curve_to_abs (const point_type& s_pt1, const point_type& s_pt2, const point_type& s_pt3) {
			bezier_curve (m_view, 
				ipoint_type (transform (m_current)),
				ipoint_type (transform (s_pt1)),
				ipoint_type (transform (s_pt2)),
				ipoint_type (transform (s_pt3)),
				m_color);
			m_current = s_pt3;
			return *this;
		}

		canvas<_View, _Point>& line_to_abs (const point_type& s_pt1) {
			line (m_view, 
				ipoint_type (transform (m_current)), 
				ipoint_type (transform (s_pt1)),
				m_color);
			m_current = s_pt1;
			return *this;
		}

		canvas<_View, _Point>& point_to_abs(const point_type& s_pt1) {
			blend_element(m_view,
				ipoint_type(transform(s_pt1)),
				m_color);
			return *this;
		}



		canvas<_View, _Point>& close_path () {
			line (m_view,
				ipoint_type (transform (m_current)), 
				ipoint_type (transform (m_beginning)), 
				m_color);
			m_current = m_beginning;
			return *this;
		}
	private:
		auto transform (const point_type& s_pt0) const {
            /* TODO #3 : transform p here
                1) pre-translate https://en.wikipedia.org/wiki/Translation_(geometry) , though this looks alot more complicated then it actually is
			


                2) rotate https://en.wikipedia.org/wiki/Rotation_matrix
                3) scale https://en.wikipedia.org/wiki/Scaling_(geometry)
                4) post-translate (same as 1)
            */

			auto p = s_pt0 + m_pre_translate;
			p = tvec2<double>(p.x*cos(m_rotate) - p.y*sin(m_rotate), p.x*sin(m_rotate) + p.y*cos(m_rotate));
			p = p * m_scale;
			p = p + m_post_translate;
			return p;
		}

		view_type& m_view;
		color_type m_color;
		point_type m_current;
		point_type m_beginning;

		point_type m_pre_translate;
		value_type m_rotate;
		point_type m_scale;
		point_type m_post_translate;

	};
}