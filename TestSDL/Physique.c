#include "physique.h"
#include "AffichageGeneral.h"







//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                Gestion de la physique                  /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int gestionPhysique(Terrain* map, SDL_Texture* display, Input* pInput, ...)
{
	Worms* worms = NULL;
	//Arme* weapon = NULL;
	va_list list;

	va_start(list, pInput);
	switch (va_arg(list, int))
	{
	case 0:
		worms = va_arg(list, Worms*);
		gestionPhysiqueWorms(pInput, worms, map, display);
		break;
	case 1:
		//cas d'une arme
		break;
	}
	va_end(list);
	return 0;
}


/**
* \fn void gestionPhysiqueWorms(Input* pInput, Worms* worms, Terrain* map, SDL_Texture* display)
* \brief Gere la physique autour d'un worms.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] worms, pointeur du worms a deplacer
* \param[in] map, pointeur vers la structure de la map
* \param[in] display, texture globale pour actualiser la position du worms dans celle-ci
* \returns void
*/
void gestionPhysiqueWorms(Input* pInput, Worms* worms, Terrain* map, SDL_Texture* display)
{
	int xRel = 0, yRel = 0;
	static char retournement = 0;
	static int  t = 0, start = 0, swap = 0;
	int collision = 0;
	if (pInput->jumpOnGoing)
	{
		if (!start && checkDeplacement(map->imageMapSurface, worms->wormsSurface, worms->dir))
		{
			finDeplacement(pInput, worms, map, 1);
			return;
		}
		start = 1;
		//On remet à zero x et y par rapport à sa position absolu de départ
		worms->wormsSurface->clip_rect.x = worms->xAbs;
		worms->wormsSurface->clip_rect.y = worms->yAbs;
	}

	/////////////////////////////////////////////////////////////
	////////////         Réalisation du saut         ////////////
	/////////////////////////////////////////////////////////////

	/*Calcul des positions relatives*/
	calculPositionRel(&xRel, &yRel, t, worms->vitx, worms->vity, worms->wormsSurface);
	t += 7;

	/*Determination de la direction du saut*/
	worms->dir = calculDirectionDeplacement(xRel, yRel, worms->dir, pInput);

	/*Calcul d'un eventuel retournement*/
	if (!retournement && !pInput->jumpOnGoing)
		retournement = retournementWorms(pInput, worms);

	/*Fonction de déplacement du worms si non saut*/
	if (!pInput->jumpOnGoing && !retournement)
		deplacementWorms(pInput, worms, map->imageMapSurface);

	/*Gestion de la collision*/
	if (retournement)
	{
		if (!swap)
			swapSurface(worms);
		if (worms->dir != DOWN && checkDeplacement(map->imageMapSurface, worms->wormsSurface, UP))
		{
			swapSurface(worms);
			deplacementWorms(pInput, worms, map->imageMapSurface);
			retournement = 0;
		}
		swap = 1;
		retournement = 0;
	}
	collision = gestionCollision(pInput->acceleration, worms->wormsSurface, map->imageMapSurface, &worms->dir, 0);

	/*Gestion de fin de mouvement*/
	if (collision)
	{
		finDeplacement(pInput, worms, map, 0);
		retournement = 0;
		t = 0;
		start = 0;
		swap = 0;
	}
	while (checkDeplacement(map->imageMapSurface, worms->wormsSurface, worms->dirSurface))
	{
		if (worms->dirSurface == RIGHT)
			worms->wormsSurface->clip_rect.x -= 1;
		else worms->wormsSurface->clip_rect.x += 1;
	}
	/*Mise à jour de l'affichage*/
	if (!checkDeplacement(map->imageMapSurface, worms->wormsSurface, UP) || !checkDeplacement(map->imageMapSurface, worms->wormsSurface, LEFT) || !checkDeplacement(map->imageMapSurface, worms->wormsSurface, RIGHT))
		updateGlobaleTexture(map->imageMapSurface, worms->wormsSurface, display, &worms->wormsRect);
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
	const double g = 9.81; //9.81 m/s ==> 37081.8 pix/s = 37 pix/ms
	//On calcul les écarts relatifs sur x et y
	*xRel = (int)(vit_X * t);
	*yRel = (int)((vit_Y * t) - ((g*t*t) / 2000));
	//On calcule maintenant les valeurs de x et y
	pSurface->clip_rect.x += (*xRel);
	pSurface->clip_rect.y -= (*yRel);
}


/**
* \fn void finDeplacement(Input* pInput, Worms* worms, Terrain* map, int check)
* \brief Gere la fin de saut en cas de collision.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] worms, pointeur du worms a deplacer
* \param[in] map, pointeur vers la structure de la map
* \param[in] check, fonctionnement particulier si juste apres checkDeplacement
* \returns void
*/
void finDeplacement(Input* pInput, Worms* worms, Terrain* map, int check)
{
	if (worms->xAbs != worms->wormsSurface->clip_rect.x || (worms->dir == DOWN || worms->dir == UP))
		worms->yAbs = worms->wormsSurface->clip_rect.y;
	worms->xAbs = worms->wormsSurface->clip_rect.x;

	if (check || (pInput->jumpOnGoing && checkDeplacement(map->imageMapSurface, worms->wormsSurface, DOWN)))
		pInput->jumpOnGoing = 0;
	if (pInput->jump)
		pInput->jump = 0;
	if (pInput->direction != NONE)
		pInput->direction = NONE;
	worms->vitx = 0;
	worms->vity = 0;
	worms->dir = DOWN;
}


/**
* \fn enum DIRECTION calculDirectionSaut(int xRel, int yRel, enum DIRECTION sensSaut,Input* pInput)
*
* \brief Determine la direction du deplacement de façon precise (selon les diagonales).
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