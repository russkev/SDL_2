#if defined(__clang__)
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif


#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <algorithm>

#include <fstream>
#include <string>
#include <sstream>

#include "view.hpp"
#include "algorithm.hpp"
#include "canvas.hpp"
#include "rendercontext.hpp"
#include "math.hpp"
#include "fps.hpp"
#include "vertex.hpp"
#include "textures.hpp"
#include "obj.hpp"

float rot_counter = 0;

void draw_animation_frame (SDL_Surface& s_surface, double s_absolute_time, double s_delta_time, const glm::mat4& projection, graphics::fps& frame_monitor, const graphics::obj& mesh) {
    using namespace graphics;
    typedef u8vec4 bgra_color_type;
    typedef vec4 point_type;
    typedef view2d<bgra_color_type> view_type;
	typedef vec2 coord_type;

	// // Define centre of screen
    auto s_center = glm::ivec2 (s_surface.w, s_surface.h) / 2;

	// // Setup canvas and view
    view_type s_view (s_surface.pixels, s_surface.w, s_surface.h);
    canvas<view_type, point_type> s_canvas (s_view);  

	// // Setup colour of shape
	s_canvas.stroke_color(bgra_color_type(0, 0, 255, 255));

	// // Create a texture
	xor_texture s_texture(256, 256);

	renderContext<view_type> s_render (s_view);
	rot_counter+= float(s_delta_time);

	// // Setup triangle transform
	mat4 translation = translate(mat4(), vec3(0.0f, 0.0f, 5.0f - 1 * std::sin(SDL_GetTicks() / 1000.0f)));
	mat4 rotation    = rotate(mat4(), rot_counter, vec3(0.0f, 1.0f, 1.0f));
	mat4 scale_a     = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
	mat4 transform   = /*projection**/(translation*rotation*scale_a);


	// // Render obj
	s_render.clear_depth_buffer();
	s_render.draw_mesh(mesh, projection, transform, s_texture);


	// // TEST // //

	// // END TEST // // 

	// // FPS counter
	frame_monitor.think(s_delta_time);


}

int main (int, char**) try {
	using namespace graphics;
    SDL_Init (SDL_INIT_EVERYTHING);
    std::atexit (&SDL_Quit);
	typedef vec4 point_type;
	typedef u8vec4 bgra_color_type;

	// // TEST // //

	// // END TEST // // 

    auto s_window = SDL_CreateWindow ("Pretty little lines", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_ALLOW_HIGHDPI);
    auto s_surface = SDL_GetWindowSurface (s_window);

	mat4 projection = perspective<float>(deg_to_rad<float>(90.0f), float(s_surface[0].w) / s_surface[0].h, -1.0f, -1000.0f);

    // High precision clock interval
    static const auto s_freq_multiplier = 1.0 / SDL_GetPerformanceFrequency (); 

    // Initial time in clock ticks
    auto s_time0 = s_freq_multiplier * SDL_GetPerformanceCounter ();
    auto s_time1 = s_time0;

	// Initialise fps
	fps frame_check(s_window);

	// // Load an obj
	obj obj_1 = "sphere_02.obj";


    while (!SDL_QuitRequested ()) {
        SDL_Event s_event;
        if (SDL_PollEvent (&s_event)) {
            continue;
        }

        SDL_FillRect (s_surface, nullptr, 0xFF000000); //0xAARRGGBB
        SDL_LockSurface (s_surface);
        
        s_time1 = s_freq_multiplier * SDL_GetPerformanceCounter ();
        draw_animation_frame (s_surface [0], s_time1, s_time1 - s_time0, projection, frame_check, obj_1);
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
