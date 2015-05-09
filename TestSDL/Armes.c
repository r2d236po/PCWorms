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
* \fn void weaponManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms *pWorms, int weaponIndex, SDL_Rect* pCamera)
* \brief Manages the use of weapons.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the main texture.
* \param[in] pWorms, pointer to the worms playing.
* \param[in] weaponIndex, index of the weapons selected.
* \returns void
*/
void weaponManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms *pWorms, int weaponIndex, SDL_Rect* pCamera)
{
	static char armePrec = 0;
	static int xCenter = 0, yCenter = 0;
	SDL_Surface* rotoSurface = NULL;
	SDL_Surface* armeIndex = NULL;
	static SDL_Rect rectWeapon;
	enum DIRECTION dirWeapon = pWorms->dirSurface;
	double angle = 0.0;


	if (globalInput->arme  && !armePrec) // On affiche l'arme la première fois
	{
		armeIndex = selectWeapon(weaponIndex, dirWeapon);
		if (armeIndex == NULL)
			return;
		xCenter = pWorms->wormsObject->objectSurface->clip_rect.x + pWorms->wormsObject->objectSurface->w / 2;
		yCenter = pWorms->wormsObject->objectSurface->clip_rect.y + pWorms->wormsObject->objectSurface->h / 2;
		centerRectToPoint(&armeIndex->clip_rect, xCenter, yCenter);
		displayWorms(pWorms, 1);
		display(armeIndex, 0);
		globalInput->raffraichissement = 1;
	}
	else if (globalInput->arme && armePrec) // On fait tourner l'arme en fonction de la souris
	{
		angle = getAngle(pWorms->wormsObject->objectSurface->clip_rect.x, pWorms->wormsObject->objectSurface->clip_rect.y, &dirWeapon, pCamera);
		if (dirWeapon == RIGHT)
			angle = -angle;
		armeIndex = selectWeapon(weaponIndex, dirWeapon);
		if (armeIndex == NULL)
			return;
		if (dirWeapon != pWorms->dirSurface)
			swapWormsSurface(pWorms);

		rotoSurface = rotozoomSurface(armeIndex, angle, 1.0, 1);
		centerRectToPoint(&rotoSurface->clip_rect, xCenter, yCenter);
		eraseRectFromMap(pMapTerrain, pTextureDisplay, &rectWeapon);
		displayWorms(pWorms, 1);
		display(rotoSurface, 0);
		globalInput->raffraichissement = 1;
	}
	else if (globalInput->arme == 0 && armePrec == 1) // On efface l'arme
	{
		eraseRectFromMap(pMapTerrain, pTextureDisplay, &rectWeapon);
		displayWorms(pWorms, 1);
		globalInput->raffraichissement = 1;
	}

	if (armeIndex != NULL)
		SDL_FreeSurface(armeIndex);
	armeIndex = NULL;
	armePrec = globalInput->arme;
	if (rotoSurface != NULL)
	{
		rectWeapon = rotoSurface->clip_rect;
		SDL_FreeSurface(rotoSurface);
		rotoSurface = NULL;
	}
}

/**
* \fn double getAngle(int x, int y, enum DIRECTION *pDir, SDL_Rect* pCamera)
* \brief Calculate an angle.
*
* \param[in] x, x coordinate of the first point.
* \param[in] y, y coordinate of the first point.
* \param[in] pDir, pointer to the direction to be filled with the orientation of the mouse.
* \returns the angle between the mouse and the point at (x,y)
*/
double getAngle(int x, int y, enum DIRECTION *pDir, SDL_Rect* pCamera)
{
	double dx = 0, dy = 0;
	int xMouse, yMouse;
	getMousePosition(pCamera, &xMouse, &yMouse);
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


SDL_Surface* selectWeapon(int weapondIndex, enum DIRECTION dir)
{
	switch (weapondIndex)
	{
	case 0:
		if (dir == LEFT)
			return animationSprite(arme1, NULL, 2, 0);
		else if (dir == RIGHT)
			return animationSprite(arme1, NULL, 2, 1);
	case 1:
		break;
	}
}

