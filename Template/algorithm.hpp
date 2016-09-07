#pragma once

#include <type_traits>

namespace graphics {

    template <typename _Vtype = float>
    auto pi () {
        return _Vtype (3.14159265359);
    }

    // return the larger value
    template <typename _Atype, typename _Btype>
    auto max (const _Atype& a, const _Btype& b) {
        return a > b ? a : b;
    }

    // return the smaller value
    template <typename _Atype, typename _Btype>
    auto min (const _Atype& a, const _Btype& b) {
        return a < b ? a : b;
    }

    // return absolute value
    template <typename _Atype>
    auto abs (const _Atype& a) {
        return a < 0 ? -a : a;
    }

    // compare and swap if a is greater then b
    template <typename _Vtype>
    bool swap_if_more (_Vtype& a, _Vtype& b) {
        if (a > b) {
            std::swap (a, b);
            return true;
        }
        return false;
    }

    // compare and swap if a is less then b
    template <typename _Vtype>
    bool swap_if_less (_Vtype& a, _Vtype& b) {
        if (a < b) {
            std::swap (a, b);
            return true;
        }
        return false;
    }

    // force s_value into range of s_min and s_max, preventing it from going above or below the limits
    template <typename _Atype, typename _Btype, typename _Xtype>
    auto clamp (const _Xtype& s_value, const _Atype& s_min, const _Btype& s_max) {
        return min (max (s_value, s_min), s_max);
    }

    // Interpolate between two values A and B determined by Q 
    template <typename _Atype, typename _Btype, typename _Qtype>
    auto lerp (const _Atype& a, const _Btype& b, _Qtype q) {
        return std::common_type_t<_Atype, _Btype> (a*(_Qtype (1) - q) + q*b);
    }

    // Blend source element with destination element depending on source element alpha channel
    template <typename _View, typename _Coord>
	auto blend_element (_View& s_view, const tvec2<_Coord>& s_point,
						const typename _View::element_type& s_source)
	{
		typedef typename _View::element_type element_type;
		typedef tvec2<_Coord> point_type;
        using namespace swizzle;
        auto& s_destination = s_view [s_point];                          // multiplying by 1/255
        //s_destination = s_source;
        s_destination = element_type (lerp (xyz (s_destination), xyz (s_source), w (s_source) * 0.00392156862f), 255);
    }

    // Draw vertical line
    template <typename _View, typename _Coord, typename _Length>
    void line_vertical (_View& s_view, const tvec2<_Coord>& s_point,
						const _Length& s_length,
						const typename _View::element_type& s_color)
	{
		typedef tvec2<_Coord> point_type;
        const auto di = clamp (s_length, -1, 1);
        for (auto i = 0; i != s_length; i += di) {
            blend_element (s_view, s_point + point_type (0, i), s_color);
        }
    }

    // Draw horizontal line
    template <typename _View, typename _Coord, typename _Length>
    void line_horizontal (_View& s_view, const tvec2<_Coord>& s_point,
						  const _Length& s_length,
						  const typename _View::element_type& s_color)
	{
		typedef tvec2<_Coord> point_type;
        const auto di = clamp (s_length, -1, 1);
        for (auto i = 0; i != s_length; i += di) {
            blend_element (s_view, s_point + point_type (i, 0), s_color);
        }
    }

	template <typename _Coord0, typename _Coord1,
			  typename _Coord2, typename _Coord3>
	auto clip_line (tvec2<_Coord0>& s_pt0,
					tvec2<_Coord1>& s_pt1,
					const tvec2<_Coord2>& s_min,
					const tvec2<_Coord3>& s_max)
	{
		enum bound_mask_type : uint32_t {
			inside = 0,
			clip_left = 1,
			clip_right = 2,
			clip_bottom = 4,
			clip_top = 8
		};
		
		const auto bound_check = [&s_min, &s_max] (const auto& s_pt) {
			auto s_result = std::uint32_t (inside);
			if (s_pt.x < s_min.x) s_result |= clip_left;
			if (s_pt.x > s_max.x) s_result |= clip_right;
			if (s_pt.y < s_min.y) s_result |= clip_bottom;
			if (s_pt.y > s_max.y) s_result |= clip_top;
			return s_result;
		};
		
		auto s_result0 = bound_check (s_pt0);
		auto s_result1 = bound_check (s_pt1);
		
		while (true) {
			if (!(s_result0 | s_result1)) {
				return true;
			}
			else if (s_result0 & s_result1) {
				return false;
			}
			
			auto s_result = s_result0 ? s_result0 : s_result1 ;
			
			tvec2<std::common_type_t<_Coord0, _Coord1, _Coord2, _Coord3>> s_tmp;
			if (s_result & clip_top) {
				s_tmp.x = s_pt0.x + (s_pt1.x - s_pt0.x) * (s_max.y - s_pt0.y) / (s_pt1.y - s_pt0.y);
				s_tmp.y = s_max.y;
			}
			else if (s_result & clip_bottom) {
				s_tmp.x = s_pt0.x + (s_pt1.x - s_pt0.x) * (s_min.y - s_pt0.y) / (s_pt1.y - s_pt0.y);
				s_tmp.y = s_min.y;
			}
			else if (s_result & clip_right) {
				s_tmp.y = s_pt0.y + (s_pt1.y - s_pt0.y) * (s_max.x - s_pt0.x) / (s_pt1.x - s_pt0.x);
				s_tmp.x = s_max.x;
			}
			else if (s_result & clip_left) {
				s_tmp.y = s_pt0.y + (s_pt1.y - s_pt0.y) * (s_min.x - s_pt0.x) / (s_pt1.x - s_pt0.x);
				s_tmp.x = s_min.x;
			}
			
			if (s_result == s_result0) {
				s_pt0 = s_tmp;
				s_result0 = bound_check (s_pt0);
			}
			else {
				s_pt1 = s_tmp;
				s_result1 = bound_check (s_pt1);
			}
		}
		
	}
	
    // Draw a line between s_point0 and s_point1 of color s_color onto view s_view
    template <typename _View, typename _Coord>
    auto line (_View& s_view,
			   const tvec2<_Coord> s_pt0,
			   const tvec2<_Coord> s_pt1,
			   const typename _View::element_type& s_color)
	{
		using namespace swizzle;
		
		static const auto precision = 1.0;
		
        typedef _Coord coord_type;
		typedef tvec2<_Coord> point_type;
		typedef typename _View::element_type element_type;
		
		auto s_point0 = s_pt0;
		auto s_point1 = s_pt1;
		clip_line (s_point0, s_point1, s_view.max () / 2 - 300, s_view.max () / 2 + 300);
		
		
        auto s_delta = s_point1 - s_point0;
        if (!s_delta.x && !s_delta.y) return blend_element (s_view, s_point0, s_color);        
        if (!s_delta.y) return line_horizontal (s_view, s_point0, s_delta.x, s_color);
        if (!s_delta.x) return line_vertical (s_view, s_point0, s_delta.y, s_color);
        
        if (abs (s_delta.x) > abs (s_delta.y)) {
            const auto dx = clamp (s_delta.x, -1, 1);
            const auto dy = clamp (s_delta.y, -1, 1) * abs (precision * s_delta.y / s_delta.x);
            auto y = precision * s_point0.y;
            for (auto x = s_point0.x; x != s_point1.x; x += dx) {
		
				const auto s = y - floor (y);
				blend_element (s_view, point_type (coord_type (x), coord_type (ceil (y))),
							   element_type (xyz (s_color), w (s_color) * (s)));
				blend_element (s_view, point_type (coord_type (x), coord_type (floor (y))),
							   element_type (xyz (s_color), w (s_color) * (precision - s)));
				
                y += dy;
            }
        }
        else {
            const auto dy = clamp (s_delta.y, -1, 1);
            const auto dx = clamp (s_delta.x, -1, 1) * abs (precision * s_delta.x / s_delta.y);
            auto x = precision * s_point0.x;
            for (auto y = s_point0.y; y != s_point1.y; y += dy) {
				const auto s = x - floor (x);
				blend_element (s_view, point_type (coord_type (floor (x)), coord_type (y)),
							   element_type (xyz (s_color), w (s_color) * (precision - s)));
				blend_element (s_view, point_type (coord_type (ceil (x)), coord_type (y)),
							   element_type (xyz (s_color), w (s_color) * s));
                x += dx;
            }
        }
    }
}
