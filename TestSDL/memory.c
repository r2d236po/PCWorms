#include "memory.h"

/**
* \fn void* my_malloc(size_t n)
* \brief Perform standard malloc with memory leak counting.
*
* \param[in] n, size of the block to allocate.
* \returns allocated block, NULL if error
*/
void* my_malloc(size_t n)
{
	n_malloc++;
	return malloc(n);
}

/**
* \fn void my_free(void* memory)
* \brief Do a free with memory leak counting.
*
* \param[in] memory, memory block to free.
* \returns void
*/
void my_free(void* memory)
{
	n_malloc--;
	free(memory);
}

void decreaseMalloc()
{
	n_malloc--;
}

/**
* \fn SDL_Surface* my_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
* \brief Perform and SDL_CreateRgbSurface with memory leak counting.
*
* \param[in] flags, should be 0.
* \param[in] width, width of the surface to create.
* \param[in] height, height of the surface to create.
* \param[in] depth, depth of the image, number of bits per pixel.
* \param[in] Rmask, red mask value.
* \param[in] Gmask, green mask value.
* \param[in] Bmask, blue mask value.
* \param[in] Amask, alpha mask value.
* \returns NULL on error, or an SDL_Surface
*/
SDL_Surface* my_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	n_surface++;
	return SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask);
}

/**
* \fn SDL_Surface* my_IMG_Load(const char* path)
* \brief Do an IMG_Load with memory leak counting.
*
* \param[in] path, path to the image to load.
* \returns NULL on error, or an SDL_Surface
*/
SDL_Surface* my_IMG_Load(const char* path)
{
	n_surface++;
	return IMG_Load(path);
}

/**
* \fn SDL_Surface* my_RenderText_Blended(TTF_Font *font, char *str, SDL_Color color)
* \brief Perform and TTF_RenderText_Blended with memory leak counting.
*
* \param[in] font, the font used.
* \param[in] str, string to render.
* \param[in] color, color of the text.
* \returns NULL on error, or an SDL_Surface with the text.
*/
SDL_Surface* my_RenderText_Blended(TTF_Font *font, char *str, SDL_Color color)
{
	n_surface++;
	return TTF_RenderText_Blended(font, str, color);
}

/**
* \fn void my_freeSurface(SDL_Surface* surface)
* \brief Do an SDL_FreeSurface with memory leak counting.
*
* \param[in] surface, surface to free.
* \returns void
*/
void my_freeSurface(SDL_Surface* surface)
{
	n_surface--;
	SDL_FreeSurface(surface);
}

/**
* \fn SDL_Texture* my_IMG_LoadTexture(SDL_Renderer* pRenderer,const char* path)
* \brief Do an IMG_LoadTexture with memory leak counting.
*
* \param[in] path, path to the image to load.
* \returns NULL on error, or an SDL_Texture
*/
SDL_Texture* my_IMG_LoadTexture(SDL_Renderer* renderer, const char* path)
{
	n_texture++;
	return IMG_LoadTexture(renderer, path);
}

/**
* \fn SDL_Texture* my_CreateTextureFromSurface(SDL_Renderer* pRenderer, SDL_Surface* surface)
* \brief Do an SDL_CreateTextureFromSurface with memory leak counting.
*
* \param[in] surface, surface to load from.
* \returns NULL on error, or an SDL_Texture
*/
SDL_Texture* my_CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface)
{
	n_texture++;
	return SDL_CreateTextureFromSurface(renderer, surface);
}

/**
* \fn SDL_Texture* my_CreateTextureFromSurface(SDL_Renderer* pRenderer, SDL_Surface* surface)
* \brief Do an SDL_CreateTexture with memory leak counting.
*
* \param[in] renderer, renderer associated with the texture.
* \param[in] format, format of the pixels.
* \param[in] access, type of access of the texture.
* \param[in] w, width of the texture.
* \param[in] h, hight of the texture.
* \returns SDL_CreateTexture
*/
SDL_Texture* my_CreateTexture(SDL_Renderer* renderer,Uint32 format,int access,int w, int h)
{
	n_texture++;
	return SDL_CreateTexture(renderer, format, access, w, h);
}

/**
* \fn void my_freeTexture(SDL_Texture* texture)
* \brief Do an SDL_DestroyTexture with memory leak counting.
*
* \param[in] texture, texture to free.
* \returns void
*/
void my_freeTexture(SDL_Texture* texture)
{
	n_texture--;
	SDL_DestroyTexture(texture);
}


/**
* \fn void memoryLeakCheck()
* \brief check for memory leak in the program.
*
* \returns void
*/
void memoryLeakCheck()
{
	if (n_malloc != 0)
		fprintf(logFile,"memoryLeakCheck : memory leak detected, number of leaks : %d.\n\n", n_malloc);
	if (n_surface != 0)
		fprintf(logFile, "memoryLeakChec : memory leak detected on surfaces, number of leaks : %d.\n\n", n_surface);
	if (n_texture != 0)
		fprintf(logFile, "memoryLeakCheck : memory leak detected on textures, number of leaks : %d.\n\n", n_texture);
	if (n_texture == 0 && n_surface == 0 && n_malloc == 0)
		fprintf(logFile, "memoryLeakCheck : no memory leak detected.\n\n");
}

