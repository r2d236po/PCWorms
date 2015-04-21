#include "KaamEngine.h"
#include "AffichageGeneral.h"
#include "input.h"

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                    KaamEngine V1.0                     /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void KaamEngine()
{

}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                          Init                          /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn void KaamInitGame(Worms** wormsTab, SDL_Surface* pSurfaceMap)
* \brief Init worms positions and put them to the ground.
*
* \param[in] wormsTab, array of worms.
* \param[in] pSurfaceMap, pointer to the map's surface.
* \returns void
* \remarks A refaire pour chaque equipe
*/
void KaamInitGame(Worms** wormsTab, SDL_Surface* pSurfaceMap)
{
	int indexWorms = 0;
	srand((int)time(NULL));
	for (indexWorms = 0; indexWorms < globalVar.nbWormsEquipe*globalVar.nbEquipe; indexWorms++)
	{
		wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.x = rand_a_b(0, (pSurfaceMap->w - wormsTab[indexWorms]->wormsObject->objectSurface->w - 1));
		wormsTab[indexWorms]->wormsObject->objectBox.x = wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.x;
		wormsTab[indexWorms]->wormsObject->objectBox.y = wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.y = 0;
		resetAbsoluteCoordinates(wormsTab[indexWorms]->wormsObject->objectSurface, &wormsTab[indexWorms]->wormsObject->absoluteCoordinate.x, &wormsTab[indexWorms]->wormsObject->absoluteCoordinate.y);
	}
	fprintf(logFile, "KaamInitGame : DONE.\n\n");
}

/**
* \fn KaamObject* KaamInitObject(SDL_Rect rectSurface, float initSpeedX, float initSpeedY, enum DIRECTION initDirection, int weapon)
* \brief Initialiazes a KaamObject structure.
*
* \param[in] rectSurface, rect of the size and position of the surface to create.
* \param[in] initSpeedX, speed along X axis to initialises the object with.
* \param[in] initSpeedY, speed along Y axis to initialises the object with.
* \param[in] initDirection, direction of the object.
* \param[in] weapon, indicate whether of not if the object is a weapon.
* \returns pointer to the KaamObject created, NULL if error was encountered.
* \remarks The surface is just initiliased, there is no pixels in it. It should be done after calling this function.
*		   Use KaamInitSurfaceObject.
*/
KaamObject* KaamInitObject(SDL_Rect rectSurface, float initSpeedX, float initSpeedY, enum DIRECTION initDirection, int weapon)
{
	KaamObject* objectTemp = (KaamObject*)malloc(sizeof(KaamObject));
	if (objectTemp == NULL)
	{
		fprintf(logFile, "KaamInitObject : FAILURE, unable to allocate memory.\n\n");
		return NULL;
	}

	/*Init object surface*/
	objectTemp->objectSurface = SDL_CreateRGBSurface(0, rectSurface.w, rectSurface.h, 32, RMASK, GMASK, BMASK, AMASK);
	if (objectTemp->objectSurface == NULL)
	{
		fprintf(logFile, "KaamInitObject : FAILURE, createRGBSurface : %s.\n\n", SDL_GetError());
		KaamDestroyObject(&objectTemp);
		return NULL;
	}
	objectTemp->objectSurface->clip_rect.x = rectSurface.x;
	objectTemp->objectSurface->clip_rect.y = rectSurface.y;

	/*Init absolute coordinate*/
	objectTemp->absoluteCoordinate.x = objectTemp->objectSurface->clip_rect.x;
	objectTemp->absoluteCoordinate.y = objectTemp->objectSurface->clip_rect.y;

	/*Init motion direction*/
	objectTemp->motionDirection = initDirection;
	objectTemp->startMotion = 0;

	/*Init side*/
	objectTemp->leftOk = -1;
	objectTemp->rightOk = -1;

	/*Init speeds*/
	objectTemp->Xspeed = initSpeedX;
	objectTemp->Yspeed = initSpeedY;

	/*Init relative time*/
	objectTemp->relativeTime = 0;
	objectTemp->falling = 0;

	/*Init react to bomb*/
	objectTemp->reactToBomb = 0;
	objectTemp->rebound = 0;

	/*Init weapon*/
	objectTemp->weapon = weapon;

	/*Init Object box*/
	objectTemp->objectBox = initRect(objectTemp->objectSurface->clip_rect.x,
		objectTemp->objectSurface->clip_rect.y,
		objectTemp->objectSurface->w,
		objectTemp->objectSurface->h);

	fprintf(logFile, "KaamInitObject : SUCCESS.\n\n");
	return objectTemp;
}

/**
* \fn void KaamDestroyObject(KaamObject** p_pObject)
* \brief Deallocate a KaamObject.
*
* \param[in] p_pObject, adress of the pointer to the object to destroy.
* \returns void
*/
void KaamDestroyObject(KaamObject** p_pObject)
{
	if ((*p_pObject)->objectSurface != NULL)
	{
		SDL_FreeSurface((*p_pObject)->objectSurface);
		(*p_pObject)->objectSurface = NULL;
	}
	if ((*p_pObject) != NULL)
	{
		free((*p_pObject));
		(*p_pObject) = NULL;
	}
	fprintf(logFile, "KaamDestroyObject : DONE. \n\n");
}

/**
* \fn void KaamInitSurfaceObject(KaamObject* pObject, Uint32* pixels, Uint32 nbPixels)
* \brief Initialises the pixels of a surface.
*
* \param[in] pObject, pointer to the object.
* \param[in] pixels, array of pixels.
* \param[in] nbPixels, number of pixels.
* \returns void
*/
void KaamInitSurfaceObject(KaamObject* pObject, Uint32* pixels, Uint32 nbPixels)
{
	memcpy(pObject->objectSurface->pixels, pixels, nbPixels*sizeof(Uint32));
}










//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Physic Management                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn void KaamPhysicManagement(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap)
* \brief  Manages physics of the worms (ground deplacement + gravity + jump).
*
* \param[in] pInput, pointer to the structure of inputs.
* \param[in] pWorms, pointer to the worms to move.
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \returns void
*/
void KaamPhysicManagement(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap)
{
	if (pInput->jumpOnGoing)
	{
		KaamNonLinearMotion(pInput, pSurfaceMap, pObject, 0);
	}
	else if (pObject->reactToBomb == 1)
	{
		KaamNonLinearMotion(pInput, pSurfaceMap, pObject, 1);
	}
	else if (pObject->weapon == 1)
	{
		KaamNonLinearMotion(pInput, pSurfaceMap, pObject, 1);
		if (dxProcess(pObject) == 0 && dyProcess(pObject) == 0)
			KaamGravityManagement(pSurfaceMap, pObject);
	}
	else// /!\ deplacement OK, gravité OK, saut NON OK
	{
		KaamGroundMotion(pInput, pObject, pSurfaceMap);
		KaamGravityManagement(pSurfaceMap, pObject);
	}
	pInput->deplacement = (MY_ABS(dxProcess(pObject)) > 0 || MY_ABS(dyProcess(pObject)) > 0);
	resetAbsoluteCoordinates(pObject->objectSurface, &pObject->objectBox.x, &pObject->objectBox.y);
}

/**
* \fn void KaamGravityManagement(SDL_Surface* pSurfaceMap, KaamObject* pObject)
* \brief Applies gravity for any KaamObject.
*
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pObject, pointer to the KaamObject which the gravity should be applied to.
* \returns void
*/
void KaamGravityManagement(SDL_Surface* pSurfaceMap, KaamObject* pObject)
{
	int onGround = testGround(pSurfaceMap, pObject->objectSurface, 1);
	enum DIRECTION downDirection = DOWN;

	if (!onGround)
	{
		setSurfaceAbsoluteCoordinates(pObject->objectSurface, pObject->absoluteCoordinate.x, pObject->absoluteCoordinate.y);
		setSurfaceRelativeCoordinates(pObject->objectSurface, pObject->relativeTime, 0.0, 0.0);
		pObject->relativeTime += 7;
		pObject->falling = 1;
		if (KaamCollisionManagement(pSurfaceMap, pObject->objectSurface, &downDirection))
		{
			pObject->relativeTime = 0;
			pObject->falling = 0;
			setSideMotionPossibility(pObject, pSurfaceMap);
			resetAbsoluteCoordinates(pObject->objectSurface, &pObject->absoluteCoordinate.x, &pObject->absoluteCoordinate.y);
		}
	}
	else
	{
		resetAbsoluteCoordinates(pObject->objectSurface, &pObject->absoluteCoordinate.x, &pObject->absoluteCoordinate.y);
		pObject->relativeTime = 0;
		pObject->falling = 0;
	}
}

/**
* \fn void KaamWormsMotionManagement(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap)
* \brief Manages the motion relative to a worms.
*
* \param[in] pInput, pointer to the structure of inputs.
* \param[in] pWorms, pointer to the worms to move.
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \returns void
*/
void KaamWormsMotionManagement(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap)
{
	int launchAnim = pInput->direction != NONE && !pInput->jumpOnGoing && (pWorms->wormsObject->falling == 0) && pInput->direction != DOWN;
	int swap = 0;
	if (launchAnim)
	{
		if (!pInput->jumpOnGoing)
			swap = swapManagement(pInput, pWorms, pSurfaceMap);
		gestionAnimationWorms(pWorms, pSurfaceMap, swap);
	}
	KaamPhysicManagement(pInput, pWorms->wormsObject, pSurfaceMap);
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Motion Management	                 /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn void KaamGroundMotionReset(Input* pInput, KaamObject* pObject)
* \brief Reset all variables relatives to a ground movement.
*
* \param[in] pInput, pointer to the input structure
* \param[in] pObject, pointer to the worms to swap
*/
void KaamGroundMotionReset(Input* pInput, KaamObject* pObject)
{
	resetInputs(pInput);
	resetAbsoluteCoordinates(pObject->objectSurface, &pObject->absoluteCoordinate.x, &pObject->absoluteCoordinate.y);
}

/**
* \fn void KaamGroundCollisionProcess(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap, int deplacement)
* \brief Manage collision on the ground.
*
* \param[in] pInput, pointer to the input structure
* \param[in] pObject, pointer to the worms to swap
* \param[in] pSurfaceMap, pointer to the map surface
* \param[in] deplacement, value of the movement
* \returns void
*/
void KaamGroundCollisionProcess(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap, int deplacement)
{
	enum DIRECTION direction = pInput->direction;
	int indexBoucle = 0;
	pObject->objectSurface->clip_rect.x += deplacement;
	pObject->motionDirection = motionDirectionProcess(dxProcess(pObject), dyProcess(pObject));
	if (collisionSurfaceWithMap(pSurfaceMap, pObject->objectSurface, &direction, 0))
	{
		if (direction == DOWN)
		{
			pObject->objectSurface->clip_rect.y -= 1;
		}
	}
	while (indexBoucle < 60 && collisionSurfaceWithMapBasic(pSurfaceMap, pObject->objectSurface))
	{
		if (pObject->motionDirection == RIGHT)
			pObject->objectSurface->clip_rect.x -= 1;
		else pObject->objectSurface->clip_rect.x += 1;
		indexBoucle++;
	}
}

/**
* \fn void KaamGroundMotion(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap)
* \brief Move the worms on the ground along the X axis.
*
* \param[in] pInput, pointer to the input structure
* \param[in] pObject, pointer to the worms to swap
* \param[in] pSurfaceMap, pointer to the map surface
* \returns void
*/
void KaamGroundMotion(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap)
{
	int authorizeMovement = (pInput->direction != NONE && pInput->direction != UP && !pInput->jumpOnGoing);

	if (authorizeMovement && pObject->falling == 0)
	{
		if (!sideInitialized(pObject))
			setSideMotionPossibility(pObject, pSurfaceMap);
		switch (pInput->direction)
		{
		case RIGHT:
			if (pObject->rightOk)
				pObject->objectSurface->clip_rect.x += groundSpeed;
			break;
		case LEFT:
			if (pObject->leftOk)
				pObject->objectSurface->clip_rect.x -= groundSpeed;
			break;
		}
		if (pObject->rightOk > 1 || pObject->leftOk > 1)
			pObject->objectSurface->clip_rect.y -= 1;
		KaamGroundMotionReset(pInput, pObject);
		setSideMotionPossibility(pObject, pSurfaceMap);
		pObject->motionDirection = motionDirectionProcess(dxProcess(pObject), 0);
	}
}

/**
* \fn void KaamNonLinearMotion(Input* pInput, SDL_Surface* pSurfaceMap, KaamObject* pObject, int allowRebound)
* \brief Manages the jump of a worms.
*
* \param[in] pInput, pointer to the structure of inputs.
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pObject, pointer to the object to move.
* \param[in] allowRebound, selects if rebound are allowed in the motion.
* \returns void
*/
void KaamNonLinearMotion(Input* pInput, SDL_Surface* pSurfaceMap, KaamObject* pObject, int allowRebound)
{
	int stopReact = allowRebound && pObject->relativeTime >= 42 && (pObject->Yspeed == 0);
	enum DIRECTION directionBeforeCollision;
	if (!pObject->startMotion)
	{
		resetAbsoluteCoordinates(pObject->objectSurface, &pObject->precedentCoordinate.x, &pObject->precedentCoordinate.y);
		if ((pObject->startMotion = getStartMotion(pInput, pObject, pSurfaceMap)) == 0)
			pObject->reactToBomb = 0;
	}
	if (pObject->startMotion)
	{
		setSurfaceAbsoluteCoordinates(pObject->objectSurface, pObject->absoluteCoordinate.x, pObject->absoluteCoordinate.y);
		if ((pObject->Yspeed <= 0) && testGround(pSurfaceMap, pObject->objectSurface, 1))
		{
			float relativeX = (float)(pObject->Xspeed * pObject->relativeTime);
			pObject->objectSurface->clip_rect.x += (int)relativeX;
			pObject->Yspeed = 0.0;
		}
		else setSurfaceRelativeCoordinates(pObject->objectSurface, pObject->relativeTime, pObject->Xspeed, pObject->Yspeed);
		pObject->motionDirection = motionDirectionProcess((pObject->objectSurface->clip_rect.x - pObject->precedentCoordinate.x), (pObject->objectSurface->clip_rect.y - pObject->precedentCoordinate.y));
		directionBeforeCollision = pObject->motionDirection;
		pObject->relativeTime += 7;
		if (stopReact || pObject->relativeTime > 7 && KaamCollisionManagement(pSurfaceMap, pObject->objectSurface, &pObject->motionDirection))
		{
			if (!KaamCollisionReaction(pObject, directionBeforeCollision, allowRebound))
			{
				pObject->rebound = 0;
				resetMotionVariables(pInput, pObject);
				pObject->reactToBomb = 0;
				setSideMotionPossibility(pObject, pSurfaceMap);
			}
		}
		resetAbsoluteCoordinates(pObject->objectSurface, &pObject->precedentCoordinate.x, &pObject->precedentCoordinate.y);
	}
	else resetMotionVariables(pInput, pObject);
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Collision Management                   /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int KaamCollisionManagement(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection)
* \brief replace le worms en cas de collision.
*
* \param[in] pSurfaceMotion, surface en deplacement
* \param[in] pSurfaceMap, surface de la map
* \param[in] pDirection, direction du deplacement du worms, peut etre modifie par la fonction
* \returns int collision, indique s'il y a eu collision
*/
int KaamCollisionManagement(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection)
{
	int t = 0;
	int collision = 0;
	while (t < 60 && collisionSurfaceWithMap(pSurfaceMap, pSurfaceMotion, pDirection, 0))	//Tant qu'on détecte une collision et que la boucle ne s'est pas effectuée plus de 60 fois (évite des boucles infinies)
	{
		switch (*pDirection)	//Switch de la valeur de derriere le pointeur pDirection, indiquant le sens de détection de la collision
		{
		case RIGHT:	//Cas d'une détection à droite
			pSurfaceMotion->clip_rect.x -= 1;	//Soustraction de la vitesse de déplacement sur l'axe x (0à gauche de l'écran et x à droite de l'écran)
			break;
		case LEFT:	//Cas d'une détection à gauche
			pSurfaceMotion->clip_rect.x += 1;	//Addition de la vitesse de déplacement sur l'axe x
			break;
		case DOWN:	//Cas d'une détection en bas
			pSurfaceMotion->clip_rect.y -= 1;	//Soustraction de la vitesse de déplacement sur l'axe y (0 en haut de l'écran et y en bas de l'écran)
			break;
		case UP:	//Cas d'une détection en hauteur
			pSurfaceMotion->clip_rect.y += 1;	//Addition de la vitesse de déplacement sur l'axe y
			break;
		case UPRIGHT:	//Cas d'une détection en diagonale haute droite
			pSurfaceMotion->clip_rect.x -= 1;	//Soustraction de la vitesse de déplacement sur l'axe x
			pSurfaceMotion->clip_rect.y += 1;	//Addition de la vitesse de déplacement sur l'axe y
			break;
		case UPLEFT:	//Cas d'une détection en diagonale haute gauche
			pSurfaceMotion->clip_rect.x += 1;	//Addition de la vitesse de déplacement sur l'axe x
			pSurfaceMotion->clip_rect.y += 1;	//Addition de la vitesse de déplacement sur l'axe y
			break;
		case DRIGHT:	//Cas d'une détection en diagonale basse droite
			pSurfaceMotion->clip_rect.x -= 1;	//Soustraction de la vitesse de déplacement sur l'axe x
			pSurfaceMotion->clip_rect.y -= 1;	//Soustraction de la vitesse de déplacement sur l'axe y 
			break;
		case DLEFT:	//Cas d'une détection en diagonale basse gauche
			pSurfaceMotion->clip_rect.x += 1;	//Addition de la vitesse de déplacement sur l'axe x
			pSurfaceMotion->clip_rect.y -= 1;	//Soustraction de la vitesse de déplacement sur l'axe y 
			break;
		default:
			break;
		}
		t++;	//Incrémentation du limiteur de boucle
		collision = 1;	//Mise à 1 de l'indicateur de collision
	}
	return collision;	//Return l'indice de collision, 0 = pas de collision, 1 = collision
}

/**
* \fn int KaamCollisionReaction(KaamObject* pObject, enum DIRECTION directionBeforeCollision, int allowRebound, int* rebound)
* \brief Manages the rebound and reaction to a collision.
*
* \param[in] pObject, pointer to the object to move.
* \param[in] directionBeforeCollision, direction of the movement before collision.
* \param[in] allowRebound, selects if rebound are allowed in the motion.
* \returns void
*/
int KaamCollisionReaction(KaamObject* pObject, enum DIRECTION directionBeforeCollision, int allowRebound)
{
	int reaction = 0;
	if (directionBeforeCollision == UPLEFT && pObject->motionDirection == DLEFT
		|| directionBeforeCollision == UPRIGHT && pObject->motionDirection == DRIGHT)
	{
		resetReboundVariables(pObject, 1.0, -1.0, 1.0, 2.0);
		reaction = 1;
	}
	else if (directionBeforeCollision == UPLEFT && pObject->motionDirection == DRIGHT
		|| directionBeforeCollision == UPRIGHT && pObject->motionDirection == DLEFT)
	{
		resetReboundVariables(pObject, -1.0, -1.0, 1.0, 2.0);
		reaction = 1;
	}
	else if (pObject->motionDirection == UPRIGHT || pObject->motionDirection == UPLEFT)
	{
		resetReboundVariables(pObject, 1.0, -1.0, 1.0, 2.0);
		reaction = 1;
	}
	else if (allowRebound && pObject->rebound < 1 &&
		(pObject->motionDirection == DRIGHT
		|| pObject->motionDirection == DLEFT))
	{
		resetReboundVariables(pObject, 1.0, 1.0, 2.0, 2.0);
		reaction = 1;
		pObject->rebound += 1;
	}
	else if (directionBeforeCollision == UP)
	{
		resetReboundVariables(pObject, 1.0, -1.0, 1.0, 2.0);
		reaction = 1;
	}
	else if ((directionBeforeCollision == UPLEFT || directionBeforeCollision == UPRIGHT) && pObject->motionDirection == UP)
	{
		resetReboundVariables(pObject, 0.0, -1.0, 1.0, 1.0);
		reaction = 1;
	}
	return reaction;
}