#include "physique.h"
#include "AffichageGeneral.h"
#include "my_stdrFct.h"
#include "KaamEngine.h"







//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                Gestion de la physique                  /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn int gestionPhysique(Terrain* map, SDL_Texture* pTextureDisplay, Input* pInput, ...)
* \brief Gere la physique autour d'un worms.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] worms, pointeur du worms a deplacer
* \param[in] pMapTerrain, pointeur vers la structure de la map
* \param[in] pTextureDisplay, texture globale pour actualiser la position du worms dans celle-ci
* \returns void
*/
int gestionPhysique(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Input* pInput, ...)
{
	Worms* worms = NULL;
	//Arme* weapon = NULL;
	va_list list;

	va_start(list, pInput);
	switch (va_arg(list, int))
	{
	case 0:
		worms = va_arg(list, Worms*);
		//KaamPhysicManagement(pInput, worms->wormsObject, pMapTerrain->imageMapSurface);
		updateTextureFromMultipleSurface(pTextureDisplay, pMapTerrain->imageMapSurface, worms->wormsObject->objectSurface, &worms->wormsObject->objectBox);
		break;
	case 1:
		//cas d'une arme
		break;
	}
	va_end(list);
	return 0;
}

/**
* \fn int checkJump(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION dir)
*
* \brief Deteremine la faisabilite d'un saut.
*
* \param[in] pSurfaceMap, surface de la map
* \param[in] pSurfaceMotion, surface en mouvement dans la map
* \return int, 1 = deplacement possible, 0 = deplacement non possible
*/
int checkDeplacement(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION direction)
{
	enum DIRECTION dir = direction;
	int x, y;
	x = pSurfaceMotion->clip_rect.x;
	y = pSurfaceMotion->clip_rect.y;
	switch (dir)
	{
	case UP:
		pSurfaceMotion->clip_rect.y -= 1;
		break;
	case UPLEFT:
		pSurfaceMotion->clip_rect.y -= 1;
		pSurfaceMotion->clip_rect.x -= 1;
		if (!checkDeplacement(pSurfaceMap, pSurfaceMotion, UP))
		{
			pSurfaceMotion->clip_rect.y += 1;
			pSurfaceMotion->clip_rect.x += 1;
			return 0;
		}
		break;
	case UPRIGHT:
		pSurfaceMotion->clip_rect.y -= 1;
		pSurfaceMotion->clip_rect.x += 1;
		if (!checkDeplacement(pSurfaceMap, pSurfaceMotion, UP))
		{
			pSurfaceMotion->clip_rect.y += 1;
			pSurfaceMotion->clip_rect.x -= 1;
			return 0;
		}
		break;
	case DRIGHT:
		pSurfaceMotion->clip_rect.y += 1;
		pSurfaceMotion->clip_rect.x += 1;
		break;
	case DLEFT:
		pSurfaceMotion->clip_rect.y += 1;
		pSurfaceMotion->clip_rect.x -= 1;
		break;
	case RIGHT:
		pSurfaceMotion->clip_rect.x += 1;
		break;
	case LEFT:
		pSurfaceMotion->clip_rect.x -= 1;
		break;
	case DOWN:
		pSurfaceMotion->clip_rect.y += 1;
		break;
	}
	if (collisionSurfaceWithMap(pSurfaceMap, pSurfaceMotion, &dir, 1))
	{
		pSurfaceMotion->clip_rect.y = y;
		pSurfaceMotion->clip_rect.x = x;
		if ((direction == LEFT || direction == RIGHT || direction == UP) && dir == DOWN)
			return 1;
		if ((direction == LEFT || direction == RIGHT) && dir == UP)
			return 1;
		return 0;
	}
	pSurfaceMotion->clip_rect.y = y;
	pSurfaceMotion->clip_rect.x = x;
	return 1;
}

/**
* \fn void initGameWorms(Worms** worms, Input* pInput, Terrain* map, SDL_Texture* pTextureDisplay, SDL_Renderer* pRenderer, SDL_Rect* pCamera)
* \brief Initialise les worms d'une équipe de la partie jusqu'au sol.
*
* \param[in] wormsTab, tableau de worms.
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'état des inputs.
* \param[in] pMapTerrain, pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqué la camera.
* \param[in] pRenderer pointeur pRenderer pour récupérer les informations relative à la fenêtre.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \returns void
* \remarks A refaire pour chaque equipe
*/
void initGameWorms(Worms** wormsTab, Input* pInput, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Renderer* pRenderer, SDL_Rect* pCamera)
{
	int i = 0;
	srand((int)time(NULL));
	for (i = 0; i < globalVar.nbWormsEquipe; i++)
	{
		//wormsTab[i]->wormsSurface->clip_rect.x = wormsTab[i]->wormsRect.x = rand_a_b(0, (pMapTerrain->imageMapSurface->w - wormsTab[i]->wormsSurface->w));
		while (checkDeplacement(pMapTerrain->imageMapSurface, wormsTab[i]->wormsObject->objectSurface, DOWN))
		{
			gestionPhysique(pMapTerrain, pTextureDisplay, pInput, 0, wormsTab[i]);
			updateScreen(pRenderer, 2, 0, pMapTerrain, 1, pTextureDisplay, pCamera, NULL);
		}
	}
	fprintf(logFile, "initGameWorms : DONE.\n\n");
}



