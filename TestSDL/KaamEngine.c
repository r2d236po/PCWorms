#include "KaamEngine.h"
#include "AffichageGeneral.h"
#include "input.h"
#include "display.h"
#include "HUD.h"


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
* \fn int KaamInitGame(Worms** wormsTab, SDL_Surface* pSurfaceMap)
* \brief Init worms positions and put them to the ground.
*
* \param[in] wormsTab, array of worms.
* \param[in] pSurfaceMap, pointer to the map's surface.
* \returns void
* \remarks A refaire pour chaque equipe
*/
int KaamInitGame(Worms** wormsTab, SDL_Surface* pSurfaceMap)
{
	int indexWorms = 0, initEnd = 0;
	srand((int)time(NULL));
	static int initStart = 0;
	if (initStart == 0)
	{
		for (indexWorms = 0; indexWorms < globalVar.nbWormsTotal; indexWorms++)
		{
			wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.x = rand_a_b(rand_a_b(0, pSurfaceMap->w), (pSurfaceMap->w - wormsTab[indexWorms]->wormsObject->objectSurface->w - 1));
			wormsTab[indexWorms]->wormsObject->objectBox.x = wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.x;
			wormsTab[indexWorms]->wormsObject->objectBox.y = wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.y = 0;
			resetAbsoluteCoordinates(wormsTab[indexWorms]->wormsObject->objectSurface, &wormsTab[indexWorms]->wormsObject->absoluteCoordinate.x, &wormsTab[indexWorms]->wormsObject->absoluteCoordinate.y);
			initStart = 1;
		}
	}
	else
	{
		updateTextSurfaceWormsTab(wormsTab);	//MAJ de la position du texte + Surface Vie	
		initEnd = testGround(pSurfaceMap, wormsTab[0]->wormsObject->objectSurface, 1) || deathByLimitMap(wormsTab[0], pSurfaceMap);
		KaamGravityManagement(pSurfaceMap, wormsTab[0]->wormsObject);
		displayWorms(wormsTab[0], 1);
		for (indexWorms = 1; indexWorms < globalVar.nbWormsTotal; indexWorms++)
		{
			initEnd &= (testGround(pSurfaceMap, wormsTab[indexWorms]->wormsObject->objectSurface, 1) || deathByLimitMap(wormsTab[indexWorms], pSurfaceMap));
			KaamGravityManagement(pSurfaceMap, wormsTab[indexWorms]->wormsObject);
			displayWorms(wormsTab[indexWorms], 1);
		}
	}
	for (indexWorms = 0; indexWorms < globalVar.nbWormsTotal; indexWorms++)
	{
		wormsTab[indexWorms]->wormsObject->objectBox.x = wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.x;
		wormsTab[indexWorms]->wormsObject->objectBox.y = wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.y;
	}
	if (initEnd)
		fprintf(logFile, "KaamInitGame : DONE.\n\n");
	return initEnd;
}

/**
* \fn KaamObject* KaamInitObject(SDL_Surface* pSurface, float initSpeedX, float initSpeedY, enum DIRECTION initDirection, int weapon)
* \brief Initialiazes a KaamObject structure.
*
* \param[in] pSurface, surface to initialize the object with.
* \param[in] initSpeedX, speed along X axis to initialises the object with.
* \param[in] initSpeedY, speed along Y axis to initialises the object with.
* \param[in] initDirection, direction of the object.
* \param[in] weapon, indicate whether of not if the object is a weapon.
* \returns pointer to the KaamObject created, NULL if error was encountered.
* \remarks The surface is just initiliased, there is no pixels in it. It should be done after calling this function.
*		   Use KaamInitSurfaceObject.
*/
KaamObject* KaamInitObject(SDL_Surface* pSurface, float initSpeedX, float initSpeedY, enum DIRECTION initDirection, int weapon)
{
	KaamObject* objectTemp = (KaamObject*)malloc(sizeof(KaamObject));
	if (objectTemp == NULL)
	{
		fprintf(logFile, "KaamInitObject : FAILURE, unable to allocate memory.\n\n");
		return NULL;
	}

	/*Init object surface*/
	objectTemp->objectSurface = SDL_CreateRGBSurface(0, pSurface->w, pSurface->h, 32, RMASK, GMASK, BMASK, AMASK);
	if (objectTemp->objectSurface == NULL)
	{
		fprintf(logFile, "KaamInitObject : FAILURE, createRGBSurface : %s.\n\n", SDL_GetError());
		KaamDestroyObject(&objectTemp);
		return NULL;
	}
	memcpy(objectTemp->objectSurface->pixels, pSurface->pixels, pSurface->w*pSurface->h*sizeof(Uint32));
	objectTemp->objectSurface->clip_rect.x = pSurface->clip_rect.x;
	objectTemp->objectSurface->clip_rect.y = pSurface->clip_rect.y;

	/*Init absolute coordinate*/
	resetAbsoluteCoordinates(objectTemp->objectSurface, &objectTemp->absoluteCoordinate.x, &objectTemp->absoluteCoordinate.y);

	/*Init precedent coordinate*/
	resetAbsoluteCoordinates(objectTemp->objectSurface, &objectTemp->precedentCoordinate.x, &objectTemp->precedentCoordinate.y);

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










//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Physic Management                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn void KaamPhysicManagement(KaamObject* pObject, SDL_Surface* pSurfaceMap)
* \brief  Manages physics of the worms (ground deplacement + gravity + jump).
*
* \param[in] pWorms, pointer to the worms to move.
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \returns void
*/
void KaamPhysicManagement(KaamObject* pObject, SDL_Surface* pSurfaceMap)
{
	if (globalInput->jumpOnGoing)
	{
		KaamNonLinearMotion(pSurfaceMap, pObject, 0);
	}
	else if (pObject->reactToBomb == 1)
	{
		KaamNonLinearMotion(pSurfaceMap, pObject, 1);
	}
	else if (pObject->weapon == 1)
	{
		if (!pObject->falling)
			KaamNonLinearMotion(pSurfaceMap, pObject, 1);
		if (!pObject->startMotion && !pObject->rebound)
			KaamGravityManagement(pSurfaceMap, pObject);
	}
	else// /!\ deplacement OK, gravité OK, saut NON OK
	{
		KaamGroundMotion(pObject, pSurfaceMap);
		KaamGravityManagement(pSurfaceMap, pObject);
	}
	if (pObject->weapon != 1)
		globalInput->deplacement = (MY_ABS(dxBoxProcess(pObject)) > 0 || MY_ABS(dyBoxProcess(pObject)) > 0);
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
		if (dyBoxProcess(pObject) > 0)
		{
			while (!globalMotionPossibility(pObject, pSurfaceMap, downDirection))
			{
				pObject->objectSurface->clip_rect.y -= 1;
				pObject->relativeTime = 0;
				pObject->falling = 0;
				setSideMotionPossibility(pObject, pSurfaceMap);
				resetAbsoluteCoordinates(pObject->objectSurface, &pObject->absoluteCoordinate.x, &pObject->absoluteCoordinate.y);
			}
		}
	}
	else
	{
		resetAbsoluteCoordinates(pObject->objectSurface, &pObject->absoluteCoordinate.x, &pObject->absoluteCoordinate.y);
		pObject->relativeTime = 0;
		pObject->falling = 0;
		resetAbsoluteCoordinates(pObject->objectSurface, &pObject->precedentCoordinate.x, &pObject->precedentCoordinate.y);
	}
}

/**
* \fn void KaamWormsMotionManagement(Worms* pWorms, SDL_Surface* pSurfaceMap)
* \brief Manages the motion relative to a worms.
*.
* \param[in] pWorms, pointer to the worms to move.
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \returns void
*/
void KaamWormsMotionManagement(Worms* pWorms, SDL_Surface* pSurfaceMap)
{
	int launchAnim = globalInput->direction != NONE && !globalInput->jumpOnGoing && (pWorms->wormsObject->falling == 0) && globalInput->direction != DOWN;
	int swap = 0;
	if (launchAnim)
	{
		if (!globalInput->jumpOnGoing)
			swap = swapManagement(pWorms, pSurfaceMap);
		gestionAnimationWorms(pWorms, swap, pSurfaceMap, 0);
	}
	if (!swap)
		KaamPhysicManagement(pWorms->wormsObject, pSurfaceMap);
	int motion = pWorms->wormsObject->startMotion || pWorms->wormsObject->falling;
	if (motion == 1 && pWorms->wormsObject->startMotion == 0 && pWorms->wormsObject->falling == 0)
	{
		wormsFallDamages(pWorms);
	}
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Motion Management	                 /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn void KaamGroundMotionReset(KaamObject* pObject)
* \brief Reset all variables relatives to a ground movement.
*
* \param[in] pObject, pointer to the worms to swap
*/
void KaamGroundMotionReset(KaamObject* pObject)
{
	resetInputs();
	resetAbsoluteCoordinates(pObject->objectSurface, &pObject->absoluteCoordinate.x, &pObject->absoluteCoordinate.y);
}

/**
* \fn void KaamGroundCollisionProcess(KaamObject* pObject, SDL_Surface* pSurfaceMap, int deplacement)
* \brief Manage collision on the ground.
*
* \param[in] pObject, pointer to the worms to swap
* \param[in] pSurfaceMap, pointer to the map surface
* \param[in] deplacement, value of the movement
* \returns void
*/
void KaamGroundCollisionProcess(KaamObject* pObject, SDL_Surface* pSurfaceMap, int deplacement)
{
	enum DIRECTION direction = globalInput->direction;
	int indexBoucle = 0;
	pObject->objectSurface->clip_rect.x += deplacement;
	pObject->motionDirection = motionDirectionProcess(dxBoxProcess(pObject), dyBoxProcess(pObject));
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
* \fn void KaamGroundMotion(KaamObject* pObject, SDL_Surface* pSurfaceMap)
* \brief Move the worms on the ground along the X axis.
*
* \param[in] pObject, pointer to the worms to swap
* \param[in] pSurfaceMap, pointer to the map surface
* \returns void
*/
void KaamGroundMotion(KaamObject* pObject, SDL_Surface* pSurfaceMap)
{
	int authorizeMovement = (globalInput->direction != NONE && globalInput->direction != UP && !globalInput->jumpOnGoing);

	if (authorizeMovement && pObject->falling == 0)
	{
		if (!sideInitialized(pObject))
			setSideMotionPossibility(pObject, pSurfaceMap);
		switch (globalInput->direction)
		{
		case RIGHT:
			if (pObject->rightOk)
				pObject->objectSurface->clip_rect.x += GROUNDSPEED;
			if (pObject->rightOk > 1)
				pObject->objectSurface->clip_rect.y -= 1;
			break;
		case LEFT:
			if (pObject->leftOk)
				pObject->objectSurface->clip_rect.x -= GROUNDSPEED;
			if (pObject->leftOk > 1)
				pObject->objectSurface->clip_rect.y -= 1;
			break;
		}
		KaamGroundMotionReset(pObject);
		setSideMotionPossibility(pObject, pSurfaceMap);
		pObject->motionDirection = motionDirectionProcess(dxBoxProcess(pObject), 0);
	}
}

/**
* \fn void KaamNonLinearMotion(SDL_Surface* pSurfaceMap, KaamObject* pObject, int allowRebound)
* \brief Manages the jump of a worms.
*
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pObject, pointer to the object to move.
* \param[in] allowRebound, selects if rebound are allowed in the motion.
* \returns void
*/
void KaamNonLinearMotion(SDL_Surface* pSurfaceMap, KaamObject* pObject, int allowRebound)
{
	int stopReact = allowRebound && pObject->relativeTime >= 42 && (pObject->Yspeed == 0);
	enum DIRECTION directionBeforeCollision;
	if (!pObject->startMotion)
	{
		resetAbsoluteCoordinates(pObject->objectSurface, &pObject->precedentCoordinate.x, &pObject->precedentCoordinate.y);
		if ((pObject->startMotion = getStartMotion(pObject, pSurfaceMap)) == 0)
			pObject->reactToBomb = 0;
		if (pObject->startMotion)
			resetAbsoluteCoordinates(pObject->objectSurface, &pObject->precedentCoordinate.x, &pObject->precedentCoordinate.y);
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
		pObject->motionDirection = motionDirectionProcess(dxBoxProcess(pObject), dyBoxProcess(pObject));
		directionBeforeCollision = pObject->motionDirection;
		pObject->relativeTime += 7;
		KaamCollisionManagement(pSurfaceMap, pObject, directionBeforeCollision, allowRebound);
		if (stopReact)
			resetNonLinearMotion(pObject, pSurfaceMap);
	}
	else resetMotionVariables(pObject);
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Collision Management                   /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int globalMotionPossibility(KaamObject* pObject, SDL_Surface* pSurfaceMap, enum DIRECTION direction)
{
	int possible = 1;
	enum DIRECTION testDirection = direction;
	if (collisionSurfaceWithMap(pSurfaceMap, pObject->objectSurface, &testDirection, 1))
		possible = 0;
	if (collisionSurfaceWithMapLimits(pSurfaceMap, pObject->objectSurface))
		possible = 0;
	return possible;
}

/**
* \fn void KaamCollisionManagement(SDL_Surface* pSurfaceMap, KaamObject* pObject, enum DIRECTION directionBeforeCollision, int allowRebound)
* \brief replace le worms en cas de collision.
*
* \param[in] pSurfaceMap, pointer to the surface's map.
* \param[in] pObject, pointer to the object in motion.
* \param[in] directionBeforeCollision, direction of the motion before contact.
* \param[in] allowRebound, selects if rebound are allowed in the motion.
* \returns void.
*/
void KaamCollisionManagement(SDL_Surface* pSurfaceMap, KaamObject* pObject, enum DIRECTION directionBeforeCollision, int allowRebound)
{
	while (!globalMotionPossibility(pObject, pSurfaceMap, pObject->motionDirection))
	{
		if (dxBoxProcess(pObject) > 0)
			pObject->objectSurface->clip_rect.x -= 1;
		if (dxBoxProcess(pObject) < 0)
			pObject->objectSurface->clip_rect.x += 1;
		if (dyBoxProcess(pObject) > 0)
			pObject->objectSurface->clip_rect.y -= 1;
		if (dyBoxProcess(pObject) < 0)
			pObject->objectSurface->clip_rect.y += 1;
		if (dxBoxProcess(pObject) == 0 && dyBoxProcess(pObject) == 0)
		{
			if (directionBeforeCollision == UPLEFT || directionBeforeCollision == UPRIGHT)
			{
				pObject->objectSurface->clip_rect.y -= 1;
				if (!collisionSurfaceWithMapBasic(pSurfaceMap, pObject->objectSurface))
					pObject->motionDirection = DOWN;
				pObject->objectSurface->clip_rect.y += 1;
			}
			if (!KaamCollisionReaction(pObject, directionBeforeCollision, allowRebound))
				resetNonLinearMotion(pObject, pSurfaceMap);
			break;
		}
	}
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
	else if (allowRebound && pObject->rebound < 2)
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