#include <SDL/SDL.h>
#include <stdio.h>
#include "AffichageGeneral.h"

void afficheImage(SDL_Window *pWindow)
{
	int *x = NULL, *y = NULL;
	SDL_Event event;
	pWindow = NULL;

	SDL_Surface* imageTest = SDL_LoadBMP("G:\Simon\Documents\cours\Projet C\TestSDL\assets\hello.bmp");
	if (imageTest == NULL)   /*test load image*/
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		return -1;
	};
	SDL_GetWindowSize(pWindow, x, y);
	SDL_Rect dest = { *x / 2 - imageTest->w / 2, *y / 2 - imageTest->h / 2, 0, 0 };
	SDL_BlitSurface(imageTest, NULL, SDL_GetWindowSurface(pWindow), &dest);
}