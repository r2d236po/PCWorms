#include "armes.h"
#include "carte.h"
#include "my_stdrFct.h"
#include "KaamEngine.h"
#include "AffichageGeneral.h"
#include "display.h"
#include "worms.h"
#include "input.h"
#include "Sounds.h"
#include "memory.h"


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
	if (updateGlobalTexture(pTextureDisplay, pSurfaceMap, &rect) < 0)
	{
		fprintf(logFile, "explosion : FAILURE, updateGlobalTexture.");
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
* \param[in] pCamera, pointer to the camera rect.
* \returns void
*/
void weaponManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms** wormsTab, int weaponIndex, SDL_Rect* pCamera)
{
	static char armePrec = 0;
	static int xCenter = 0, yCenter = 0, fire = 0, nbShot = 0;
	SDL_Surface* rotoSurface = NULL;
	SDL_Surface* armeIndex = NULL;
	static SDL_Rect rectWeapon;
	enum DIRECTION dirWeapon = wormsTab[globalVar.indexWormsTab]->dirSurface;
	double angle = 0.0;
	static double angleShot = 0.0;

	if (globalInput->arme  && !armePrec) // On affiche l'arme la première fois
		initWeaponMode(wormsTab[globalVar.indexWormsTab], &xCenter, &yCenter, &rectWeapon, &nbShot);

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
					rotoSurface = my_rotozoomSurface(armeIndex, -angle, 1.0, 1);
				else rotoSurface = my_rotozoomSurface(armeIndex, angle, 1.0, 1);
			}
			else
			{
				if (dirWeapon == RIGHT)
					rotoSurface = my_rotozoomSurface(armeIndex, -angleShot * 180.0 / PI, 1.0, 1);
				else rotoSurface = my_rotozoomSurface(armeIndex, angleShot * 180.0 / PI, 1.0, 1);
			}
			if (rotoSurface != NULL)
			{
				centerRectToPoint(&rotoSurface->clip_rect, xCenter, yCenter);

				/*Erase previous position + display worms and weapon*/
				if (rectWeapon.x == 0 && rectWeapon.y == 0 && rectWeapon.w == 0 && rectWeapon.h == 0)
					rectWeapon = rotoSurface->clip_rect;
				eraseRectFromMap(pMapTerrain, pTextureDisplay, &rectWeapon);
				displayWorms(wormsTab[globalVar.indexWormsTab], 1);
				wormsOverlay(wormsTab);
				display(rotoSurface, 0);

				if (globalInput->lclick && !fire)
				{
					fire = 1;
					globalInput->lclick = 0;
					angleShot = angle * PI / 180.0;
					nbShot++;
				}
				if (fire && fireWeapon(pMapTerrain, pTextureDisplay, dirWeapon, angleShot, wormsTab, rotoSurface))
				{
					fire = 0;
					if (nbShot == NBSHOTPERTOUR)
						globalInput->arme = 0;
				}
			}
		}
	}
	if (!globalInput->arme && armePrec) // On efface l'arme
	{
		exitWeaponMode(pMapTerrain, pTextureDisplay, wormsTab, &rectWeapon);
		if (nbShot == NBSHOTPERTOUR)
			globalInput->changeWorms = 1;
	}


	if (armeIndex != NULL)
	{
		my_freeSurface(armeIndex);
	}
	if (rotoSurface != NULL)
	{
		rectWeapon = rotoSurface->clip_rect;
		my_freeSurface(rotoSurface);
		globalInput->raffraichissement = 1;
	}
	rotoSurface = NULL;
	armeIndex = NULL;
	armePrec = globalInput->arme;
}

/**
* \fn SDL_Surface* selectWeapon(int weapondIndex, enum DIRECTION dir)
* \brief Select the right weapon to display.
*
* \param[in] weapondIndex, index of the weapon.
* \param[in] dir, direction of the weapon.
* \returns surface of the desired weapons, NULL if error.
*/
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
		if (dir == LEFT)
			return animationSprite(arme2, NULL, 2, 0);
		else if (dir == RIGHT)
			return animationSprite(arme2, NULL, 2, 1);
		break;
	}
	return NULL;
}

/**
* \fn int fireWeapon(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, enum DIRECTION dir, double angle, Worms** wormsTab, SDL_Surface* weaponSurface)
* \brief Fire a bullet.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the main texture.
* \param[in] dir, direction of the shot.
* \param[in] angle, angle of the shot in radians.
* \param[in] wormsTab, array of worms.
* \param[in] weaponSurface, surface of the weapons.
* \returns 1 if contact, 0 else.
*/
int fireWeapon(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, enum DIRECTION dir, double angle, Worms** wormsTab, SDL_Surface* weaponSurface)
{
	SDL_Surface* bulletSprite = NULL;
	SDL_Surface* tmp = NULL;
	static SDL_Surface* bulletSurface = NULL;
	static int fire = 0;
	int contact = 0;
	if (!fire)
		bulletSprite = loadImage("../assets/sprites/bullet1.png");
	if (bulletSprite != NULL)
	{
		fire = 1;
		playChunk(1, FireBullet);
		if (dir == LEFT)
		{
			tmp = animationSprite(bulletSprite, NULL, 2, 0);
			if (tmp != NULL)
			{
				bulletSurface = my_rotozoomSurface(tmp, angle * 180.0 / PI, 1.0, 1);
				my_freeSurface(tmp);
			}
			bulletSurface->clip_rect.x = weaponSurface->clip_rect.x - bulletSurface->w;
		}
		else
		{
			tmp = animationSprite(bulletSprite, NULL, 2, 1);
			if (tmp != NULL)
			{
				bulletSurface = my_rotozoomSurface(tmp, -angle * 180.0 / PI, 1.0, 1);
				my_freeSurface(tmp);
			}
			bulletSurface->clip_rect.x = weaponSurface->clip_rect.x + weaponSurface->w;
		}
		bulletSurface->clip_rect.y = wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.y + wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->h / 2 - bulletSurface->h;
		my_freeSurface(bulletSprite);
	}

	if (bulletSurface != NULL && moveBullet(pMapTerrain, bulletSurface, angle, wormsTab, dir))
	{
		eraseRectFromMap(pMapTerrain, pTextureDisplay, &bulletSurface->clip_rect);
		my_freeSurface(bulletSurface);
		bulletSurface = NULL;
		contact = 1;
		fire = 0;
	}

	return contact;
}

/**
* \fn int moveBullet(Terrain *pMapTerrain, SDL_Surface* bulletSurface, double angle, Worms** wormsTab, enum DIRECTION dir)
* \brief Move the bullet.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] bulletSurface, surface of the bullet.
* \param[in] angle, angle of the shot in radians.
* \param[in] wormsTab, array of worms.
* \param[in] dir, direction of the shot.
* \returns void
*/
int moveBullet(Terrain *pMapTerrain, SDL_Surface* bulletSurface, double angle, Worms** wormsTab, enum DIRECTION dir)
{
	int speedX = (int)(WEAPONSPEEDX * cos(angle)), speedY = (int)(WEAPONSPEEDY * sin(angle));
	int index = 0;
	if (dir == LEFT)
		speedX = -speedX;
	bulletSurface->clip_rect.x += speedX;
	bulletSurface->clip_rect.y += speedY;
	display(bulletSurface, 1);
	wormsOverlayWithSurface(wormsTab, bulletSurface);
	if (impactBulletWorms(wormsTab, &bulletSurface->clip_rect, &index))
	{
		if (index >= 0 && wormsTab[index]->vie > 0)
		{
			if (dir == LEFT)
				wormsTab[index]->wormsObject->objectSurface->clip_rect.x -= 2;
			else wormsTab[index]->wormsObject->objectSurface->clip_rect.x += 2;
		}
		return 1;
	}
	if (collisionRectWithMap(pMapTerrain->collisionMapSurface, &bulletSurface->clip_rect, NULL, NULL))
	{
		return 1;
	}
	return 0;
}

/**
* \fn int impactBulletWorms(Worms** wormsTab, SDL_Rect* pRect, int* index)
* \brief Determine if a bullet hit the map or a worms.
*
* \param[in] wormsTab, array of worms.
* \param[in] pRect, rect of the bullet.
* \param[in] index, index of the worms touched by the bullet.
* \returns 1 = bullet hit either map or worms, 0 = no hit
*/
int impactBulletWorms(Worms** wormsTab, SDL_Rect* pRect, int* index)
{
	int i = 0;
	for (i = 0; i < globalVar.nbWormsTotal; i++)
	{
		if (i == globalVar.indexWormsTab)
			continue;
		if (collisionRectWithRect(pRect, &wormsTab[i]->wormsObject->objectSurface->clip_rect))
		{
			if (wormsTab[i]->vie > 0)
			{
				wormsTab[i]->vie -= 15;
				if (wormsTab[i]->vie <= 0)
				{
					wormsDead(wormsTab[i], 0);
					displayWorms(wormsTab[i], 1);
				}
				globalInput->raffraichissement = 1;
				*index = i;
			}
			else *index = -1;
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
* \param[in] pCamera, pointer to the camera rect.
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
		return (atan(dy / MY_ABS(dx)) / PI) * 180.0;
	else if (dy > 0)
		return -90.0;
	else return 90.0;
}

/**
* \fn void initWeaponMode(Worms* pWorms, int* xCenter, int* yCenter, SDL_Rect* pRect, int *nbShot)
* \brief Init the weapon mode.
*
* \param[in] pWorms, pointer to the worms who is in weapon's mode.
* \param[in] xCenter, pointer to the x value to center the weapon.
* \param[in] yCenter, pointer to the y value to center the weapon.
* \param[in] pRect, pointer to the rect of the weapon to erase pixel.
* \param[in] nbShot, pointer to the shot counter.
* \returns void
*/
void initWeaponMode(Worms* pWorms, int* xCenter, int* yCenter, SDL_Rect* pRect, int *nbShot)
{
	static int indexWorms = 0;
	/*Init the shot counter*/
	if (indexWorms != globalVar.indexWormsTab)
		*nbShot = 0;

	/*Center weapon on worms*/
	setCenterWeapons(pWorms, xCenter, yCenter);

	/*Set the target Cursor*/
	SDL_SetCursor(globalInput->cursor.cursor2);
	globalInput->cursor.currentCursor = 1;

	/*Init rect*/
	*pRect = initRect(0, 0, 0, 0);

	indexWorms = globalVar.indexWormsTab;
}

/**
* \fn void exitWeaponMode(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Worms** wormsTab, SDL_Rect* pRect)
* \brief Exit the weapon mode.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the main texture.
* \param[in] wormsTab, array of worms.
* \param[in] pRect, pointer to the rect of the weapon to erase pixel.
* \returns void
*/
void exitWeaponMode(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Worms** wormsTab, SDL_Rect* pRect)
{
	/*Erase previous position + display worms and weapon*/
	if (pRect != NULL)
		eraseRectFromMap(pMapTerrain, pTextureDisplay, pRect);
	displayWormsTab(wormsTab, 0);
	globalInput->raffraichissement = 1;

	/*Set standard cursor*/
	SDL_SetCursor(globalInput->cursor.cursor1);
	globalInput->cursor.currentCursor = 0;

	/*Reset rect*/
	*pRect = initRect(0, 0, 0, 0);
}

/**
* \fn void grenadeManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms** wormsTab, SDL_Rect* pCamera)
* \brief Manage the grenade mode.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the main texture.
* \param[in] wormsTab, pointer to the worms who is in weapon's mode.
* \param[in] pCamera, pointer to the rect of the weapon to erase pixel.
* \returns void
*/
void grenadeManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms** wormsTab, SDL_Rect* pCamera)
{
	int radius = 50, triggerBomb = 0, endAnim = 0;
	SDL_Rect  bombRect;
	static KaamObject *grenadeObject = NULL;
	static int animGrenade = 0, indexAnim = 0, lastTime = 0, counter = 0, x = 0, y = 0;
	int timerGrenade = TIMERGRENADE - counter;
	double power = 0.0;
	if (timerGrenade <= 0 && !animGrenade)
	{
		triggerBomb = 1;
		animGrenade = 1;
	}
	if (!triggerBomb && !animGrenade && grenadeObject == NULL)
	{
		if (powerGrenade(pMapTerrain, pTextureDisplay, &power, pCamera))
		{
			SDL_Surface* surface = loadImage(GRENADEPATH);
			if (surface != NULL)
			{
				float speedX = 0.0, speedY = 0.0;
				enum DIRECTION dirGrenade = DOWN;
				initGrenade(pCamera, wormsTab[globalVar.indexWormsTab], &speedX, &speedY, &dirGrenade, power);
				grenadeObject = KaamInitObject(surface, speedX, speedY, dirGrenade, 1);
				if (wormsTab[globalVar.indexWormsTab]->dirSurface == RIGHT)
				{
					initObjectPosition(grenadeObject,
						wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.x + wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->w,
						wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.y);
				}
				else
				{
					initObjectPosition(grenadeObject,
						wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.x - grenadeObject->objectSurface->w,
						wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.y);
				}
				playChunk(1, LancerGrenade);
				my_freeSurface(surface);
			}
			else return;
		}
		else counter = 1;
	}
	if (grenadeObject != NULL)
	{
		KaamPhysicManagement(grenadeObject, pMapTerrain->collisionMapSurface);
		display(grenadeObject->objectSurface, 1);
		wormsOverlayWithSurface(wormsTab, grenadeObject->objectSurface);
		globalInput->raffraichissement = 1;
	}


	/*Reaction to the bomb*/
	if (triggerBomb && grenadeObject != NULL)
	{
		x = grenadeObject->objectSurface->clip_rect.x + grenadeObject->objectSurface->w / 2;
		y = grenadeObject->objectSurface->clip_rect.y + grenadeObject->objectSurface->h / 2;
		bombRect = initRect(x - radius, y - radius, 2 * radius, 2 * radius);
		bombReactionManagement(wormsTab, &bombRect, x, y, radius);
		eraseRectFromMap(pMapTerrain, pTextureDisplay, &grenadeObject->objectSurface->clip_rect);
		if (grenadeObject != NULL)
			KaamDestroyObject(&grenadeObject);
		animGrenade = 1;
		playChunk(1, ExploMed);
	}

	/*Anim explosion*/
	if (animGrenade)
	{
		endAnim = animationGrenade(pMapTerrain, pTextureDisplay, &indexAnim, x, y, wormsTab);
	}

	/*Destruction of the map*/
	if (endAnim)
	{
		bombRect = initRect(x - 50, y - 50, 100, 100);
		explosion(x, y, radius, pMapTerrain->globalMapSurface, pTextureDisplay);
		updateSurfaceFromSurface(pMapTerrain->collisionMapSurface, pMapTerrain->globalMapSurface, &bombRect, 0);
		globalInput->grenade = 0;
		triggerBomb = animGrenade = counter = endAnim = 0;
		x = 0;
		y = 0;
		globalInput->raffraichissement = 1;
		globalInput->changeWorms = 1;
	}
	else if ((SDL_GetTicks() - lastTime) >= 1000)
	{
		lastTime = SDL_GetTicks();
		counter++;
	}

}

/**
* \fn void initGrenade(SDL_Rect *pCamera, Worms *pWorms, float *speedX, float *speedY, enum DIRECTION *dir, double power)
* \brief initialise the parameters of the grenade object.
*
* \param[in] pCamera, pointer to the camera rect.
* \param[in] pWorms, pointer to the worms who is in weapon's mode.
* \param[in] speedX, variable to be filled with init x speed.
* \param[in] speedY, variable to be filled with init y speed.
* \param[in] dir, pointer to a direction to be filled with the init direction of the grenade.
* \returns void
*/
void initGrenade(SDL_Rect *pCamera, Worms *pWorms, float *speedX, float *speedY, enum DIRECTION *dir, double power)
{
	enum DIRECTION dirMouse;
	double angle = getAngle(pWorms->wormsObject->objectSurface->clip_rect.x + pWorms->wormsObject->objectSurface->w / 2,
		pWorms->wormsObject->objectSurface->clip_rect.y + pWorms->wormsObject->objectSurface->h / 2, &dirMouse, pCamera);

	if (angle == 90.0)
		*dir = UP;
	else if (angle == -90.0)
		*dir = DOWN;
	else if (angle < 0.0)
	{
		if (dirMouse == LEFT)
			*dir = UPLEFT;
		else *dir = UPRIGHT;
	}
	else if (angle > 0.0)
	{
		if (dirMouse == LEFT)
			*dir = DLEFT;
		else *dir = DRIGHT;
	}
	else
	{
		if (dirMouse == LEFT)
			*dir = LEFT;
		else *dir = RIGHT;
	}
	if (dirMouse == RIGHT)
		*speedX = (float)(GRENADESPEEDX * power * cos(angle * PI / 180.0));
	else *speedX = (float)(-GRENADESPEEDX * power * cos(angle * PI / 180.0));
	*speedY = (float)(GRENADESPEEDY * power * sin(-angle * PI / 180.0));
	if (MY_ABS(angle) < 30.0)
	{
		*speedY = (float)(GRENADESPEEDY * power * sin(PI / 6.0));
	}
}

/**
* \fn int animationGrenade(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, int *indexAnim, int x, int y, Worms** wormsTab)
* \brief Animate the explosion of the grenade.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the main texture.
* \param[in] indexAnim, pointer to the index of the animation.
* \param[in] x, x postion of the grenade.
* \param[in] y, y postion of the grenade.
* \returns 1 = animation is done, 0 = animation is still running
*/
int animationGrenade(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, int *indexAnim, int x, int y, Worms** wormsTab)
{
	int endAnim = 0, index;
	char str[100];
	SDL_Surface* animSprite = NULL, *exploSurface = NULL;
	index = selectExplo(str, *indexAnim);
	animSprite = loadImage(str);
	if (animSprite != NULL)
	{
		exploSurface = animationSprite(animSprite, NULL, NBFRAMEGRENADE / 4, index);
		my_freeSurface(animSprite);
		if (exploSurface != NULL)
		{
			exploSurface->clip_rect.x = x - 50;
			exploSurface->clip_rect.y = y - 50;
			ajustExploWithMap(pMapTerrain->collisionMapSurface, &exploSurface);
			display(exploSurface, 1);
			wormsOverlayWithSurface(wormsTab, exploSurface);
			*indexAnim += 1;
			globalInput->raffraichissement = 1;
		}
	}
	if (*indexAnim == NBFRAMEGRENADE)
	{
		*indexAnim = 0;
		endAnim = 1;
		eraseRectFromMap(pMapTerrain, pTextureDisplay, &exploSurface->clip_rect);
	}
	if (exploSurface != NULL)
	{
		my_freeSurface(exploSurface);
		exploSurface = NULL;
	}
	return endAnim;
}

/**
* \fn int selectExplo(char *str, int indexGen)
* \brief Determines wich sprite of the explo need to be loaded.
*
* \param[in] str,string to be filled with the path to the sprite of the explosion.
* \param[in] indexGen, general index of the animation.
* \returns sub index of the animation in the current sprite.
*/
int selectExplo(char *str, int indexGen)
{
	int index = 0;
	if (indexGen < 5)
	{
		strcpy(str, EXPLO1);
		index = indexGen;
	}
	else if (indexGen >= 5 && indexGen < 10)
	{
		strcpy(str, EXPLO2);
		index = indexGen - 5;
	}
	else if (indexGen >= 10 && indexGen < 15)
	{
		strcpy(str, EXPLO3);
		index = indexGen - 10;
	}
	else if (indexGen >= 15 && indexGen < 20)
	{
		strcpy(str, EXPLO4);
		index = indexGen - 15;
	}
	else if (indexGen >= 20 && indexGen < 25)
	{
		strcpy(str, EXPLO5);
		index = indexGen - 20;
	}

	return index;
}

/**
* \fn void ajustExploWithMap(SDL_Surface* pSurfaceMap, SDL_Surface** exploSurface)
* \brief Crop the surface to the limit of the map.
*
* \param[in] pSurfaceMap, map's surface.
* \param[in] exploSurface, explosion surface.
* \returns void.
*/
void ajustExploWithMap(SDL_Surface* pSurfaceMap, SDL_Surface** exploSurface)
{
	int x = (*exploSurface)->clip_rect.x, y = (*exploSurface)->clip_rect.y, w = (*exploSurface)->w, h = (*exploSurface)->h;
	SDL_Rect troncRect = initRect(0, 0, w, h);
	if (x < 0)
	{
		troncRect.x = -x;
		troncRect.w = w + x;
		x = 0;
	}
	if (y < 0)
	{
		troncRect.y = -y;
		troncRect.h = h + y;
		y = 0;
	}
	if (x + w > pSurfaceMap->w)
		troncRect.w = pSurfaceMap->w - x;
	if (y + h > pSurfaceMap->h)
		troncRect.h = pSurfaceMap->h - y;
	if (troncRect.x != 0 || troncRect.y != 0 || troncRect.w != w || troncRect.h != h)
	{
		SDL_Surface* surfaceTemp = NULL;
		surfaceTemp = my_CreateRGBSurface(0, troncRect.w, troncRect.h, 32, RMASK, GMASK, BMASK, AMASK);
		if (surfaceTemp != NULL)
		{
			if (copySurfacePixels((*exploSurface), &troncRect, surfaceTemp, NULL))
			{
				my_freeSurface((*exploSurface));
				(*exploSurface) = surfaceTemp;
				(*exploSurface)->clip_rect.x = x;
				(*exploSurface)->clip_rect.y = y;
			}
		}
	}
}

/**
* \fn int powerGrenade(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay,double *power, SDL_Rect* pCamera)
* \brief Get the power of the launch grenade.
*
* \param[in] pMapTerrain, pointer to a terrain structure.
* \param[in] pTextureDisplay, pointer to the x value to center the weapon.
* \param[in] power, pointer to the power variable.
* \param[in] x, x postion of the grenade.
* \param[in] y, y postion of the grenade.
* \returns 1 = grenade is launched, 0 = grenade is not launched.
*/
int powerGrenade(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, double *power, SDL_Rect* pCamera)
{
	int launchGrenade = 0, w, h;
	static int counterPowerBar = 1, counterTime, lastTime = 0;
	double coeff = (double)((POWERMAX - POWERMIN) / (NBFRAMEPOWER - 1));
	SDL_Surface* powerSurface = NULL;
	SDL_Surface* completeBar = loadImage(POWERBARPATH);

	w = pMapTerrain->globalMapSurface->w;
	h = pMapTerrain->globalMapSurface->h;
	if (completeBar != NULL)
	{
		powerSurface = loadPowerBar(completeBar, counterPowerBar);
		if (powerSurface != NULL)
		{
			powerSurface->clip_rect.x = pCamera->x;
			powerSurface->clip_rect.y = 0;
			completeBar->clip_rect.x = pCamera->x;
			completeBar->clip_rect.y = 0;
			eraseRectFromMap(pMapTerrain, pTextureDisplay, &completeBar->clip_rect);
			display(powerSurface, 1);
			globalInput->raffraichissement = 1;
			my_freeSurface(powerSurface);
			powerSurface = NULL;
		}
	}

	if (globalInput->lclick)
	{
		*power = (double)(counterPowerBar * coeff) + (1.0 - coeff);
		globalInput->lclick = 0;
		counterPowerBar = 1;
		launchGrenade = 1;
		if (completeBar != NULL)
			eraseRectFromMap(pMapTerrain, pTextureDisplay, &completeBar->clip_rect);
	}
	if ((SDL_GetTicks() - lastTime) >= 100)
	{
		lastTime = SDL_GetTicks();
		counterPowerBar++;
		if (counterPowerBar == NBFRAMEPOWER + 1)
			counterPowerBar = 1;
	}
	if (completeBar != NULL)
		my_freeSurface(completeBar);
	return launchGrenade;
}

/**
* \fn SDL_Surface* loadPowerBar(SDL_Surface* completeBar, int counter)
* \brief Get the right loading image.
*
* \param[in] completeBar, map's surface.
* \param[in] counter, counter of loading.
* \returns NULL on error, pointer to the surface else.
*/
SDL_Surface* loadPowerBar(SDL_Surface* completeBar, int counter)
{
	SDL_Surface* temp = NULL;
	int w = 0, h = 0;
	SDL_Rect loadRect;
	h = completeBar->h;
	w = counter * completeBar->w / NBFRAMEPOWER;
	temp = my_CreateRGBSurface(0, w, h, 32, RMASK, GMASK, BMASK, AMASK);
	if (temp != NULL)
	{
		loadRect.x = 0;
		loadRect.y = 0;
		loadRect.w = w;
		loadRect.h = completeBar->h;
		copySurfacePixels(completeBar, &loadRect, temp, NULL);
	}
	return temp;
}