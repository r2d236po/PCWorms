#include "armes.h"
#include "carte.h"
#include "my_stdrFct.h"
#include "AffichageGeneral.h"
#include "display.h"


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
void explosion(int x, int y, int rayon, SDL_Surface *pSurfaceMap, SDL_Texture *pTextureDisplay)
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
				WritePixel(pSurfaceMap, i + offsetX, j + offsetY, pixelTransparent);
			}
		}
	}
	reajustRect(&rect, pSurfaceMap);
	if (updateTextureFromSurface(pTextureDisplay, pSurfaceMap, &rect) < 0)
	{
		fprintf(logFile, "explosion : FAILURE, updateTextureFromSurface.");
	}
}

/**
* \fn void weaponManagement(Input *pInput, Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms *pWorms, int weaponIndex)
* \brief Manages the use of weapons.
*
* \param[in] pInput, pointer to hte input structure.
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the main texture.
* \param[in] pWorms, pointer to the worms playing.
* \param[in] weaponIndex, index of the weapons selected.
* \returns void
*/
void weaponManagement(Input *pInput, Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms *pWorms, int weaponIndex)
{
	static char armePrec = 0;
	SDL_Surface* rotoSurface = NULL;
	static SDL_Rect rectWeapon;
	enum DIRECTION dirWeapon = pWorms->dirSurface;
	double angle = 0.0;


	if (pInput->arme  && !armePrec) // On affiche l'arme la première fois
	{
		arme1->clip_rect.x = pWorms->wormsObject->objectSurface->clip_rect.x - 10;
		arme1->clip_rect.y = pWorms->wormsObject->objectSurface->clip_rect.y + 5;
		display(pWorms->wormsObject->objectSurface, 1);
		display(arme1, 0);
	}
	else if (pInput->arme && armePrec) // On fait tourner l'arme en fonction de la souris
	{
		angle = getAngle(pWorms->wormsObject->objectSurface->clip_rect.x, pWorms->wormsObject->objectSurface->clip_rect.y, &dirWeapon);

		if (dirWeapon != pWorms->dirSurface)
			swapWormsSurface(pWorms);

		rotoSurface = rotozoomSurface(arme1, angle, 1.0, 1);
		centerRectToPoint(&rotoSurface->clip_rect, arme1->clip_rect.x + arme1->w / 2, arme1->clip_rect.y + arme1->h / 2);
		eraseWeapon(pMapTerrain, pTextureDisplay, &rectWeapon);
		display(pWorms->wormsObject->objectSurface, 1);
		display(rotoSurface, 0);
	}
	else if (pInput->arme == 0 && armePrec == 1) // On efface l'arme
	{
		eraseWeapon(pMapTerrain, pTextureDisplay, &rectWeapon);
		display(pWorms->wormsObject->objectSurface, 1);
	}

	armePrec = pInput->arme;
	if (rotoSurface != NULL)
	{
		rectWeapon = rotoSurface->clip_rect;
		SDL_FreeSurface(rotoSurface);
		rotoSurface = NULL;
	}
}

/**
* \fn double getAngle(int x, int y, enum DIRECTION *pDir)
* \brief Calculate an angle.
*
* \param[in] x, x coordinate of the first point.
* \param[in] y, y coordinate of the first point.
* \param[in] pDir, pointer to the direction to be filled with the orientation of the mouse.
* \returns the angle between the mouse and the point at (x,y)
*/
double getAngle(int x, int y, enum DIRECTION *pDir)
{
	double dx = 0, dy = 0;
	int xMouse, yMouse;
	SDL_GetMouseState(&xMouse, &yMouse);
	dx = xMouse - x;
	dy = yMouse - y;
	if (dx < 0)
		*pDir = LEFT;
	else *pDir = RIGHT;
	if (dx != 0.0)
		return (atan(dy / MY_ABS(dx)) / pi) * 180.0;
	else if (dy > 0)
		return -90.0;
	else return 90.0;
}

/**
* \fn void eraseWeapon(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, SDL_Rect *pRect)
* \brief Restore an area of the main surface.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the main texture.
* \param[in] pRect, pointer to the rect of the area to erase.
* \returns void
*/
void eraseWeapon(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, SDL_Rect *pRect)
{
	updateSurfaceFromSurface(pMapTerrain->globalMapSurface, pMapTerrain->collisionMapSurface, pRect, 1);
	updateTextureFromSurface(pTextureDisplay, pMapTerrain->globalMapSurface, pRect);
}