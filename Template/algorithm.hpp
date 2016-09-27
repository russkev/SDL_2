#pragma once

#include "tvec.hpp"
#include <type_traits>
#include <cmath>

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

    // force s_value into range of s_min and s_max, preventing it from going above or below the limits
    template <typename _Atype, typename _Btype, typename _Xtype>
    auto clamp (const _Xtype& s_value, const _Atype& s_min, const _Btype& s_max) {
        return min (max (s_value, s_min), s_max);
    }

    // Interpolate between two values A and B determined by Q 
    // Sometimes this is also refered to as Mix instead of Lerp
    template <typename _Atype, typename _Btype, typename _Qtype>
    auto lerp (const _Atype& a, const _Btype& b, _Qtype q) {
        return std::common_type_t<_Atype, _Btype> (round (a*(_Qtype (1) - q) + q*b));
    }

    // Blend source element with destination element depending on source element alpha channel
    template <typename _View, typename _Coord>
    auto blend_element ( // Takes view, pixel coordinate and color
        _View& s_view, 
        const tvec2<_Coord>& s_point,						
        const typename _View::element_type& s_source)
    {
        typedef typename _View::element_type element_type;
        typedef tvec2<_Coord> point_type;
        using namespace swizzle;
        auto& s_destination = s_view [s_point];                                 // multiplying by 1/255
        s_destination = element_type (lerp (xyz (s_destination), xyz (s_source), w (s_source) * 0.00392156862f), 255);
    }

    // Draw vertical line
    template <typename _View, typename _Coord, typename _Length>
    void line_vertical (
        _View& s_view, const tvec2<_Coord>& s_point,
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
    void line_horizontal (
        _View& s_view, const tvec2<_Coord>& s_point,
        const _Length& s_length,
        const typename _View::element_type& s_color)
    {
        typedef tvec2<_Coord> point_type;
        const auto di = clamp (s_length, -1, 1);
        for (auto i = 0; i != s_length; i += di) {
            blend_element (s_view, s_point + point_type (i, 0), s_color);
        }
    }

	enum {
		pointInside = 0,
		pointTop = 8,
		pointBottom = 4,
		pointLeft = 2,
		pointRight = 1
	};

	template <typename _Coord0, typename _Coord1, typename _Coord2>
	int clip_code(
		tvec2<_Coord0> testCoord, 
		const tvec2<_Coord1>& s_min,
		const tvec2<_Coord2>& s_max) 
	{
		int code = pointInside; // Initilaize to 0000

		if (testCoord.x < s_min.x) code |= pointLeft;
		if (testCoord.x > s_max.x) code |= pointRight;
		if (testCoord.y < s_min.y) code |= pointTop;
		if (testCoord.y > s_max.y) code |= pointBottom;

		return code;
	}

	template <typename _Coord0, typename _Coord1,
              typename _Coord2, typename _Coord3>
   auto clip_line (
        tvec2<_Coord0>& s_pt0,          // In/Out first point of the line
        tvec2<_Coord1>& s_pt1,          // In/Out second point of the line
        const tvec2<_Coord2>& s_min,    // Clip rectangle upper left
        const tvec2<_Coord3>& s_max)    //                lower right
    {
        typedef std::common_type_t<_Coord0, _Coord1, _Coord2, _Coord3> coord_type;

        // TODO #1 : implement clipping algorithm
        // Possible candidate https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

		auto code0 = clip_code(s_pt0, s_min, s_max);
		auto code1 = clip_code(s_pt1, s_min, s_max);

		while (true) {
			if (!(code0 | code1)) {
				return true; // Both ends inside screen so trivially accept
			}
			else if (code0 & code1) {
				return false; // Line completely outside of screen, trivially reject
			}
			else {
				tvec2<float> n_pt;
				int codeOut;


				//Find the first endpoint that lies outside the screen:
				codeOut = code0 ? code0 : code1;

				if (codeOut & pointTop) { //Point is above screen
					n_pt.x = s_pt0.x + ((s_pt1.x - s_pt0.x) / (float(s_pt1.y - s_pt0.y))) * (s_min.y - s_pt0.y);
					// // Other way of doing same thing:
					//n_pt.x = s_pt0.x + (s_pt1.x - s_pt0.x)*(s_min.y - s_pt0.y) / (s_pt1.y - s_pt0.y);
					n_pt.y = s_min.y;
				}
				else if (codeOut & pointBottom) { //Point is below screen
					n_pt.x = s_pt0.x + ((s_pt1.x - s_pt0.x) / (float(s_pt1.y - s_pt0.y))) * (s_max.y - s_pt0.y);
					n_pt.y = s_max.y;
				}
				else if (codeOut & pointLeft) { //Point is left of screen
					n_pt.y = s_pt0.y + ((s_pt1.y - s_pt0.y) / float(s_pt1.x - s_pt0.x)) * (s_min.x - s_pt0.x);
					n_pt.x = s_min.x;
				}
				else if (codeOut & pointRight) { //Point is right of screen
					n_pt.y = s_pt0.y + ((s_pt1.y - s_pt0.y) / (float(s_pt1.x - s_pt0.x))) * (s_max.x - s_pt0.x);
					n_pt.x = s_max.x;
				}
				if (codeOut == code0) {
					s_pt0.x = n_pt.x;
					s_pt0.y = n_pt.y;
					code0 = clip_code(s_pt0, s_min, s_max);
				}
				else {
					s_pt1.x = n_pt.x;
					s_pt1.y = n_pt.y;
					code1 = clip_code(s_pt1, s_min, s_max);
				}
			}
		}


        return false; // Returns true if line intersects clip rect , false otherwise
    }
    
    // Draw a line between s_point0 and s_point1 of color s_color onto view s_view
    template <typename _View, typename _Coord0, typename _Coord1>
    auto line (_View& s_view, 
        tvec2<_Coord0> s_pt0, 
        tvec2<_Coord1> s_pt1,
        const typename _View::element_type& s_color)
    {
        using namespace swizzle;
        
        static const auto one = 1.0f;
        
        typedef std::common_type_t<_Coord0, _Coord1> coord_type;
        typedef tvec2<coord_type> point_type;
        typedef typename _View::element_type element_type;
        typedef typename element_type::value_type color_type;
        
        if (!clip_line (s_pt0, s_pt1, s_view.min (), s_view.max () - 1))
            return;
                
        auto s_delta = s_pt1 - s_pt0;
        if (!s_delta.x && !s_delta.y) return blend_element (s_view, s_pt0, s_color);        
        if (!s_delta.y) return line_horizontal (s_view, s_pt0, s_delta.x, s_color);
        if (!s_delta.x) return line_vertical (s_view, s_pt0, s_delta.y, s_color);
        
        if (abs (s_delta.x) > abs (s_delta.y)) {
            const auto dx = clamp (s_delta.x, -1, 1);
            const auto dy = clamp (s_delta.y, -1, 1) * abs (one * s_delta.y / s_delta.x);
            auto y = one * s_pt0.y;
            for (auto x = s_pt0.x; x != s_pt1.x; x += dx) {		
                const auto s = y - floor (y);
                blend_element (s_view, point_type (coord_type (x), coord_type (ceil (y))),
                               element_type (xyz (s_color), color_type (w (s_color) * (s))));
                blend_element (s_view, point_type (coord_type (x), coord_type (floor (y))),
                               element_type (xyz (s_color), color_type (w (s_color) * (one - s))));
                y += dy;
            }
        }
        else {
            const auto dy = clamp (s_delta.y, -1, 1);
            const auto dx = clamp (s_delta.x, -1, 1) * abs (one * s_delta.x / s_delta.y);
            auto x = one * s_pt0.x;
            for (auto y = s_pt0.y; y != s_pt1.y; y += dy) {
                const auto s = x - floor (x);
                blend_element (s_view, point_type (coord_type (ceil (x)), coord_type (y)),
                               element_type (xyz (s_color), color_type (w (s_color) * s)));
                blend_element (s_view, point_type (coord_type (floor (x)), coord_type (y)),
                               element_type (xyz (s_color), color_type (w (s_color) * (one - s))));
                x += dx;
            }
        }
    }

    // Quadric bezier curve
    template <typename _View, typename _Coord0, typename _Coord1, typename _Coord2>              
    void bezier_curve (_View& s_view, 
        const tvec2<_Coord0>& s_pt0, 
        const tvec2<_Coord1>& s_pt1, 
        const tvec2<_Coord2>& s_pt2,       
        const typename _View::element_type s_color) 
    {
        // TODO (optional) : implement quadric bezier drawing
        // can start by looking at https://en.wikipedia.org/wiki/B%C3%A9zier_curve
		tvec2<int> startCoord = s_pt0, endCoord = s_pt0;

		for (float t = 0; t <= 1; t = t + 0.3333) {
			endCoord.x = s_pt0.x*((1 - t)*(1 - t)) + 2 * (1 - t)*t*s_pt1.x + t*t*s_pt2.x;
			endCoord.y = s_pt0.y*((1 - t)*(1 - t)) + 2 * (1 - t)*t*s_pt1.y + t*t*s_pt2.y;
			line(s_view, startCoord, endCoord, s_color);
			startCoord = endCoord;
			
		}
    }

	// Check if curve is flat
	template <typename _Coord0, typename _Coord1, typename _Coord2, typename _Coord4>
	bool curve_is_flat(
		const tvec2<_Coord0>& s_pt0,
		const tvec2<_Coord1>& s_pt1,
		const tvec2<_Coord2>& s_pt2,
		const tvec2<_Coord4>& s_pt3) 
	{
		tvec2<int> d;
		float d2, d3, d4, distance_tolerance = 1;

		d = s_pt3 - s_pt0;

		d2 = fabs(((s_pt1.x - s_pt3.x) * d.y - (s_pt1.y - s_pt3.y) * d.x));
		d3 = fabs(((s_pt2.x - s_pt3.x) * d.y - (s_pt2.y - s_pt3.y) * d.x));

		d4 = (d2 + d3)*(d2 + d3);
		distance_tolerance = distance_tolerance * (d.x*d.x + d.y*d.y);

		if (d4 < distance_tolerance) {
			return true;
		}
		return false;

	}

	// Check if points are near
	template <typename _Coord0, typename _Coord1>
	bool points_are_near(
		const tvec2<_Coord0>& s_pt0,
		const tvec2<_Coord1>& s_pt1)
	{
		float tolerance = 1.5;
		return (fabs(s_pt0.x - s_pt1.x) < tolerance && fabs(s_pt0.y - s_pt1.y) < tolerance);
	}

	// Draw crosshair
	template <typename _View, typename _Coord0>
	void crosshair(
		_View& s_view,
		const tvec2<_Coord0>& s_pt0,
		const typename _View::element_type s_color)
	{
		int size = 5;
		line(s_view, tvec2<int>(s_pt0.x + size, s_pt0.y), tvec2<int>(s_pt0.x - size, s_pt0.y), s_color);
		line(s_view, tvec2<int>(s_pt0.x, s_pt0.y + size), tvec2<int>(s_pt0.x, s_pt0.y - size), s_color);
	}
	int count = 0;
    // Cubic bezier curve
    template <typename _View, typename _Coord0, typename _Coord1, typename _Coord2>
    void bezier_curve (_View& s_view,
        const tvec2<_Coord0>& s_pt0,
        const tvec2<_Coord1>& s_pt1,
        const tvec2<_Coord2>& s_pt2,
        const tvec2<_Coord2>& s_pt3,
        const typename _View::element_type s_color) 
    {
        // TODO #2 : implement cubic bezier drawing
        // can start by looking at https://en.wikipedia.org/wiki/B%C3%A9zier_curve
		//tvec2<_Coord0> s_pt01, s_pt12, s_pt23, s_pt012, s_pt123, s_pt0123;

		tvec4<std::uint8_t> colRed(0, 0, 255, 255);
		tvec4<std::uint8_t> colPurple(127, 0, 127, 255);
		tvec4<std::uint8_t> colBlue(255, 0, 0, 255);
		tvec4<std::uint8_t> colAqua(127, 127, 0, 255);

		++count;

		auto s_pt01 = s_pt0 + (s_pt1 - s_pt0) * 0.5f;
		auto s_pt12 = s_pt1 + (s_pt2 - s_pt1) * 0.5f;
		auto s_pt23 = s_pt2 + (s_pt3 - s_pt2) * 0.5f;
		
		auto s_pt012 = s_pt01 + (s_pt12 - s_pt01) * 0.5f;
		auto s_pt123 = s_pt12 + (s_pt23 - s_pt12) * 0.5f;
		auto s_pt0123 = s_pt012 + (s_pt123 - s_pt012) * 0.5f;

		if (curve_is_flat(s_pt0, s_pt1, s_pt2, s_pt3) || 
			(points_are_near(s_pt0, s_pt1) && points_are_near(s_pt1, s_pt2) && points_are_near(s_pt2, s_pt3))) //Prevent stack overflow
		{
			line(s_view, tvec2<int>(s_pt0), tvec2<int>(s_pt3), s_color);
			//crosshair(s_view, s_pt0, colRed);
			//crosshair(s_view, s_pt3, colRed);
		}
		else {
			bezier_curve(s_view, tvec2<int>(s_pt0), tvec2<int>(s_pt01), tvec2<int>(s_pt012), tvec2<int>(s_pt0123), s_color);
			bezier_curve(s_view, tvec2<int>(s_pt0123), tvec2<int>(s_pt123), tvec2<int>(s_pt23), tvec2<int>(s_pt3), s_color);
		}

		return;
    }
}
