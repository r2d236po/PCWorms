#include "armes.h"
#include "carte.h"
#include "my_stdrFct.h"
#include "AffichageGeneral.h"


/* Fonctions concernant la gestion des armes */

/**
* \fn void explosion(int x, int y, int rayon, SDL_Surface * pSurfaceMap, SDL_Texture * pTextureDisplay)
* \brief Supprime une zone de la map de forme circulaire de rayon donnee aux coordonnees voulu.
*
* \param[in] x, coordonnee x du centre du cercle
* \param[in] y, coordonnee y du centre du cercle
* \param[in] rayon, rayon du cercle de la bombe
* \param[in] pSurfaceMap, pointeur vers la surface de la map
* \param[in] pTextureDisplay, pointeur de la texture globale
* \returns void
*/
void explosion(int x, int y, int rayon, SDL_Surface * pSurfaceMap, SDL_Texture * pTextureDisplay)
{

	int i = 0, j = 0, xP = 0, yP = 0, offsetX = 0, offsetY = 0;
	Uint32 pixelTransparent = SDL_MapRGBA(pSurfaceMap->format, 255, 255, 255, 0);
	SDL_Rect  rect = initRect(x - rayon, y - rayon, 2 * rayon, 2 * rayon);
	offsetX = rect.x;
	offsetY = rect.y;
	for (j = 0; j < 2 * rayon; j++)
	{
		for (i = 0; i < 2 * rayon; i++)
		{
			xP = CARRE(rayon - i);
			yP = CARRE(rayon - j);

			if ((xP + yP) < CARRE(rayon)
				&& i + offsetX >= 0
				&& j + offsetY >= 0
				&& i + offsetX < pSurfaceMap->clip_rect.w
				&& j + offsetY < pSurfaceMap->clip_rect.h
				)
			{
				DrawPixel(pSurfaceMap, i + offsetX, j + offsetY, pixelTransparent);
			}
		}
	}
	reajustRect(&rect, pSurfaceMap);

	if (updateTextureFromSurface (pTextureDisplay,pSurfaceMap, &rect) < 0)
	{
		fprintf(logFile, "explosion : FAILURE, updateTextureFromSurface.");
	}
}