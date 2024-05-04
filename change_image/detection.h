
#ifndef DETECTION_H
#define DETECTION_H
char rec_rech_s(Uint32* pixels,SDL_PixelFormat* format,
		size_t i,size_t j,size_t h,size_t w,
		size_t *s,size_t *e,size_t *we,int col);

size_t* rech_s(SDL_Surface* surface,char col);

char rec_red(Uint32* pixels,SDL_PixelFormat* format,size_t i,size_t j,
		size_t lon,char angle,size_t h,size_t w);

void surface_to_red(SDL_Surface* surface);

SDL_Surface* create_image(SDL_Surface* surface, 
        size_t width1, size_t height1, size_t width2, size_t height2);

double angle_to_turn(SDL_Surface* surface);

char is_line(Uint32* pixels,size_t i,int black_tol, 
        SDL_PixelFormat* format,size_t H_max,size_t H,size_t W);

char verif_sud(SDL_Surface* surface);

void redelet(Uint32* pixels,size_t W,size_t H,size_t i,size_t j
		,SDL_PixelFormat* format);

void delet(SDL_Surface* surface,size_t i,size_t j);

size_t* get_sudoku(SDL_Surface* surface);

#endif
