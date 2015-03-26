#include "physique.h"
#include "AffichageGeneral.h"







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
		gestionPhysiqueWorms(pInput, worms, pMapTerrain, pTextureDisplay);
		break;
	case 1:
		//cas d'une arme
		break;
	}
	va_end(list);
	return 0;
}


/**
* \fn void gestionPhysiqueWorms(Input* pInput, Worms* worms, Terrain* map, SDL_Texture* pTextureDisplay)
* \brief Gere la physique autour d'un worms.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] pWorms, pointeur du worms a deplacer
* \param[in] pMapTerrain, pointeur vers la structure de la map
* \param[in] pTextureDisplay, texture globale pour actualiser la position du worms dans celle-ci
* \returns void
*/
void gestionPhysiqueWorms(Input* pInput, Worms* pWorms, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay)
{
	int xRel = 0, yRel = 0;
	static char retournement = 0;
	static int  t = 0, start = 0;
	int collision = 0;
	if (pInput->jumpOnGoing || pInput->direction != NONE || pInput->jump || !checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, DOWN))
	{
		if (pInput->jumpOnGoing)
		{
			if (!start && checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, pWorms->dir))
			{
				finDeplacement(pInput, pWorms, pMapTerrain, 1);
				return;
			}
			start = 1;
			//On remet � zero x et y par rapport � sa position absolu de d�part
			pWorms->wormsSurface->clip_rect.x = pWorms->xAbs;
			pWorms->wormsSurface->clip_rect.y = pWorms->yAbs;
			pInput->raffraichissement = 1;
		}

		/////////////////////////////////////////////////////////////
		////////////         R�alisation du saut         ////////////
		/////////////////////////////////////////////////////////////

		/*Calcul des positions relatives*/
		if (pWorms->xAbs != pWorms->wormsSurface->clip_rect.x || pWorms->yAbs != pWorms->wormsSurface->clip_rect.y)
			pInput->raffraichissement = 1;
		calculPositionRel(&xRel, &yRel, t, pWorms->vitx, pWorms->vity, pWorms->wormsSurface);
		t += 7;

		/*Determination de la direction du saut*/
		pWorms->dir = calculDirectionDeplacement(xRel, yRel, pWorms->dir, pInput);

		/*Calcul d'un eventuel retournement*/
		if (!retournement && !pInput->jumpOnGoing)
			retournement = retournementWorms(pInput, pWorms);

		/*Fonction de d�placement du worms si non saut*/
		if (!pInput->jumpOnGoing && !retournement)
			deplacementWorms(pInput, pWorms, pMapTerrain->imageMapSurface);

		/*Gestion de la collision*/
		if (retournement)
		{
			swapSurface(pWorms);
			if (pWorms->dir != DOWN && checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, UP))
			{
				swapSurface(pWorms);
				deplacementWorms(pInput, pWorms, pMapTerrain->imageMapSurface);
				retournement = 0;
			}
			retournement = 0;
		}
		collision = gestionCollision(pInput->acceleration, pMapTerrain->imageMapSurface, pWorms->wormsSurface, &pWorms->dir, 0);

		/*Gestion de fin de mouvement*/
		if (collision)
		{
			finDeplacement(pInput, pWorms, pMapTerrain, 0);
			retournement = 0;
			t = 0;
			start = 0;
		}
		while (checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, pWorms->dirSurface))
		{
			if (pWorms->dirSurface == RIGHT)
				pWorms->wormsSurface->clip_rect.x -= 1;
			else pWorms->wormsSurface->clip_rect.x += 1;
		}
		/*Mise � jour de l'affichage*/
		if (!checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, UP) || !checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, LEFT) || !checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, RIGHT))
			updateGlobaleTexture(pMapTerrain->imageMapSurface, pWorms->wormsSurface, pTextureDisplay, &pWorms->wormsRect);
		pInput->deplacement = 1;
	}
	else
	{
		pInput->deplacement = 0;
		finDeplacement(pInput, pWorms, pMapTerrain, 0);
	}
}


/**
* \fn void calculPositionRel(int* xRel, int* yRel, int t, float vit_X, float vit_Y, SDL_Surface* pSurface)
* \brief Calcule les positions relatives durant un deplacement.
*
* \param[in] xRel, pointeur vers la valeur de l'offset en x relatif
* \param[in] yRel, pointeur vers la valeur de l'offset en y relatif
* \param[in] t, temps instantanne
* \param[in] vit_X, valeur de la vitesse de l'objet selon x
* \param[in] vit_Y, valeur de la vitesse de l'objet selon y
* \param[in] pSurface, surface de l'objet en mouvement
* \returns void
*/
void calculPositionRel(int* xRel, int* yRel, int t, float vit_X, float vit_Y, SDL_Surface* pSurface)
{
	const double g = 9.81; //Acceleration de 9.81 m/s 
	//On calcul les �carts relatifs sur x et y
	*xRel = (int)(vit_X * t);	//calcul de la position relative en x
	*yRel = (int)((vit_Y * t) - ((g*t*t) / 2000));	//calcul de la position relative en y
	//On calcule maintenant les valeurs de x et y de la surface
	pSurface->clip_rect.x += (*xRel);	//Mise � jours de la valeur de x de la surface par rapport � la position relative
	pSurface->clip_rect.y -= (*yRel);	//Mise � jours de la valeur de y de la surface par rapport � la position relative
}


/**
* \fn void finDeplacement(Input* pInput, Worms* worms, Terrain* map, int check)
* \brief Gere la fin de saut en cas de collision.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] pWorms, pointeur du worms a deplacer
* \param[in] pMapTerrain, pointeur vers la structure de la map
* \param[in] checkJump, fonctionnement particulier si juste apres checkDeplacement, indique si un saut est possible
* \returns void
*/
void finDeplacement(Input* pInput, Worms* pWorms, Terrain* pMapTerrain, int checkJump)
{
	//Remise � 0 des valeurs absolues x et y du worms
	if (pWorms->xAbs != pWorms->wormsSurface->clip_rect.x || (pWorms->dir == DOWN || pWorms->dir == UP))	//Si la valeur absolu de x du worms est differente de celle de la surface (on fait pas du surplace) ou que le worms se dirige vers le haut ou le bas (saut sur place)
		pWorms->yAbs = pWorms->wormsSurface->clip_rect.y;	//Valeur absolu de y  = valeur de y de la surface du worms
	pWorms->xAbs = pWorms->wormsSurface->clip_rect.x;	//Valeur absolu de x  = valeur de x de la surface du worms

	//Remise � 0 des booleens
	if (checkJump || (pInput->jumpOnGoing && checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, DOWN)))	//Si checkJump est � 1 (un saut n'est pas possible) ou que l'on touche le sol (donc fin de d�placement)
		pInput->jumpOnGoing = 0;	//Remise � 0 du booleen indiquant un saut en cours
	if (pInput->jump)	//Si le booleen de la touche espace est � 1
		pInput->jump = 0;	//Remise � 0 du booleen de la touche espace
	
	//Remise � 0 des directions
	if (pInput->direction != NONE)	//Si la direction clavier n'est pas NONE
		pInput->direction = NONE;	//Remise � NONE de la direction clavier
	pWorms->dir = DOWN;	//Set de la direction du worms vers le bas pour detecter le sol et une eventuelle chute

	//Remise � 0 des valeurs des vecteurs vitesse
	pWorms->vitx = 0;	//Remise � 0 de la valeur du vecteur de vitesse horizontale
	pWorms->vity = 0;	//Remise � 0 de la valeur du vecteur de vitesse verticale
}


/**
* \fn enum DIRECTION calculDirectionSaut(int xRel, int yRel, enum DIRECTION sensSaut,Input* pInput)
*
* \brief Determine la direction du deplacement de fa�on precise (selon les diagonales).
*
* \param[in] xRel, valeurs relatives de la position en x pendant le saut
* \param[in] yRel, valeurs relatives de la position en y pendant le saut
* \param[in] sensObjet, sens du worms et donc du saut (vers la droite ou vers la gauche)
* \param[in] pInput, structure d'inputs
* \return DIRECTION, direction du saut, NONE sinon
*/
enum DIRECTION calculDirectionDeplacement(int xRel, int yRel, enum DIRECTION sensObjet, Input* pInput)
{
	static int yPrec = 0;
	if (pInput->jumpOnGoing)
	{
		switch (sensObjet)
		{
		case UP:
			if (yRel >= yPrec)
				yPrec = yRel;
			else if (yRel < 46 && yRel != 0)
			{
				yPrec = 0;
				return DOWN;
			}
			break;
		case UPLEFT:
			if (xRel > -59 && yRel < 62)
				return UPLEFT;
			else if (xRel < -59 && yRel < 62)
				return DLEFT;
			break;
		case UPRIGHT:
			if (xRel <= 59 && yRel < 62)
				return UPRIGHT;
			else if (xRel > 59 && yRel < 62)
				return DRIGHT;
			break;
		}
	}
	else if (yRel < 0)
	{
		return DOWN;
	}
	yPrec = 0;
	return sensObjet;
}



/**
* \fn int checkJump(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION dir)
*
* \brief Deteremine la faisabilite d'un saut.
*
* \param[in] pSurfaceMap, surface de la map
* \param[in] pSurfaceMotion, surface en mouvement dans la map
* \return int, 1 = saut non possible, 0 = saut possible
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
		break;
	case UPRIGHT:
		pSurfaceMotion->clip_rect.y -= 1;
		pSurfaceMotion->clip_rect.x += 1;
		if (checkDeplacement(pSurfaceMap, pSurfaceMotion, UP))
		{
			pSurfaceMotion->clip_rect.y += 1;
			pSurfaceMotion->clip_rect.x -= 1;
			return 1;
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
	if (detectionCollisionSurfaceV2(pSurfaceMap, pSurfaceMotion, &dir, 0))
	{
		pSurfaceMotion->clip_rect.y = y;
		pSurfaceMotion->clip_rect.x = x;
		if (dir == direction)
			return 1;
	}
	pSurfaceMotion->clip_rect.y = y;
	pSurfaceMotion->clip_rect.x = x;
	return 0;
}

/**
* \fn void initGameWorms(Worms** worms, Input* pInput, Terrain* map, SDL_Texture* pTextureDisplay, SDL_Renderer* pRenderer, SDL_Rect* pCamera)
* \brief Initialise les worms de la partie jusqu'au sol.
*
* \param[in] wormsTab, pointeur vers la structure du worms en cours de jeu pour modifier ses param�tres de position.
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'�tat des inputs.
* \param[in] pRenderer pointeur pWindow pour r�cup�rer les informations relative � la fen�tre.
* \param[in] pMapTerrain, pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqu� la camera.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \returns void
*/
void initGameWorms(Worms** wormsTab, Input* pInput, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Renderer* pRenderer, SDL_Rect* pCamera)
{
	int i = 0;
	srand((int)time(NULL));
	for (i = 0; i < globalVar.nbWormsEquipe; i++)
	{
		wormsTab[i]->wormsSurface->clip_rect.x = wormsTab[i]->wormsRect.x = rand_a_b(0, (pMapTerrain->imageMapSurface->w - wormsTab[i]->wormsSurface->w));
		while (!checkDeplacement(pMapTerrain->imageMapSurface, wormsTab[i]->wormsSurface, DOWN))
		{
			gestionPhysique(pMapTerrain, pTextureDisplay, pInput, 0, wormsTab[i]);
			updateScreen(pRenderer, 2, 0, pMapTerrain, 1, pTextureDisplay, pCamera, NULL);
		}
		while (detectionCollisionSurface(pMapTerrain->imageMapSurface, wormsTab[i]->wormsSurface))
		{
			wormsTab[i]->wormsSurface->clip_rect.y -= 1;
			updateGlobaleTexture(pMapTerrain->imageMapSurface, wormsTab[i]->wormsSurface, pTextureDisplay, &wormsTab[i]->wormsRect);
			updateScreen(pRenderer, 2, 0, pMapTerrain, 1, pTextureDisplay, pCamera, NULL);
		}
	}

}

/**
* \fn int rand_a_b(int a, int b)
* \brief Retourne un nombre aleatoire appartenant a [a;b[.
*
* \param[in] a, borne inferieur, inclus.
* \param[in] b, borne superieure, exclus.
* \returns rand, valeur aleatoire
*/
int rand_a_b(int a, int b)
{
	return rand() % (b - a) + a;
}