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
	Uint32 pixelTest = SDL_MapRGBA(pSurfaceMap->format, 255, 255, 255, 0);
	SDL_Rect  rect = initRect(x - rayon, y - rayon, 2 * rayon, 2 * rayon);
	int id;
	double temps = 0.0;
	long clk_tck = CLOCKS_PER_SEC;
	clock_t t1, t2;
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
				&& i + offsetX < pSurfaceMap->clip_rect.w
				&& j + offsetY < pSurfaceMap->clip_rect.h
				)
			{
				DrawPixel(pSurfaceMap, i + offsetX, j + offsetY, pixelTest);
			}
		}
	}
	reajustRect(&rect, pSurfaceMap);
	/*t1 = clock();
	for (id = 0; id < 1000; id++)
		updateGlobaleTexture2(pSurfaceMap, pSurfaceMap, pTextureDisplay, &rect);
	t2 = clock();
	temps = (double)(t2 - t1) / (double)clk_tck;*/
	if (updateGlobaleTexture(pSurfaceMap, pSurfaceMap, pTextureDisplay, &rect) < 0)
	{
		printf("Erreur lors de l'update de la texture.");
	}
}