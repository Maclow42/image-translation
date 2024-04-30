#pragma once
#include <SDL2/SDL_surface.h>
#include "matrix.h"

typedef struct 
{
    matrix *input;
    matrix *output;

}datas;

SDL_Surface* load_image(char* path);

datas *get_imgList(char* dirpath, size_t size);

matrix *imageToMatrix(char* path);

matrix *LoadFromSurface(SDL_Surface** surface, size_t n);
