#if __has_include (<SDL2/SDL.h>)
	#include <SDL2/SDL.h>
#endif

#if __has_include (<SDL.h>)
	#include <SDL.h>
#endif

#include <cstdlib>
#include <iostream>
#include <typeinfo>

#include "tvec.hpp"
#include "view.hpp"
#include "algorithm.hpp"


void draw_animation_frame (SDL_Surface& s_surface, double s_absolute_time, double s_delta_time) {
    using namespace graphics;
    typedef tvec4<std::uint8_t> rgba_color_type;
    typedef tvec2<int> coord_type;
    view2d<rgba_color_type> s_view (s_surface.pixels, s_surface.w, s_surface.h);    
    const auto s_center = coord_type (s_surface.w >> 1, s_surface.h >> 1);
    const auto s_radius = int (min (s_center.x, s_center.y) * 0.75f);
	const auto s_nlines = 128;
    for (auto i = 0; i < s_nlines; ++i) {
        auto fi = i * pi<double> () * 2.0 / s_nlines + s_absolute_time * pi<double> () * (1.0 / 16.0);
        auto pt = coord_type ((int)round (+sin (fi) * s_radius),
                              (int)round (-cos (fi) * s_radius));
        line (s_view, s_center, s_center + pt, rgba_color_type (int (255.0f* i/s_nlines), int (255.0f - 255.0f * i/s_nlines), 0, 255));
    }

}

int main (int, char**) try {
    SDL_Init (SDL_INIT_EVERYTHING);
    std::atexit (&SDL_Quit);

    auto s_window = SDL_CreateWindow ("Pretty little lines", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_ALLOW_HIGHDPI);
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

        SDL_FillRect (s_surface, nullptr, 0xFFFFFF);
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
