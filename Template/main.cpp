#if defined(__clang__)
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif


#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <sstream>

#include "tvec.hpp"
#include "view.hpp"
#include "algorithm.hpp"
#include "canvas.hpp"

void draw_animation_frame (SDL_Surface& s_surface, double s_absolute_time, double s_delta_time) {
    using namespace graphics;
    typedef tvec4<std::uint8_t> bgra_color_type;
    typedef tvec2<float> point_type;
    typedef view2d<bgra_color_type> view_type;

    auto s_center = tvec2<int> (s_surface.w, s_surface.h) / 2;

    view_type s_view (s_surface.pixels, s_surface.w, s_surface.h);
    canvas<view_type, point_type> s_canvas (s_view);    

    // Pushes the picture so it is centered at (0, 0)
    s_canvas.pre_translate (point_type (-462.4624365f, -272.7413815f));
    // Animate the scale
    s_canvas.scale (point_type (1) - std::sin ((float)s_absolute_time)*0.75f);
    // Scale it further down to half size
    s_canvas.scale (0.5f);
    // Animate rotatation
    s_canvas.rotate (pi<float> () * (float)s_absolute_time * 0.5f);
    // Push the processed picture back to the center of the screen
    s_canvas.post_translate (0.5f*point_type ((float)s_surface.w, (float)s_surface.h));

    // Draw the picture
    s_canvas.stroke_color (bgra_color_type (255, 0, 0, 128));
    s_canvas.move_to_abs  (point_type (409.57131f, 69.491383f));
    s_canvas.curve_to_abs (point_type (434.06031f, 118.46738f), point_type ( 435.94631f, 116.58518f), point_type (435.94631f, 116.58518f));
    s_canvas.line_to_abs  (point_type (462.29006f, 118.08518f));
    s_canvas.line_to_abs  (point_type (488.97756f, 116.58518f));
    s_canvas.curve_to_abs (point_type (488.97756f, 116.58518f), point_type ( 490.86456f, 118.46838f), point_type (515.35256f, 69.491383f));
    s_canvas.curve_to_abs (point_type (505.93256f, 323.79038f), point_type ( 741.40931f, 207.00808f), point_type (641.57131f, 88.335183f));
    s_canvas.curve_to_abs (point_type (995.70831f, 199.47318f), point_type ( 897.75082f, 427.40318f), point_type (709.38381f, 466.96018f));
    s_canvas.curve_to_abs (point_type (780.96581f, 353.93918f), point_type ( 666.05104f, 352.05578f), point_type (609.54006f, 406.67888f));
    s_canvas.curve_to_abs (point_type (547.38006f, 299.30788f), point_type ( 462.29006f, 475.99138f), point_type (462.29006f, 475.99138f));
    s_canvas.curve_to_abs (point_type (462.29006f, 475.99138f), point_type ( 377.54581f, 299.30778f), point_type (315.38381f, 406.67888f));
    s_canvas.curve_to_abs (point_type (258.87281f, 352.04988f), point_type ( 143.95907f, 353.93918f), point_type (215.54006f, 466.96018f));
    s_canvas.curve_to_abs (point_type (27.171063f, 427.40308f), point_type (-70.783437f, 199.47318f), point_type (283.35256f, 88.335183f));
    s_canvas.curve_to_abs (point_type (183.51456f, 207.00708f), point_type ( 418.99131f, 323.78938f), point_type (409.57131f, 69.491383f));
    s_canvas.close_path ();

    // Draw the corsshair
    s_canvas.reset_transform ();
    s_canvas.stroke_color (bgra_color_type (0, 0, 255, 255));
    s_canvas.move_to_abs (s_center);
    s_canvas.line_to_abs (s_center + point_type (+10, 0));
    s_canvas.move_to_abs (s_center);
    s_canvas.line_to_abs (s_center + point_type (-10, 0));
    s_canvas.move_to_abs (s_center);
    s_canvas.line_to_abs (s_center + point_type (0, +10));
    s_canvas.move_to_abs (s_center);
    s_canvas.line_to_abs (s_center + point_type (0, -10));

	// // TEST // // ------------
	tvec2<int> p0(100, 500), p1(600, 200), p2(1000, 400);

	bgra_color_type lineColour = { 0, 0, 0, 255 };

	//line(s_surface, p0, p1, { 0,0,0,255 });
	line(s_surface, p0, p1, lineColour);


	// // END TEST // // --------



}

int main (int, char**) try {
    SDL_Init (SDL_INIT_EVERYTHING);
    std::atexit (&SDL_Quit);


	// // TEST AREA // //-----------------------------
	graphics::tvec2<int> in(50, 30), tl(-20, -20), t(30, -30), tr(550, -50), l(-50, 50), r(550, 50), bl(-50, 550), b(50, 550), br(550, 550), 
		p0(100, 500), p1(600, 200), p2(1000, 400);

	graphics::tvec2<int> min(0, 0);
	graphics::tvec2<int> max(500, 500);

	// // TEST AREA END // //-------------------------


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

        SDL_FillRect (s_surface, nullptr, 0xFFFFFFFF);
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
