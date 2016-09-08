#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include "tvec.hpp"

#if !defined(__clang__)
#ifndef __PRETTY_FUNCTION__
    #define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
#endif

namespace graphics {
    
    template <typename _Element>
    struct view1d {        
        typedef _Element element_type;

        view1d (void* s_elements, std::int64_t s_length):
            view1d ((element_type*)s_elements, s_length)
        {}

        view1d (element_type* s_elements, std::int64_t s_length):
            m_elements (s_elements),
            m_length (s_length)
        {}

        template <typename _Vtype, std::enable_if_t<std::is_integral<_Vtype>::value, int> = 0>
        auto&& operator [] (_Vtype i) const {            
            if (i >= m_length || i < 0) {
                throw std::out_of_range (__PRETTY_FUNCTION__);
            }
            return m_elements [i];
        }

        auto size () const { return max () - min (); }
        auto min () const { return 0; }
        auto max () const { return m_length; }

    private:
        element_type* m_elements;
        std::int64_t m_length;
    };

    template <typename _Element>
    struct view2d {
        typedef _Element element_type;        

        view2d (void* s_elements, std::int64_t s_horizontal, std::int64_t s_vertical):
            view2d ((element_type*)s_elements, s_horizontal, s_vertical)
        {}

        view2d (element_type* s_elements, std::int64_t s_horizontal, std::int64_t s_vertical):
            m_elements (s_elements),
            m_horizontal (s_horizontal),
            m_vertical (s_vertical)
        {}

        template <typename _Vtype, std::enable_if_t<std::is_integral<_Vtype>::value, int> = 0>
        auto operator [] (_Vtype i) const {            
            if (i >= m_vertical || i < 0) {
                throw std::out_of_range (__PRETTY_FUNCTION__);
            } 
            return view1d<_Element> (&m_elements [m_horizontal * i], m_horizontal);
        }

        template <typename _Vtype, std::enable_if_t<std::is_integral<_Vtype>::value, int> = 0>
        auto&& operator [] (const tvec2<_Vtype>& s_index) const {
            return (*this) [s_index.y] [s_index.x];
        }

        auto size () const { return max () - min () ; }
        auto min () const { return tvec2<std::int64_t> (0, 0); }
        auto max () const { return tvec2<std::int64_t> (m_horizontal, m_vertical); }

    private:
        element_type* m_elements;
        std::int64_t m_horizontal, m_vertical;
    };    

}
