#if defined(__clang__)
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif


#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <array>

#include "tvec.hpp"
#include "tmat.hpp"
#include "view.hpp"
#include "algorithm.hpp"
#include "canvas.hpp"
#include "starfield.hpp"
#include "rendercontext.hpp"

void draw_animation_frame (SDL_Surface& s_surface, double s_absolute_time, double s_delta_time) {
    using namespace graphics;
    typedef tvec4<std::uint8_t> bgra_color_type;
    typedef tvec2<float> point_type;
    typedef view2d<bgra_color_type> view_type;

    auto s_center = tvec2<int> (s_surface.w, s_surface.h) / 2;

    view_type s_view (s_surface.pixels, s_surface.w, s_surface.h);
	//same as:
	// view2d<tvec4<std::uint8_t> > s_view... 
    canvas<view_type, point_type> s_canvas (s_view);  

	s_canvas.stroke_color(bgra_color_type(0, 0, 255, 255));

	s_canvas.point_to_abs(tvec2<float>(1.0f, 1.0f));

	renderContext<view_type> s_render (s_view);
	point_type min_y_vert(100, 100);
	point_type mid_y_vert(0, 200);
	point_type max_y_vert(80, 300);

	s_render.fill_triangle(max_y_vert, mid_y_vert, min_y_vert);

	point_type temp = mid_y_vert - min_y_vert;
}

int main (int, char**) try {
	using namespace graphics;
    SDL_Init (SDL_INIT_EVERYTHING);
    std::atexit (&SDL_Quit);

	// // TEST // //
	std::array<std::array<int, 4>, 4> matrix_a = {
		{1,2,3,4},
		{4,5,6,7},
		{8,9,0,1},
		{2,3,4,5}
	};


	tmat<float> matrix_test;
	matrix_test.init_identity();

	// // END TEST // // 


    auto s_window = SDL_CreateWindow ("Pretty little lines", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_ALLOW_HIGHDPI);
    auto s_surface = SDL_GetWindowSurface (s_window);

    // High precision clock interval
    static const auto s_freq_multiplier = 1.0 / SDL_GetPerformanceFrequency (); 

    // Initial time in clock ticks
    auto s_time0 = s_freq_multiplier * SDL_GetPerformanceCounter ();
    auto s_time1 = s_time0;

    while (!SDL_QuitRequested ()) {
        SDL_Event s_event;
        if (SDL_PollEvent (&s_event)) {
            continue;
        }

        SDL_FillRect (s_surface, nullptr, 0xFF000000); //0xAARRGGBB
        SDL_LockSurface (s_surface);
        
        s_time1 = s_freq_multiplier * SDL_GetPerformanceCounter ();
        draw_animation_frame (s_surface [0], s_time1, s_time1 - s_time0);
        s_time0 = s_time1;

        SDL_UnlockSurface (s_surface);
        SDL_UpdateWindowSurface (s_window);
    }

    SDL_DestroyWindow (s_window);
    return 0;
}
catch (const std::exception& ex) {
    std::cout << "Uncaught exception : " << typeid (ex).name () << "\n";
    std::cout << ex.what () << "\n";

#ifdef _MSC_VER
    __debugbreak ();
#endif
}
