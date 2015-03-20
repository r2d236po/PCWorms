#include "armes.h"
#include "carte.h"
#include "AffichageGeneral.h"


/* Fonctions concernant la gestion des armes */


//Fait disparaitre un rond d'un certain rayon, aux coordonnés précisées, dans la surface passée en paramètre
void explosion(int x, int y, int rayon, SDL_Surface * imageMapSurface, SDL_Texture * ptexture)
{

	int i = 0, j = 0, xP = 0, yP = 0, offsetX = 0, offsetY = 0;
	Uint32 pixelTest = SDL_MapRGBA(imageMapSurface->format, 255, 255, 255, 0);
	SDL_Rect  rect = initRect(x - rayon, y - rayon, 2 * rayon, 2 * rayon);
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
				&& i + offsetX < imageMapSurface->clip_rect.w
				&& j + offsetY < imageMapSurface->clip_rect.h
				)
			{
				DrawPixel(imageMapSurface, i + offsetX, j + offsetY, pixelTest);
			}
		}
	}
	secureRect(&rect, imageMapSurface);

	if (updateGlobaleTexture(imageMapSurface, imageMapSurface, ptexture, &rect) < 0)
	{
		printf("Erreur lors de l'update de la texture.");
	}
}