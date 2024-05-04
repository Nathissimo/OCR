#ifndef DRAW_GRID_H
#define DRAW_GRID_H

// - Draw the character in the surface
void draw_all_digit(SDL_Surface* surface, size_t x, size_t y, 
        size_t w, size_t h, int grid[9][9], int result[9][9]);

#endif
