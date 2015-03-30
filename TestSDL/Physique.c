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
	int retournement = 0, collision = 0;;
	static int  t = 0, start = 0;
	static int xPrec = 0, yPrec = 0, finAnim = 0;
	static enum DIRECTION directionPrec = NONE;
	int onGround = !checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, DOWN);
	if (pInput->jumpOnGoing || pInput->direction != NONE || pInput->jump /*|| !finAnim */ || !onGround)
	{
		if (!start && !checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, pWorms->dir))
		{
			finDeplacement(pInput, pWorms, pMapTerrain, 1, 1);
			return;
		}
		if (pInput->jumpOnGoing)
		{
			/*On remet à zero x et y par rapport à sa position absolu de départ*/
			start = 1;
			pWorms->wormsSurface->clip_rect.x = pWorms->xAbs;
			pWorms->wormsSurface->clip_rect.y = pWorms->yAbs;
		}

		/////////////////////////////////////////////////////////////
		////////////         Réalisation du saut         ////////////
		/////////////////////////////////////////////////////////////

		/*Calcul des positions relatives*/
		if (pInput->jumpOnGoing || !onGround)
		{
			calculPositionRel(&xRel, &yRel, t, pWorms->vitx, pWorms->vity, pWorms->wormsSurface);
		}
		t += 7;

		/*Calcul d'un eventuel retournement*/
		if (!retournement && !pInput->jumpOnGoing)
			retournement = retournementWorms(pInput, pWorms);

		/*Fonction de déplacement du worms si non saut*/
		if (!pInput->jumpOnGoing && !retournement)
		{
			finAnim = deplacementWorms(pInput, pWorms, pMapTerrain->imageMapSurface,0);
		}

		/*Determination de la direction du saut*/
		pWorms->dir = calculDirectionDeplacement((pWorms->wormsSurface->clip_rect.x - xPrec), (pWorms->wormsSurface->clip_rect.y - yPrec));
		if ((pInput->direction == LEFT || pInput->direction == RIGHT) && (pWorms->dir == DRIGHT || pWorms->dir == DLEFT))
			pWorms->dir = DOWN;
		/*Gestion de la collision*/
		if (retournement)
		{
			swapSurface(pWorms);
			if (!checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, pWorms->dirSurface))
			{
				swapSurface(pWorms);
				deplacementWorms(pInput, pWorms, pMapTerrain->imageMapSurface, 0);
				retournement = 0;
			}
			retournement = 0;
		}
		collision = gestionCollision(pInput->acceleration, pMapTerrain->imageMapSurface, pWorms->wormsSurface, &pWorms->dir);

		/*Gestion de fin de mouvement*/
		if (collision)
		{
			finDeplacement(pInput, pWorms, pMapTerrain, 0, finAnim);
			retournement = 0;
			t = 0;
			start = 0;
		}

		/*Mise à jour de l'affichage*/
		updateGlobaleTexture(pMapTerrain->imageMapSurface, pWorms->wormsSurface, pTextureDisplay, &pWorms->wormsRect);

		/*Indicateurs de déplacement et de raffraichissement de l'image*/
		pInput->deplacement = 1;
		pInput->raffraichissement = 1;
		pInput->direction = NONE;
		xPrec = pWorms->wormsRect.x;
		yPrec = pWorms->wormsRect.y;
	}
	else
	{
		pInput->deplacement = 0;
		finDeplacement(pInput, pWorms, pMapTerrain, 0, finAnim);
		t = 0;
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

	/*On calcul les écarts relatifs sur x et y*/
	*xRel = (int)(vit_X * t);	//calcul de la position relative en x
	*yRel = (int)((vit_Y * t) - ((g*t*t) / 2000));	//calcul de la position relative en y

	/*On met à jour maintenant les valeurs de x et y de la surface*/
	pSurface->clip_rect.x += (*xRel);	//Mise à jours de la valeur de x de la surface par rapport à la position relative
	pSurface->clip_rect.y -= (*yRel);	//Mise à jours de la valeur de y de la surface par rapport à la position relative
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
void finDeplacement(Input* pInput, Worms* pWorms, Terrain* pMapTerrain, int checkJump, int finAnim)
{
	/*Remise à 0 des valeurs absolues x et y du worms*/
	if ((pWorms->xAbs != pWorms->wormsSurface->clip_rect.x) || pWorms->dir == DOWN || pWorms->dir == UP)	//Si la valeur absolu de x du worms est differente de celle de la surface (on fait pas du surplace) ou que le worms se dirige vers le haut ou le bas (saut sur place)
		pWorms->yAbs = pWorms->wormsSurface->clip_rect.y;	//Valeur absolu de y  = valeur de y de la surface du worms
	pWorms->xAbs = pWorms->wormsSurface->clip_rect.x;	//Valeur absolu de x  = valeur de x de la surface du worms

	/*Remise à 0 des booleens*/
	if (checkJump || (pInput->jumpOnGoing && !checkDeplacement(pMapTerrain->imageMapSurface, pWorms->wormsSurface, DOWN)))	//Si checkJump est à 1 (un saut n'est pas possible) ou que l'on touche le sol (donc fin de déplacement)
		pInput->jumpOnGoing = 0;	//Remise à 0 du booleen indiquant un saut en cours
	if (pInput->jump)	//Si le booleen de la touche espace est à 1
		pInput->jump = 0;	//Remise à 0 du booleen de la touche espace

	/*Remise à 0 des directions*/
	if (pInput->direction != NONE && finAnim)	//Si la direction clavier n'est pas NONE
		pInput->direction = NONE;	//Remise à NONE de la direction clavier
	pWorms->dir = DOWN;	//Set de la direction du worms vers le bas pour detecter le sol et une eventuelle chute

	/*Remise à 0 des valeurs des vecteurs vitesse*/
	pWorms->vitx = 0;	//Remise à 0 de la valeur du vecteur de vitesse horizontale
	pWorms->vity = 0;	//Remise à 0 de la valeur du vecteur de vitesse verticale
}


/**
* \fn enum DIRECTION calculDirectionSaut(int dx, int dy)
*
* \brief Determine la direction du deplacement de façon precise (selon les diagonales).
*
* \param[in] dx, ecart entre deux valeurs de x
* \param[in] dy, ecart entre deux valeurs de y
* \return DIRECTION, direction du deplacement
*/
enum DIRECTION calculDirectionDeplacement(int dx, int dy)
{
	if (dx == 0)
	{
		if (dy >= 0)
			return DOWN;
		else return UP;
	}
	else if (dy == 0)
	{
		if (dx > 0)
			return RIGHT;
		else return LEFT;
	}
	else if (dx > 0)
	{
		if (dy > 0)
			return DRIGHT;
		else return UPRIGHT;
	}
	else if (dy < 0)
		return UPLEFT;
	else return DLEFT;
}



/**
* \fn int checkJump(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION dir)
*
* \brief Deteremine la faisabilite d'un saut.
*
* \param[in] pSurfaceMap, surface de la map
* \param[in] pSurfaceMotion, surface en mouvement dans la map
* \return int, 1 = saut possible, 0 = saut non possible
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
		if (!checkDeplacement(pSurfaceMap, pSurfaceMotion, UP))
		{
			pSurfaceMotion->clip_rect.x -= 1;
			return 0;
		}
		break;
	case LEFT:
		pSurfaceMotion->clip_rect.x -= 1;
		if (!checkDeplacement(pSurfaceMap, pSurfaceMotion, UP))
		{
			pSurfaceMotion->clip_rect.x += 1;
			return 0;
		}
		break;
	case DOWN:
		pSurfaceMotion->clip_rect.y += 1;
		break;
	}
	if (detectionCollisionSurfaceV2(pSurfaceMap, pSurfaceMotion, &dir, 1))
	{
		pSurfaceMotion->clip_rect.y = y;
		pSurfaceMotion->clip_rect.x = x;
		if ((direction == LEFT || direction == RIGHT || direction == UP) && dir == DOWN)
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
		while (checkDeplacement(pMapTerrain->imageMapSurface, wormsTab[i]->wormsSurface, DOWN))
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
	if (logFile != NULL)
		fprintf(logFile, "initGameWorms : DONE.\n\n");
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