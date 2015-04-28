#include "display.h"
#include "my_stdrFct.h"

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
			reindexTab(nbSurface, index, 0, surfaceTab);
			reindexTab(nbSurface, index, 1, xTab);
			reindexTab(nbSurface, index, 1, yTab);
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
	va_list list;
	va_start(list, type);
	switch (type)
	{
	case 0 :
		//pSurfaceTab = va_arg(list, SDL_Surface**);
		pSurfaceTab = tab;
		break;
	case 1 :
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
	va_end(list);
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

