#ifndef MEMORY_H
#define MEMORY_H

/*Include Source*/
#include "Libraries.h"

/*Leaking memory variables*/
unsigned int n_malloc;
unsigned int n_surface;
unsigned int n_texture;

/*Leaking memory function*/
void* my_malloc(size_t n);
void my_free(void* memory);
void decreaseMalloc();

SDL_Surface* my_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
SDL_Surface* my_IMG_Load(const char* path);
SDL_Surface* my_RenderText_Blended(TTF_Font *font, char *str, SDL_Color color);
void my_freeSurface(SDL_Surface* surface);

SDL_Texture* my_IMG_LoadTexture(SDL_Renderer* renderer, const char* path);
SDL_Texture* my_CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);
SDL_Texture* my_CreateTexture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h);
void my_freeTexture(SDL_Texture* texture);

/*Check memory leak*/
void memoryLeakCheck();

#endif