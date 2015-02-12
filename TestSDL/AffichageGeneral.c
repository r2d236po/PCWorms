#include <SDL/SDL.h>
#include <stdio.h>
#include "AffichageGeneral.h"

int afficheImage(SDL_Window *pWindow)
{
	//int *x = NULL, *y = NULL;

	SDL_Surface* imageTest = SDL_LoadBMP("../assets/hello.bmp");
	if (imageTest == NULL)   /*test loadBmp image*/
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		return -1;
	};

	//SDL_GetWindowSize(pWindow, x, y);
	SDL_Rect dest = { 1080 / 2 - imageTest->w / 2, 600 / 2 - imageTest->h / 2, 0, 0 };
	SDL_BlitSurface(imageTest, NULL, SDL_GetWindowSurface(pWindow), &dest);
	SDL_UpdateWindowSurface(pWindow);
}