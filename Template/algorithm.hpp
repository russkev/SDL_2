#pragma once

#include <type_traits>

namespace graphics {

    template <typename _Vtype = float>
    auto pi () {
        return _Vtype (3.14159265359);
    }

    // return the larger value
    template <typename _Atype, typename _Btype>
    const auto& max (const _Atype& a, const _Btype& b) {
        return a > b ? a : b;
    }

    // return the smaller value
    template <typename _Atype, typename _Btype>
    const auto& min (const _Atype& a, const _Btype& b) {
        return a > b ? a : b;
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
    const auto& clamp (const _Xtype& s_value, const _Atype& s_min, const _Btype& s_max) {
        return min (max (s_value, s_min), s_max);
    }

    // Interpolate between two values A and B determined by Q 
    template <typename _Atype, typename _Btype, typename _Qtype>
    auto lerp (const _Atype& a, const _Btype& b, _Qtype q) {
        return std::common_type_t<_Atype, _Btype> (a*(_Qtype (1) - q) + q*b);
    }

    // Blend source element with destination element depending on source element alpha channel
    template <typename _View, typename _Point, typename _Element>
    auto blend_element (_View& s_view, const _Point& s_point, const _Element& s_source) {
        using namespace swizzle;
        auto& s_destination = s_view [s_point];                          // multiplying by 1/255
        s_destination = s_source;
        //s_destination = _Element (lerp (xyz (s_destination), xyz (s_source), w (s_source) * 0.00392156862f), 255);
    }

    // Draw vertical line
    template <typename _View, typename _Point, typename _Length, typename _Color>
    void line_vertical (_View& s_view, const _Point& s_point, const _Length& s_length, const _Color& s_color) {
        for (auto i = 0; i < s_length; ++i) {
            blend_element (s_view, s_point + _Point (0, i), s_color);
        }
    }

    // Draw horizontal line
    template <typename _View, typename _Point, typename _Length, typename _Color>
    void line_horizontal (_View& s_view, const _Point& s_point, const _Length& s_length, const _Color& s_color) {
        for (auto i = 0; i < s_length; ++i) {
            blend_element (s_view, s_point + _Point (i, 0), s_color);
        }
    }

    // Draw a line between s_point0 and s_point1 of color s_color onto view s_view
    template <typename _View, typename _Point, typename _Color>
    auto line (_View& s_view, const _Point& s_point0, const _Point& s_point1, const _Color& s_color) {

        auto s_delta = s_point1 - s_point0;
        if (!s_delta.x && !s_delta.y) return blend_element (s_view, s_point0, s_color);        
        if (!s_delta.y) return line_horizontal (s_view, s_point0, s_delta.x, s_color);
        if (!s_delta.x) return line_vertical (s_view, s_point0, s_delta.y, s_color);

        auto s_error = -1.0f;
        auto s_delta_error = abs (1.0f * s_delta.x / s_delta.y);

        auto y = s_point0.y;
        auto dx = clamp (s_point0.x - s_point1.x, -1, 1);
        for (auto x = s_point0.x; x < s_point1.x; x += dx) {
            blend_element (s_view, _Point (x, y), s_color);
            s_error += s_delta_error;
            if (s_error >= 0.0f) {
                s_error -= 1.0f;
                ++y;
            }
        }
    }
}