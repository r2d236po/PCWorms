#include "armes.h"
#include "Libraries.h" //Inclus toutes les librairies

/* Fonctions concernant la gestion des armes */

SDL_Surface * bombExplo(int x, int y, int rayon){
	int i = 0, j = 0;
	int xP = 0, yP = 0;

	Uint32 rmask, gmask, bmask, amask;
	Uint32* pixelWrite = NULL;
	SDL_Surface * cercle = NULL;
	Uint32 pixelTest;

	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;

	cercle = SDL_CreateRGBSurface(0, rayon * 2, rayon * 2, 32, rmask, gmask, bmask, amask);
	cercle->clip_rect.x = x - rayon;
	cercle->clip_rect.y = y - rayon;
	pixelTest = SDL_MapRGBA(cercle->format, 255, 255, 255, 0);
	for (j = 0; j < cercle->h; j++)
	{
		for (i = 0; i < cercle->w; i++)
		{
			xP = (rayon - i)*(rayon - i);
			yP = (rayon - j)*(rayon - j);

			if (xP + yP < (rayon*rayon)){
				DrawPixel(cercle, i-1, j-1, pixelTest);
			}
			else DrawPixel(cercle, i, j, SDL_MapRGBA(cercle->format, 0, 0, 0, 255));

		}
	}
	return cercle;
}