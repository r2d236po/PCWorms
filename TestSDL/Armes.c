#include "armes.h"
#include "carte.h"
#include "my_stdrFct.h"
#include "AffichageGeneral.h"
#include "display.h"
#include "worms.h"


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
* \fn void weaponManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms** wormsTab, int weaponIndex, SDL_Rect* pCamera)
* \brief Manages the use of weapons.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the main texture.
* \param[in] wormsTab, array of worms.
* \param[in] weaponIndex, index of the weapons selected.
* \returns void
*/
void weaponManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms** wormsTab, int weaponIndex, SDL_Rect* pCamera)
{
	static char armePrec = 0;
	static int xCenter = 0, yCenter = 0, fire = 0;
	SDL_Surface* rotoSurface = NULL;
	SDL_Surface* armeIndex = NULL;
	static SDL_Rect rectWeapon;
	enum DIRECTION dirWeapon = wormsTab[globalVar.indexWormsTab]->dirSurface;
	double angle = 0.0;
	static double angleShot = 0.0;

	if (globalInput->arme  && !armePrec) // On affiche l'arme la première fois
		initWeaponMode(wormsTab[globalVar.indexWormsTab], &xCenter, &yCenter);

	if (globalInput->arme && armePrec) // On fait tourner l'arme en fonction de la souris
	{
		if (!fire)
			//angle = getAngle(wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.x, wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.y, &dirWeapon, pCamera);
			angle = getAngle(xCenter, yCenter, &dirWeapon, pCamera);
		armeIndex = selectWeapon(weaponIndex, dirWeapon);
		if (armeIndex != NULL)
		{
			if (dirWeapon != wormsTab[globalVar.indexWormsTab]->dirSurface)
			{
				swapWormsSurface(wormsTab[globalVar.indexWormsTab]);
				setCenterWeapons(wormsTab[globalVar.indexWormsTab], &xCenter, &yCenter);
			}
			/*Rotate the weapon + center it on worms*/
			if (!fire)
			{
				if (dirWeapon == RIGHT)
					rotoSurface = rotozoomSurface(armeIndex, -angle, 1.0, 1);
				else rotoSurface = rotozoomSurface(armeIndex, angle, 1.0, 1);
			}
			else
			{
				if (dirWeapon == RIGHT)
					rotoSurface = rotozoomSurface(armeIndex, -angleShot * 180.0 / pi, 1.0, 1);
				else rotoSurface = rotozoomSurface(armeIndex, angleShot * 180.0 / pi, 1.0, 1);
			}
			if (rotoSurface == NULL)
			{
				SDL_FreeSurface(armeIndex);
				armePrec = globalInput->arme = 0;
				return;
			}
			centerRectToPoint(&rotoSurface->clip_rect, xCenter, yCenter);

			/*Erase previous position + display worms and weapon*/
			eraseRectFromMap(pMapTerrain, pTextureDisplay, &rectWeapon);
			displayWorms(wormsTab[globalVar.indexWormsTab], 1);
			display(rotoSurface, 0);

			if (globalInput->lclick && !fire)
			{
				fire = 1;
				globalInput->lclick = 0;
				angleShot = angle * pi / 180.0;
			}
			if (fire && fireWeapon(pMapTerrain, pTextureDisplay, dirWeapon, angleShot, wormsTab, rotoSurface))
			{
				fire = 0;
				globalInput->arme = 0;
			}
		}
	}
	if (!globalInput->arme && armePrec) // On efface l'arme
	{
		exitWeaponMode(pMapTerrain, pTextureDisplay, wormsTab[globalVar.indexWormsTab], &rectWeapon);
	}

	if (armeIndex != NULL)
	{
		SDL_FreeSurface(armeIndex);
		globalInput->raffraichissement = 1;
	}
	if (rotoSurface != NULL)
	{
		rectWeapon = rotoSurface->clip_rect;
		SDL_FreeSurface(rotoSurface);
	}
	rotoSurface = NULL;
	armeIndex = NULL;
	armePrec = globalInput->arme;
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
	return NULL;
}



int fireWeapon(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, enum DIRECTION dir, double angle, Worms** wormsTab, SDL_Surface* pSurface)
{
	SDL_Surface* bulletSprite = NULL;
	static SDL_Surface* bulletSurface = NULL;
	static int fire = 0;
	int contact = 0;
	if (!fire)
		bulletSprite = loadImage("../assets/sprites/bullet1.png");
	if (bulletSprite != NULL)
	{
		fire = 1;
		if (dir == LEFT)
		{
			bulletSurface = animationSprite(bulletSprite, NULL, 2, 0);
			bulletSurface = rotozoomSurface(bulletSurface, angle * 180.0 / pi, 1.0, 1);
			bulletSurface->clip_rect.x = pSurface->clip_rect.x - bulletSurface->w;
		}
		else
		{
			bulletSurface = animationSprite(bulletSprite, NULL, 2, 1);
			bulletSurface = rotozoomSurface(bulletSurface, -angle * 180.0 / pi, 1.0, 1);
			bulletSurface->clip_rect.x = pSurface->clip_rect.x + pSurface->w;
		}
		bulletSurface->clip_rect.y = wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.y + wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->h / 2 - bulletSurface->h;
		SDL_FreeSurface(bulletSprite);
	}

	if (bulletSurface != NULL && moveBullet(pMapTerrain, bulletSurface, angle, wormsTab, dir))
	{
		eraseRectFromMap(pMapTerrain, pTextureDisplay, &bulletSurface->clip_rect);
		SDL_FreeSurface(bulletSurface);
		contact = 1;
		fire = 0;
	}

	return contact;
}


int moveBullet(Terrain *pMapTerrain, SDL_Surface* bulletSurface, double angle, Worms** wormsTab, enum DIRECTION dir)
{
	int speedX = (int)(WEAPONSPEEDX * cos(angle)), speedY = (int)(WEAPONSPEEDY * sin(angle));
	if (dir == LEFT)
		speedX = -speedX;
	bulletSurface->clip_rect.x += speedX;
	bulletSurface->clip_rect.y += speedY;
	display(bulletSurface, 1);
	if (impactBulletWorms(wormsTab, &bulletSurface->clip_rect))
	{
		return 1;
	}
	if (collisionRectWithMap(pMapTerrain->collisionMapSurface, &bulletSurface->clip_rect, NULL, NULL))
	{
		return 1;
	}
	return 0;
}

/**
* \fn int impactBulletWorms(Worms** wormsTab, SDL_Rect* pRect)
* \brief Determine if a bullet hit the map or a worms.
*
* \param[in] wormsTab, array of worms.
* \param[in] pRect, rect of the bullet.
* \returns 1 = bullet hit either map or worms, 0 = no hit
*/
int impactBulletWorms(Worms** wormsTab, SDL_Rect* pRect)
{
	int i = 0;
	for (i = 0; i < globalVar.nbWormsTotal; i++)
	{
		if (i == globalVar.indexWormsTab)
			continue;
		if (wormsTab[i]->team == wormsTab[globalVar.indexWormsTab]->team)
			return 1;
		if (collisionRectWithRect(pRect, &wormsTab[i]->wormsObject->objectSurface->clip_rect))
		{
			wormsTab[i]->vie -= 80;
			if (wormsTab[i]->vie <= 0)
			{
				wormsDead(wormsTab[i], 0);
				displayWorms(wormsTab[i], 1);
			}
			globalInput->raffraichissement = 1;
			return 1;
		}
	}
	return 0;
}

/**
* \fn void setCenterWeapons(Worms* pWorms, int* xCenter, int* yCenter)
* \brief Center the weapon on the worms.
*
* \param[in] pWorms, pointer to the worms who is in weapon's mode.
* \param[in] xCenter, pointer to the x value to center the weapon.
* \param[in] yCenter, pointer to the y value to center the weapon.
* \returns void
*/
void setCenterWeapons(Worms* pWorms, int* xCenter, int* yCenter)
{
	*xCenter = pWorms->wormsObject->objectSurface->clip_rect.x + pWorms->wormsObject->objectSurface->w / 2;
	*yCenter = pWorms->wormsObject->objectSurface->clip_rect.y + 2 * pWorms->wormsObject->objectSurface->h / 3;
}

/**
* \fn double getAngle(int x, int y, enum DIRECTION *pDir, SDL_Rect* pCamera)
* \brief Calculate an angle.
*
* \param[in] x, x coordinate of the first point.
* \param[in] y, y coordinate of the first point.
* \param[in] pDir, pointer to the direction to be filled with the orientation of the mouse.
* \returns the angle between the mouse and the point at (x,y) in degree
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

/**
* \fn void initWeaponMode(Worms* pWorms, int* xCenter, int* yCenter)
* \brief Init the weapon mode.
*
* \param[in] pWorms, pointer to the worms who is in weapon's mode.
* \param[in] xCenter, pointer to the x value to center the weapon.
* \param[in] yCenter, pointer to the y value to center the weapon.
* \returns void
*/
void initWeaponMode(Worms* pWorms, int* xCenter, int* yCenter)
{
	/*Center weapon on worms*/
	setCenterWeapons(pWorms, xCenter, yCenter);

	/*Set the target Cursor*/
	SDL_SetCursor(globalInput->cursor.cursor2);
	globalInput->cursor.currentCursor = 1;
}

/**
* \fn void exitWeaponMode(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Worms* pWorms, SDL_Rect* pRect)
* \brief Exit the weapon mode.
*
* \param[in] pMapTerrain, pointer to the worms who is in weapon's mode.
* \param[in] pTextureDisplay, pointer to the x value to center the weapon.
* \param[in] pWorms, pointer to the worms who is in weapon's mode.
* \param[in] pRect, pointer to the rect of the weapon to erase pixel.
* \returns void
*/
void exitWeaponMode(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Worms* pWorms, SDL_Rect* pRect)
{
	/*Erase previous position + display worms and weapon*/
	if (pRect != NULL)
		eraseRectFromMap(pMapTerrain, pTextureDisplay, pRect);
	displayWorms(pWorms, 1);
	globalInput->raffraichissement = 1;

	/*Set standard cursor*/
	SDL_SetCursor(globalInput->cursor.cursor1);
	globalInput->cursor.currentCursor = 0;
}