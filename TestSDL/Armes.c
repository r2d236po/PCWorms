#include "armes.h"
#include "carte.h"
#include "AffichageGeneral.h"


/* Fonctions concernant la gestion des armes */


//Fait disparaitre un rond d'un certain rayon, aux coordonnés précisées, dans la surface passée en paramètre
void explosion(int x, int y, int rayon, SDL_Surface ** imageMapSurface, SDL_Texture * ptexture)
{

	int i = 0, j = 0, xP = 0, yP = 0, offsetX = 0, offsetY = 0;
	Uint32 pixelTest = SDL_MapRGBA(imageMapSurface[0]->format, 255, 255, 255, 0);
	SDL_Rect  rect = { x - rayon, y - rayon, rayon * 2, rayon * 2 };
	offsetX = x - rayon;
	offsetY = y - rayon;
	for (j = 0; j < 2 * rayon; j++)
	{
		for (i = 0; i < 2 * rayon; i++)
		{
			xP = (rayon - i)*(rayon - i);
			yP = (rayon - j)*(rayon - j);

			if (xP + yP < (rayon*rayon)
				&& i + offsetX >= 0
				&& j + offsetY >= 0
				&& i + offsetX < imageMapSurface[0]->clip_rect.w
				&& j + offsetY < imageMapSurface[0]->clip_rect.h
				)
			{
				DrawPixel(imageMapSurface[0], i + offsetX, j + offsetY, pixelTest);
			}
		}
	}

	if (rect.x < 0) {
		rect.w -= rect.x;
		rect.x = 0;
	}
	if (rect.y < 0) {
		rect.h -= rect.y;
		rect.y = 0;
	}
	if (rect.x + rect.w >= imageMapSurface[0]->clip_rect.w){
		rect.w -= rect.x + rect.w - imageMapSurface[0]->clip_rect.w;
	}
	if (rect.y + rect.h >= imageMapSurface[0]->clip_rect.h){
		rect.h -= rect.y + rect.h - imageMapSurface[0]->clip_rect.h;
	}

	if (updateGlobaleTexture(imageMapSurface, ptexture, 0, &rect) < 0)
	{
		printf("Erreur lors de l'update de la texture.");
	}
}