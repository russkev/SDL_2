#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include "tvec.hpp"

namespace graphics {
    

    template <typename _Element>
    struct view1d {        
        typedef _Element element_type;

        view1d (void* s_elements, std::size_t s_length):
            view1d ((element_type*)s_elements, s_length)
        {}

        view1d (element_type* s_elements, std::size_t s_length):
            m_elements (s_elements),
            m_length (s_length)
        {}

        auto&& operator [] (std::size_t i) const {
            if (i >= m_length) {
                throw std::out_of_range (__FUNCSIG__);                
            }
            return m_elements [i];
        }

        auto width () const { return m_length; }

    private:
        element_type* m_elements;
        std::size_t   m_length;
    };

    template <typename _Element>
    struct view2d {
        typedef _Element element_type;

        view2d (void* s_elements, std::size_t s_horizontal, std::size_t s_vertical):
            view2d ((element_type*)s_elements, s_horizontal, s_vertical)
        {}

        view2d (element_type* s_elements, std::size_t s_horizontal, std::size_t s_vertical):
            m_elements (s_elements),
            m_horizontal (s_horizontal),
            m_vertical (s_vertical)
        {}

        auto operator [] (std::size_t i) const {
            if (i >= m_horizontal) {
                throw std::out_of_range (__FUNCSIG__);                
            } 
            return view1d<_Element> (&m_elements [m_horizontal * i], m_horizontal);
        }

        auto&& operator [] (const tvec2<int>& s_index) const {
            if (s_index.x >= m_horizontal) 
                throw std::out_of_range (__FUNCSIG__);
            if (s_index.y >= m_vertical)   
                throw std::out_of_range (__FUNCSIG__);
            return (*this) [s_index.y] [s_index.x];
        }

        auto height () const { return m_vertical; }
        auto width () const { return m_horizontal; }

    private:
        element_type* m_elements;
        std::size_t m_horizontal, m_vertical;
    };    

}
