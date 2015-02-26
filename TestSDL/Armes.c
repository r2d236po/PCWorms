#include "armes.h"
#include "Libraries.h" //Inclus toutes les librairies
#include "AffichageGeneral.h"

/* Fonctions concernant la gestion des armes */

void * bombExplo(int x, int y, int rayon, SDL_Surface ** imageMapSurface, SDL_Texture * ptexture){
	int i = 0, j = 0, xP = 0, yP = 0, offsetX = 0, offsetY = 0;

	Uint32 pixelTest;
	pixelTest = SDL_MapRGBA(imageMapSurface[0]->format, 255, 255, 255, 0);
	SDL_Rect  rect = { x - rayon, y-rayon, rayon * 2, rayon * 2 };
	offsetX = x - rayon;
	offsetY = y - rayon;
	for (j = 0; j < 2 * rayon; j++)
	{
		for (i = 0; i < 2 * rayon; i++)
		{
			xP = (rayon - i)*(rayon - i);
			yP = (rayon - j)*(rayon - j);

			if (xP + yP < (rayon*rayon)){
				DrawPixel(imageMapSurface[0], i + offsetX, j + offsetY, pixelTest);
			}
		}
	}
	updateGlobaleTexture(imageMapSurface, ptexture, 0, &rect);
}