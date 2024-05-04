#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "draw_grid.h"

// - Draw the character in the surface
void draw_all_digit(SDL_Surface* surface, size_t x, size_t y, 
        size_t w, size_t h, int grid[9][9], int result[9][9])
{
    TTF_Init();
	TTF_Font *font = TTF_OpenFont("sdl_files/Lato-Heavy.ttf", w / 10);
    
    if (font == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    SDL_Color color = {255,0,0,42};
    char digit[2] = "  ";

    for (size_t row = 0; row < 9; row++)
    {
        for (size_t col = 0; col < 9; col++)
        {
            if (grid[row][col] == 0)
            {
                digit[1] = result[row][col] + '0';
                SDL_Surface* surf_digit = 
                    TTF_RenderText_Solid(font, digit, color);
                SDL_Rect msg_rect;
                msg_rect.x = col * w / 9 + x;
                msg_rect.y = row * h / 9 + y;
                msg_rect.w = w;
                msg_rect.h = h;

                SDL_BlitSurface(surf_digit, NULL, surface, &msg_rect);
                SDL_FreeSurface(surf_digit);
            }
        }
    }

    TTF_CloseFont(font);
}
