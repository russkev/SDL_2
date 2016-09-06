#pragma once

#include <type_traits>

namespace graphics {

    template <typename T>
    struct tvec2 {

        #pragma warning (push)
        #pragma warning (disable : 4244)
        template <typename I>
        tvec2 (const tvec2<I>& a): 
        tvec2 (static_cast<I>(a.x), 
               static_cast<I>(a.y))
        {}
        #pragma warning (pop)

        tvec2 (T x, T y): x (x), y (y) {}        
        tvec2 (T x): tvec2 (x, x) {}
        tvec2 (): tvec2 (0) {}

        union {
            struct { T x, y; };
            struct { T g, a; }; // gray and alpha
        };
    };

    template <typename T0, typename T1> auto operator + (const tvec2<T1>& a, const tvec2<T0>& b) { return tvec2<std::common_type_t<T0, T1>> (a.x + b.x, a.y + b.y); }
    template <typename T0, typename T1> auto operator - (const tvec2<T1>& a, const tvec2<T0>& b) { return tvec2<std::common_type_t<T0, T1>> (a.x - b.x, a.y - b.y); }
    template <typename T0, typename T1> auto operator / (const tvec2<T1>& a, const tvec2<T0>& b) { return tvec2<std::common_type_t<T0, T1>> (a.x / b.x, a.y / b.y); }
    template <typename T0, typename T1> auto operator * (const tvec2<T1>& a, const tvec2<T0>& b) { return tvec2<std::common_type_t<T0, T1>> (a.x * b.x, a.y * b.y); }
    template <typename T0, typename T1> auto operator + (const tvec2<T1>& a, T0 b) { return tvec2<std::common_type_t<T0, T1>> (a.x + b, a.y + b); }
    template <typename T0, typename T1> auto operator - (const tvec2<T1>& a, T0 b) { return tvec2<std::common_type_t<T0, T1>> (a.x - b, a.y - b); }
    template <typename T0, typename T1> auto operator * (const tvec2<T1>& a, T0 b) { return tvec2<std::common_type_t<T0, T1>> (a.x * b, a.y * b); }
    template <typename T0, typename T1> auto operator / (const tvec2<T1>& a, T0 b) { return tvec2<std::common_type_t<T0, T1>> (a.x / b, a.y / b); }
    template <typename T0, typename T1> auto operator + (T1 a, const tvec2<T0>& b) { return tvec2<std::common_type_t<T0, T1>> (a + b.x, a + b.y); }
    template <typename T0, typename T1> auto operator - (T1 a, const tvec2<T0>& b) { return tvec2<std::common_type_t<T0, T1>> (a - b.x, a - b.y); }
    template <typename T0, typename T1> auto operator / (T1 a, const tvec2<T0>& b) { return tvec2<std::common_type_t<T0, T1>> (a / b.x, a / b.y); }
    template <typename T0, typename T1> auto operator * (T1 a, const tvec2<T0>& b) { return tvec2<std::common_type_t<T0, T1>> (a * b.x, a * b.y); }
    template <typename T0> auto operator - (const tvec2<T0>& a) { return tvec2<T0> (-a.x, -a.y); }
    template <typename T0> auto operator + (const tvec2<T0>& a) { return a; }

    template <typename T>
    struct tvec3 {
        #pragma warning (push)
        #pragma warning (disable : 4244)
        template <typename I>
        tvec3 (const tvec3<I>& a): 
        tvec3 (static_cast<I>(a.x), 
               static_cast<I>(a.y), 
               static_cast<I>(a.z))
        {}
        #pragma warning (pop)

        tvec3 (const tvec2<T>& p, T z): tvec3 (p.x, p.y, z) {}
        tvec3 (T x, const tvec2<T>& p): tvec3 (x, p.x, p.y) {}

        tvec3 (T x, T y, T z): x (x), y (y), z (z) {}        
        tvec3 (T x, T y): tvec3 (x, y, 0) {}
        tvec3 (T x): tvec3 (x, x, x) {}
        tvec3 (): tvec3 (0) {}

        union {
            struct { T x, y, z; };
            struct { T r, g, b; }; // red green blue
        };
    };

    template <typename T0, typename T1> auto operator + (const tvec3<T0>& a, const tvec3<T1>& b) { return tvec3<std::common_type_t<T0, T1>> (a.x + b.x, a.y + b.y, a.z + b.z); }
    template <typename T0, typename T1> auto operator - (const tvec3<T0>& a, const tvec3<T1>& b) { return tvec3<std::common_type_t<T0, T1>> (a.x - b.x, a.y - b.y, a.z - b.z); }
    template <typename T0, typename T1> auto operator / (const tvec3<T0>& a, const tvec3<T1>& b) { return tvec3<std::common_type_t<T0, T1>> (a.x / b.x, a.y / b.y, a.z / b.z); }
    template <typename T0, typename T1> auto operator * (const tvec3<T0>& a, const tvec3<T1>& b) { return tvec3<std::common_type_t<T0, T1>> (a.x * b.x, a.y * b.y, a.z * b.z); }
    template <typename T0, typename T1> auto operator + (const tvec3<T0>& a, T1 b) { return tvec3<std::common_type_t<T0, T1>> (a.x + b, a.y + b, a.z + b); }
    template <typename T0, typename T1> auto operator - (const tvec3<T0>& a, T1 b) { return tvec3<std::common_type_t<T0, T1>> (a.x - b, a.y - b, a.z - b); }
    template <typename T0, typename T1> auto operator * (const tvec3<T0>& a, T1 b) { return tvec3<std::common_type_t<T0, T1>> (a.x * b, a.y * b, a.z * b); }
    template <typename T0, typename T1> auto operator / (const tvec3<T0>& a, T1 b) { return tvec3<std::common_type_t<T0, T1>> (a.x / b, a.y / b, a.z / b); }
    template <typename T0, typename T1> auto operator + (T0 a, const tvec3<T1>& b) { return tvec3<std::common_type_t<T0, T1>> (a + b.x, a + b.y, a + b.z); }
    template <typename T0, typename T1> auto operator - (T0 a, const tvec3<T1>& b) { return tvec3<std::common_type_t<T0, T1>> (a - b.x, a - b.y, a - b.z); }
    template <typename T0, typename T1> auto operator * (T0 a, const tvec3<T1>& b) { return tvec3<std::common_type_t<T0, T1>> (a * b.x, a * b.y, a * b.z); }
    template <typename T0, typename T1> auto operator / (T0 a, const tvec3<T1>& b) { return tvec3<std::common_type_t<T0, T1>> (a / b.x, a / b.y, a / b.z); }
    template <typename T0> auto operator - (const tvec3<T0>& a) { return tvec3<T0> (-a.x, -a.y, -a.z); }
    template <typename T0> auto operator + (const tvec3<T0>& a) { return a; }

    template <typename T>
    struct tvec4 {
        // Type conversion/casting constructor
        #pragma warning (push)
        #pragma warning (disable : 4244)
        template <typename I>
        tvec4 (const tvec4<I>& a): 
        tvec4 (static_cast<I>(a.x), 
               static_cast<I>(a.y), 
               static_cast<I>(a.z), 
               static_cast<I>(a.w)) 
        {}
        #pragma warning (pop)

        tvec4 (const tvec3<T>& p, T w): tvec4 (p.x, p.y, p.z, w) {}
        tvec4 (T x, const tvec3<T>& p): tvec4 (x, p.x, p.y, p.z) {}
        tvec4 (const tvec2<T>& p, T z, T w): tvec4 (p.x, p.y, z, w) {}
        tvec4 (T x, const tvec2<T>& p, T w): tvec4 (x, p.x, p.y, w) {}
        tvec4 (T x, T y, const tvec2<T>& p): tvec4 (x, y, p.x, p.y) {}
        tvec4 (const tvec2<T>& p, const tvec2<T>& q): tvec4 (p.x, p.y, q.x, q.y) {}

        tvec4 (T x, T y, T z, T w): x (x), y (y), z (z), w (w) {}
        tvec4 (T x, T y, T z): tvec4 (x, y, z, 0) {}
        tvec4 (T x, T y): tvec4 (x, y, 0) {}
        tvec4 (T x): tvec4 (x, x, x, x) {}
        tvec4 (): tvec4 (0) {}

        union {
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };            
        };
    };

    template <typename T0, typename T1> auto operator + (const tvec4<T0>& a, const tvec4<T1>& b) { return tvec4<std::common_type_t<T0, T1>> (a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
    template <typename T0, typename T1> auto operator - (const tvec4<T0>& a, const tvec4<T1>& b) { return tvec4<std::common_type_t<T0, T1>> (a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
    template <typename T0, typename T1> auto operator / (const tvec4<T0>& a, const tvec4<T1>& b) { return tvec4<std::common_type_t<T0, T1>> (a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
    template <typename T0, typename T1> auto operator * (const tvec4<T0>& a, const tvec4<T1>& b) { return tvec4<std::common_type_t<T0, T1>> (a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }

    template <typename T0, typename T1> auto operator + (const tvec4<T0>& a, T1 b) { return tvec4<std::common_type_t<T0, T1>> (a.x + b, a.y + b, a.z + b, a.w + b); }
    template <typename T0, typename T1> auto operator - (const tvec4<T0>& a, T1 b) { return tvec4<std::common_type_t<T0, T1>> (a.x - b, a.y - b, a.z - b, a.w - b); }
    template <typename T0, typename T1> auto operator * (const tvec4<T0>& a, T1 b) { return tvec4<std::common_type_t<T0, T1>> (a.x * b, a.y * b, a.z * b, a.w * b); }
    template <typename T0, typename T1> auto operator / (const tvec4<T0>& a, T1 b) { return tvec4<std::common_type_t<T0, T1>> (a.x / b, a.y / b, a.z / b, a.w / b); }

    template <typename T0, typename T1> auto operator + (T0 a, const tvec4<T1>& b) { return tvec4<std::common_type_t<T0, T1>> (a + b.x, a + b.y, a + b.z, a + b.w); }
    template <typename T0, typename T1> auto operator - (T0 a, const tvec4<T1>& b) { return tvec4<std::common_type_t<T0, T1>> (a - b.x, a - b.y, a - b.z, a - b.w); }
    template <typename T0, typename T1> auto operator * (T0 a, const tvec4<T1>& b) { return tvec4<std::common_type_t<T0, T1>> (a * b.x, a * b.y, a * b.z, a * b.w); }
    template <typename T0, typename T1> auto operator / (T0 a, const tvec4<T1>& b) { return tvec4<std::common_type_t<T0, T1>> (a / b.x, a / b.y, a / b.z, a / b.w); }

    template <typename T0> auto operator - (const tvec4<T0>& a) { return tvec4<T0> (-a.x, -a.y, -a.z, -a.w); }
    template <typename T0> auto operator + (const tvec4<T0>& a) { return a; }

    namespace swizzle {
        template <typename T> auto x (const tvec4<T>& p) { return p.x; }
        template <typename T> auto y (const tvec4<T>& p) { return p.y; }
        template <typename T> auto z (const tvec4<T>& p) { return p.z; }
        template <typename T> auto w (const tvec4<T>& p) { return p.w; }

        template <typename T> auto x (const tvec3<T>& p) { return p.x; }
        template <typename T> auto y (const tvec3<T>& p) { return p.y; }
        template <typename T> auto z (const tvec3<T>& p) { return p.z; }

        template <typename T> auto x (const tvec2<T>& p) { return p.x; }
        template <typename T> auto y (const tvec2<T>& p) { return p.y; }    

    /**********************************************************************/

        template <typename T> auto xyzw (const tvec4<T>& p) { return p; }
        template <typename T> auto xyz  (const tvec4<T>& p) { return tvec3<T> (p.x, p.y, p.z); }
        template <typename T> auto xy   (const tvec4<T>& p) { return tvec2<T> (p.x, p.y); }

        template <typename T> auto xyz  (const tvec3<T>& p) { return p; }
        template <typename T> auto xy   (const tvec3<T>& p) { return tvec2<T> (p.x, p.y); }

    }

}
