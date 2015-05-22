#include "display.h"
#include "my_stdrFct.h"


SDL_Texture* pGlobalTexture;
Terrain* pMainTerrain;

/**
* \fn int initDisplay(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay)
* \brief initialize the global pointer for display.
*
* \param[in] pMapTerrain, pointer to the Terrain structure of the game.
* \param[in] pTextureDisplay, pointer to the main texture.
* \returns 0, -1 if error
*/
int initDisplay(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay)
{
	if (pMapTerrain != NULL)
		pMainTerrain = pMapTerrain;
	else
	{
		fprintf(logFile, "initDisplay : FAILURE, pMapTerrain = NULL.\n\n");
		return -1;
	}
	if (pTextureDisplay != NULL)
		pGlobalTexture = pTextureDisplay;
	else
	{
		fprintf(logFile, "initDisplay : FAILURE, pTextureDisplay = NULL.\n\n");
		return -1;
	}
	fprintf(logFile, "initDisplay : DONE.\n\n");
	return 0;
}

/**
* \fn int display(SDL_Surface* pSurface, int mode)
* \brief Displays a surface.
*
* \param[in] pSurface, pointer to the surface.
* \param[in] mode, mode of display (1 = erase last position, 0 = display without erasing).
* \returns -1 = error, 0 = no error
* \remarks The mode 1 should be used on a dynamic surface and the mode 0 on a passive surface.
*/
int display(SDL_Surface* pSurface, int mode)
{
	static SDL_Surface* surfaceTab[NBSURFACEMAX];
	static int nbSurface = 0, xTab[NBSURFACEMAX], yTab[NBSURFACEMAX];
	if (pSurface == NULL && mode >= 0)
	{
		fprintf(logFile, "display : FAILURE, pSurface = NULL.\n\n");
		return -1;
	}
	int index, surfaceFound = 0, indexFound = 0;
	if (nbSurface == 0)
	{
		xTab[0] = 0;
		yTab[0] = 0;
		surfaceTab[0] = NULL;
	}
	for (index = 0; index < nbSurface; index++)
	{
		if (surfaceTab[index] == pSurface)
		{
			surfaceFound = 1;
			indexFound = index;
		}
		if (surfaceTab[index]->flags != 0 || surfaceTab[index]->format == NULL)
		{
			reindexTab(NBSURFACEMAX, index, 0, surfaceTab);
			reindexTab(NBSURFACEMAX, index, 1, xTab);
			reindexTab(NBSURFACEMAX, index, 1, yTab);
			nbSurface--;
			index = 0;
		}
	}
	if (!surfaceFound)
	{
		nbSurface++;
		surfaceTab[index] = pSurface;
		xTab[index] = pSurface->clip_rect.x;
		yTab[index] = pSurface->clip_rect.y;
		indexFound = index;
	}
	SDL_Rect rect = initRect(xTab[indexFound], yTab[indexFound], pSurface->w, pSurface->h);
	if (mode == 1)
		copySurfacePixels(pMainTerrain->collisionMapSurface, &rect, pMainTerrain->globalMapSurface, &rect);
	updateTextureFromMultipleSurface(pGlobalTexture, pMainTerrain->globalMapSurface, pSurface, &rect);
	xTab[indexFound] = pSurface->clip_rect.x;
	yTab[indexFound] = pSurface->clip_rect.y;
	return 0;
}



/**
* \fn void reindexTab(int size, int startPosition, int type, ...)
* \brief Redo the indexation of a tab in case of delete line.
*
* \param[in] size, size of the tab.
* \param[in] startPosition, the position to start the reindex.
* \param[in] type, type of the tab, 0 = array of SDL_Surface*, 1 = array of int, 2 = array of char.
* \returns void
*/
void reindexTab(int size, int startPosition, int type, void* tab)
{
	int index;
	int* intTab = NULL;
	char* charTab = NULL;
	SDL_Surface** pSurfaceTab = NULL;
	switch (type)
	{
	case 0:
		//pSurfaceTab = va_arg(list, SDL_Surface**);
		pSurfaceTab = tab;
		break;
	case 1:
		//intTab = va_arg(list, int*);
		intTab = tab;
		break;
	case 2:
		//charTab = va_arg(list, char*);
		charTab = tab;
		break;
	}
	for (index = startPosition; index < (size - 1); index++)
	{
		if (intTab != NULL)
			intTab[index] = intTab[index + 1];
		else if (charTab != NULL)
			charTab[index] = charTab[index + 1];
		else if (pSurfaceTab != NULL)
			pSurfaceTab[index] = pSurfaceTab[index + 1];
	}
	if (intTab != NULL)
		intTab[(size - 1)] = 0;
	else if (charTab != NULL)
		charTab[(size - 1)] = 0;
	else if (pSurfaceTab != NULL)
		pSurfaceTab[(size - 1)] = NULL;
}


/**
* \fn void endDisplay()
* \brief Reset the global pointer to NULL.
*
* \returns void
*/
void endDisplay()
{
	pMainTerrain = NULL;
	pGlobalTexture = NULL;
}



/**
* \fn int displayWorms(Worms* pWorms, int mode)
* \brief Displays a worms with his life and name.
*
* \param[in] pWorms, pointer to the worms.
* \param[in] mode, mode of display (1 = erase last position, 0 = display without erasing).
* \returns -1 = error, 0 = no error
* \remarks The mode 1 should be used on a dynamic surface and the mode 0 on a passive surface.
*/
int displayWorms(Worms* pWorms, int mode)
{
	if (display(pWorms->texteLifeSurface, mode) < 0)
		return -1;
	if (display(pWorms->texteNameSurface, mode) < 0)
		return -1;
	if (display(pWorms->wormsObject->objectSurface, mode) < 0)
		return -1;
	if (display(pWorms->texteLifeSurface, mode) < 0)
		return -1;
	return 0;
}

/**
* \fn SDL_Surface* animationSprite(SDL_Surface* pSurfaceSprite, SDL_Surface* pAnimSurface, int nbFrame, int indexAnim)
* \brief Load the right frame into a surface from a sprite.
*
* \param[in] pSurfaceSprite, pointer to the sprite's surface.
* \param[in] pAnimSurface, pointer to the animation's surface, NULL creates automatically a new surface.
* \param[in] nbFrame, number of frame in the sprite.
* \param[in] indexAnim, index of the frame in the sprite.
* \returns the surface with the frame, NULL on error
* \remarks ATTENTION : If the size of the pAnimSurface and the size of the frame are different, the function creates a new surface
* and free the old one. If the size are the same, the pAnimSurface is returned. SO BE CAREFULL, if in your function you call
* animationSprite with NULL in pAnimSurface, remember to FREE your surface when you're done.
*/
SDL_Surface* animationSprite(SDL_Surface* pSurfaceSprite, SDL_Surface* pAnimSurface, int nbFrame, int indexAnim)
{
	int w, h, step = 0, x = 0;
	SDL_Surface* newAnimSurface = NULL;
	SDL_Rect clip;
	if (pSurfaceSprite == NULL)
		return pAnimSurface;
	w = pSurfaceSprite->w;
	h = pSurfaceSprite->h;
	step = w / nbFrame;
	x = step * indexAnim;
	clip = initRect(x, 0, step, h);
	if (pAnimSurface == NULL || pAnimSurface->w != step || pAnimSurface->h != h)
	{
		newAnimSurface = SDL_CreateRGBSurface(0, step, h, 32, RMASK, GMASK, BMASK, AMASK);
		if (newAnimSurface != NULL && newAnimSurface != pSurfaceSprite)
		{
			if (!copySurfacePixels(pSurfaceSprite, &clip, newAnimSurface, NULL))
				newAnimSurface = NULL;
			if (pAnimSurface != NULL)
			{
				recenterSurface(pAnimSurface, newAnimSurface);
				SDL_FreeSurface(pAnimSurface);
			}
		}
		return newAnimSurface;
	}
	else copySurfacePixels(pSurfaceSprite, &clip, pAnimSurface, NULL);
	return pAnimSurface;
}

/**
* \fn void recenterSurface(SDL_Surface* oldSurface, SDL_Surface* newSurface)
* \brief Reposition the new surface at the same place as the old surface used to be.
*
* \param[in] oldSurface, pointer to the old surface.
* \param[in] newSurface, pointer to the new surface..
* \returns void
* \remarks the functions handle the negative positionning but NOT the limits of the map, so be carreful.
*/
void recenterSurface(SDL_Surface* oldSurface, SDL_Surface* newSurface)
{
	newSurface->clip_rect.x = oldSurface->clip_rect.x;
	newSurface->clip_rect.y = oldSurface->clip_rect.y + oldSurface->h - newSurface->h;
	if (newSurface->clip_rect.x <= 0)
		newSurface->clip_rect.x = 0;
	if (newSurface->clip_rect.x + newSurface->w >= pMainTerrain->globalMapSurface->w)
		newSurface->clip_rect.x = pMainTerrain->globalMapSurface->w - newSurface->w;
	if (newSurface->clip_rect.y <= 0)
		newSurface->clip_rect.y = 0;
	if (newSurface->clip_rect.y + newSurface->h >= pMainTerrain->globalMapSurface->h)
		newSurface->clip_rect.y = pMainTerrain->globalMapSurface->h - newSurface->h;
}