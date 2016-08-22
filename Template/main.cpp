#include <iostream>
#include <tuple>
#include <algorithm>
#include <SDL.h>


void Stamp (int x, int y, std::uint32_t color, int size, SDL_Surface* surface) {
    SDL_Rect s_paint;
    s_paint.w = size/2;
    s_paint.h = size/2;
    s_paint.x = x - s_paint.w;
    s_paint.y = y - s_paint.h;
    SDL_FillRect (surface, &s_paint, color);
}

void Paint (int x0, int y0, int x1, int y1, std::uint32_t color, int size, SDL_Surface* surface) {
    static const auto precision = 2048;

    const auto tx = x1 - x0;
    const auto ty = y1 - y0;

    auto dx = 0;
    auto dy = 0;
    auto tt = 0;

    if (std::abs (tx) >= std::abs (ty)) {
        tt = std::abs (tx);
        dx = (tx / tt) * precision;
        dy = ty * precision / std::abs (tx);
    }
    else {
        tt = std::abs (ty);
        dy = (ty / tt) * precision;
        dx = tx * precision / std::abs (ty);
    }

    auto x = 0;
    auto y = 0;
    for (auto i = 0; i < tt; ++i) {
        Stamp (x0 + x / precision, 
               y0 + y / precision, 
               color, 
               size, 
               surface);
        x += dx;
        y += dy;
    }
}

int main (int, char**) {
    SDL_Init (SDL_INIT_EVERYTHING);
    std::atexit (&SDL_Quit);

    auto s_window = SDL_CreateWindow ("Fuck me", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280u, 720u, 0u);
    auto s_surface = SDL_GetWindowSurface (s_window);
    SDL_FillRect (s_surface, nullptr, 0xFFFFFFFF);

    SDL_Event s_event;
    auto s_last_x = 0;
    auto s_last_y = 0;
    auto s_size = 0;

    while (!SDL_QuitRequested ()) {
        if (SDL_PollEvent (&s_event)) {

            switch (s_event.type) {
            case SDL_MOUSEMOTION:
                if (s_event.motion.state & SDL_BUTTON_LMASK) {
                    SDL_LockSurface (s_surface);
                    Paint (s_last_x,
                           s_last_y,
                           s_event.motion.x,
                           s_event.motion.y,
                           0xFFFF0000,
                           s_size,
                           s_surface);
                    SDL_UnlockSurface (s_surface);
                }
                s_last_x = s_event.motion.x;
                s_last_y = s_event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (s_event.button.button == SDL_BUTTON_RIGHT) {
                    SDL_FillRect (s_surface, nullptr, 0xFFFFFFFF);                    
                }
                break;
            case SDL_MOUSEWHEEL:
                s_size = std::max (1, std::min (100, s_size + s_event.wheel.y));
                std::cout << "Brush size : " << s_size << "\n";
                break;
            }

            continue;
        }

        SDL_UpdateWindowSurface (s_window);        
    }

    SDL_DestroyWindow (s_window);
    return 0;
}