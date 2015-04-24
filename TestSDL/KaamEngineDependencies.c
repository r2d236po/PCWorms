#include "KaamEngineDependencies.h"


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                    Set functions                       /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn void setSurfaceRelativeCoordinates(SDL_Surface* pSurface, int relativeTime, float Xspeed, float Yspeed)
* \brief Process relatives coordinates for gravity and jump functions.
*
* \param[in] pSurface, pointer to the surface to move.
* \param[in] relativeTime, relative time to use to generate coordinates.
* \param[in] Xspeed, speed along the X axis.
* \param[in] Yspeed, speed along the Y axis.
* \returns void
*/
void setSurfaceRelativeCoordinates(SDL_Surface* pSurface, int relativeTime, float Xspeed, float Yspeed)
{
	float relativeX = 0.0, relativeY = 0.0;
	const double g = 9.81;

	relativeX = (float)(Xspeed * relativeTime);
	relativeY = (float)(Yspeed * relativeTime - (float)(g*CARRE(relativeTime) / 2000));

	pSurface->clip_rect.x += (int)relativeX;
	pSurface->clip_rect.y -= (int)relativeY;
}

/**
* \fn void setSurfaceAbsoluteCoordinates(SDL_Surface* pSurface, int absoluteX, int absoluteY)
* \brief Put a surface to the absolute coordinates wanted.
*
* \param[in] pSurface, pointer to the surface to move.
* \param[in] absoluteX, absolute coordinate along the X axis.
* \param[in] absoluteY, absolute coordinate along the Y axis.
* \returns void
*/
void setSurfaceAbsoluteCoordinates(SDL_Surface* pSurface, int absoluteX, int absoluteY)
{
	pSurface->clip_rect.x = absoluteX;
	pSurface->clip_rect.y = absoluteY;
}

/**
* \fn void setSpeed(float* Xspeed, float* Yspeed, float valueX, float valueY)
* \brief Set speed vectors along Y and X axis.
*
* \param[in] Xspeed, pointer to the variable filled with the speed along the X axis to be set.
* \param[in] Yspeed, pointer to the variable filled with the speed along the Y axis to be set.
* \param[in] valueX, value of the speed along X axis to set.
* \param[in] Yspeed, value of the speed along Y axis to set.
* \returns void
*/
void setSpeed(float* Xspeed, float* Yspeed, float valueX, float valueY)
{
	*Xspeed = valueX;
	*Yspeed = valueY;
}

/**
* \fn void setSideMotionPossibility(KaamObject* pObject, SDL_Surface* pSurfaceMap)
* \brief Set the side variable that indicate a faisability of a ground move.
*
* \param[in] pObject, pointer to the object to test.
* \param[in] pSurfaceMap, pointer to the map's surface.
* \returns void
*/
void setSideMotionPossibility(KaamObject* pObject, SDL_Surface* pSurfaceMap)
{
	pObject->rightOk = 1;
	pObject->leftOk = 1;
	pObject->objectSurface->clip_rect.x += Xtest;
	enum DIRECTION directionTest = pObject->motionDirection;
	if (collisionSurfaceWithMapLimits(pSurfaceMap, pObject->objectSurface)
		|| collisionSurfaceWithMap(pSurfaceMap, pObject->objectSurface, &directionTest, 1))
	{
		if (directionTest == DOWN)
		{
			directionTest = UP;
			pObject->objectSurface->clip_rect.y -= Ytest;
			if (collisionSurfaceWithMap(pSurfaceMap, pObject->objectSurface, &directionTest, 1) && directionTest != DOWN)
				pObject->rightOk = 0;
			else pObject->rightOk = 2;
			pObject->objectSurface->clip_rect.y += Ytest;
		}
		else pObject->rightOk = 0;
	}
	pObject->objectSurface->clip_rect.x -= 2*Xtest;
	if (collisionSurfaceWithMapLimits(pSurfaceMap, pObject->objectSurface)
		|| collisionSurfaceWithMap(pSurfaceMap, pObject->objectSurface, &directionTest, 1))
	{
		if (directionTest == DOWN)
		{
			directionTest = UP;
			pObject->objectSurface->clip_rect.y -= Ytest;
			if (collisionSurfaceWithMap(pSurfaceMap, pObject->objectSurface, &directionTest, 1) && directionTest != DOWN)
				pObject->leftOk = 0;
			else pObject->leftOk = 2;
			pObject->objectSurface->clip_rect.y += Ytest;
		}
		else pObject->leftOk = 0;
	}
	pObject->objectSurface->clip_rect.x += Xtest;
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Reset functions                      /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn void resetInputs(Input* pInput)
* \brief Reset the inputs relatives to jump and deplacement
*
* \param[in] pInput, pointer to the structure of inputs.
* \returns void
*/
void resetInputs(Input* pInput)
{
	pInput->jump = 0;
	pInput->jumpOnGoing = 0;
	pInput->direction = NONE;
}

/**
* \fn void resetSpeed(float* Xspeed, float* Yspeed)
* \brief Reset speed vectors along Y and X axis.
*
* \param[in] Xspeed, pointer to the variable filled with the speed along the X axis to be reset.
* \param[in] Yspeed, pointer to the variable filled with the speed along the Y axis to be reset.
* \returns void
*/
void resetSpeed(float* Xspeed, float* Yspeed)
{
	*Xspeed = 0.0;
	*Yspeed = 0.0;
}

/**
* \fn void resetMotionVariables(Input* pInput, KaamObject* pObject, int* relativeTime, int* startMotion)
* \brief Resets all variable relative to a jump.
*
* \param[in] pInput, pointer to the structure of inputs.
* \param[in] pObject, pointer to the object to move.
* \returns void
*/
void resetMotionVariables(Input* pInput, KaamObject* pObject)
{
	resetInputs(pInput);
	resetSpeed(&pObject->Xspeed, &pObject->Yspeed);
	resetAbsoluteCoordinates(pObject->objectSurface, &pObject->absoluteCoordinate.x, &pObject->absoluteCoordinate.y);
	pObject->relativeTime = 0;
	pObject->startMotion = 0;
	pObject->motionDirection = DOWN;
}

/**
* \fn void resetReboundVariables(KaamObject* pObject, int* relativeTime, float signX, float signY, float coeffX, float coeffY)
* \brief Resets all variable relative to a rebound.
*
* \param[in] pObject, pointer to the object to move.
* \param[in] signX, new sign to apply on X speed.
* \param[in] signY, new sign to apply on Y speed.
* \param[in] coeffX, value wich will divide the X speed.
* \param[in] coeffY, value wich will divide the Y speed.
* \returns void
*/
void resetReboundVariables(KaamObject* pObject, float signX, float signY, float coeffX, float coeffY)
{
	pObject->Yspeed = signY * pObject->Yspeed / coeffY;
	pObject->Xspeed = signX * pObject->Xspeed / coeffX;
	resetAbsoluteCoordinates(pObject->objectSurface, &pObject->absoluteCoordinate.x, &pObject->absoluteCoordinate.y);
	pObject->relativeTime = 0;
}

/**
* \fn void resetAbsoluteCoordinates(SDL_Surface* pSurface, int* absoluteX, int* absoluteY)
* \brief Get the absolute coordinate of a surface into variable.
*
* \param[in] pSurface, pointer to the surface to move.
* \param[in] absoluteX, pointer to the variable to fill in with the absolute coordinate along the X axis.
* \param[in] absoluteY, pointer to the variable to fill in with the absolute coordinate along the Y axis.
* \returns void
*/
void resetAbsoluteCoordinates(SDL_Surface* pSurface, int* absoluteX, int* absoluteY)
{
	*absoluteX = pSurface->clip_rect.x;
	*absoluteY = pSurface->clip_rect.y;
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                    Test functions                      /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////


/**
* \fn int getStartMotion(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap)
* \brief Manages the start of a jump of a worms.
*
* \param[in] pInput, pointer to the structure of inputs.
* \param[in] pWorms, pointer to the worms to move.
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \returns jumpOk, 1 = the jump can start, 0 = the jump cannot start
*/
int getStartMotion(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap)
{
	int	motionOk = 1;
	if (pInput->jump)
	{
		motionOk = jumpDoability(pSurfaceMap, pObject->objectSurface, pObject->motionDirection);
	}
	else motionOk = jumpDoability(pSurfaceMap, pObject->objectSurface, UP);

	return motionOk;
}

/**
* \fn int jumpDoability(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION jumpDirection)
* \brief Determine the faisability of a jump.
*
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pSurfaceMotion, pointer to the surface in motion.
* \param[in] jumpDirection, direction of the jump (UP or UPLEFT or UPRIGHT).
* \returns 1 = jump doable, 0 = jump not doable
*/
int jumpDoability(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION jumpDirection)
{
	int checkUpLeft = 1, checkUpRight = 1, checkUp = 1, checkRight = 1, checkLeft = 1;
	int doAble = 1;

	checkUp = checkDirection(pSurfaceMap, pSurfaceMotion, 0, -7, UP);
	if (jumpDirection == UP || jumpDirection == UPLEFT)
	{
		checkUpLeft = checkDirection(pSurfaceMap, pSurfaceMotion, -7, -7, UPLEFT);
		checkLeft = checkDirection(pSurfaceMap, pSurfaceMotion, -7, 0, LEFT);
	}
	if (jumpDirection == UP || jumpDirection == UPRIGHT)
	{
		checkUpRight = checkDirection(pSurfaceMap, pSurfaceMotion, 7, -7, UPRIGHT);
		checkRight = checkDirection(pSurfaceMap, pSurfaceMotion, 7, 0, RIGHT);
	}
	doAble = processCheck(checkLeft, checkRight, checkUp, checkUpLeft, checkUpRight, jumpDirection);
	return doAble;
}

/**
* \fn int checkDirection(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, int offsetX, int offsetY, enum DIRECTION checkDirection)
* \brief Checks a collision within the offsets range provided and depending of the direction given.
*
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pSurfaceMotion, pointer to the surface in motion.
* \param[in] offsetX, range of the detection along the X axis.
* \param[in] offsetY, range of the detection along the Y axis.
* \param[in] checkDirection, direction to check.
* \returns  1 = clear, 0 = obstacle within the range
*/
int checkDirection(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, int offsetX, int offsetY, enum DIRECTION checkDirection)
{
	int check = 1, y = pSurfaceMotion->clip_rect.y, x = pSurfaceMotion->clip_rect.x;
	enum DIRECTION directionTest = checkDirection;

	pSurfaceMotion->clip_rect.x += offsetX;
	pSurfaceMotion->clip_rect.y += offsetY;
	if (collisionSurfaceWithMap(pSurfaceMap, pSurfaceMotion, &directionTest, 1))
	{
		if (directionTest != DOWN)
			check = 0;
		if (directionTest == DOWN && checkDirection == DOWN)
			check = 0;
	}
	pSurfaceMotion->clip_rect.x = x;
	pSurfaceMotion->clip_rect.y = y;
	return check;
}

/**
* \fn int processCheck(int checkLeft, int checkRight, int checkUp, int checkUpLeft, int checkUpRight, enum DIRECTION jumpDirection)
* \brief Process the doability of a jump depending of the different sides check and direction of the jump.
*
* \param[in] checkLeft, indicates if left side is good to go.
* \param[in] checkRight, indicates if right side is good to go.
* \param[in] checkUp, indicates if the top is good to go.
* \param[in] checkUpLeft, indicates if upper corner left is good to go.
* \param[in] checkUpRight, indicates if upper corner right is good to go.
* \param[in] jumpDirection, direction of the jump (UP or UPLEFT or UPRIGHT).
* \returns 1 = the direction is OK with obstacles around the object, 0 = obstacle int the way of the jump
*/
int processCheck(int checkLeft, int checkRight, int checkUp, int checkUpLeft, int checkUpRight, enum DIRECTION jumpDirection)
{
	int generalCheck = 1;

	if (checkUp)
	{
		switch (jumpDirection)
		{
		case UP:
			if ((!checkRight && !checkUpRight) || (!checkLeft && !checkUpLeft))
				generalCheck = 0;
			break;
		case UPLEFT:
			if (!checkLeft || !checkUpLeft)
				generalCheck = 0;
			break;
		case UPRIGHT:
			if (!checkRight || !checkUpRight)
				generalCheck = 0;
			break;
		default:
			break;
		}
	}
	else generalCheck = 0;
	return generalCheck;
}

/**
* \fn int testGround(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, int testValue)
* \brief Checks if a surface is on the ground.
*
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pSurfaceMotion, pointer to the surface in motion.
* \param[in] testValue, value of the y offset to test ground
* \returns  1 = onGround, 0 = not on the ground
*/
int testGround(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, int testValue)
{
	return !checkDirection(pSurfaceMap, pSurfaceMotion, 0, testValue, DOWN);
}

/**
* \fnint sideInitialized(KaamObject* pObject)
* \brief Test if the side variables have been initialized.
*
* \param[in] pObject, pointer to the object to test.
* \returns 1 = variables are initialized, 0 = variables aren't initialized
*/
int sideInitialized(KaamObject* pObject)
{
	return (pObject->leftOk >= 0 && pObject->rightOk >= 0);
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Collision functions                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn int collisionSurfaceWithMapLimits(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
* \brief Test if the object in motion is out of the map.
*
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pSurfaceMotion, pointer to the surface in motion.
* \returns  1 = collision, 0 = no collision
*/
int collisionSurfaceWithMapLimits(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
{
	int xMax = pSurfaceMap->w, yMax = pSurfaceMap->h;
	int x = pSurfaceMotion->clip_rect.x, y = pSurfaceMotion->clip_rect.y;
	int w = pSurfaceMotion->w, h = pSurfaceMotion->h;

	if (x < 0
		|| y < 0
		|| x + w > xMax
		|| y + h > yMax)
	{
		reajustSurfaceWithMapLimits(pSurfaceMap, pSurfaceMotion);
		return 1;
	}
	return 0;
}

/**
* \fn int collisionSurfaceWithMap(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection)
* \brief Detecte s'il y a collision entre deux surfaces selon le sens de déplacement du worms.
*
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pSurfaceMotion, pointer to the surface in motion.
* \param[in] pDirection, pointer to the object direction.
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int collisionSurfaceWithMap(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection, int checkMode)
{
	//Variables d'acquisitions
	Uint32 pixelS1 = 0;	//Variable stockant le pixel en cours de lecture de la surface de la map
	Uint8 rS1 = 0, gS1 = 0, bS1 = 0, aS1 = 0;	//Variables stockant les informations sur les couleurs du pixel lu de la surface de la map
	Uint32 pixelS2 = 0;	//Variable stockant le pixel en cours de lecture de la surface en mouvement
	Uint8 rS2 = 0, gS2 = 0, bS2 = 0, aS2 = 0;	//Variables stockant les informations sur les couleurs du pixel lu de la surface en mouvement
	int offset_xS2 = pSurfaceMotion->clip_rect.x;	//Offset en x de la surface en mouvement dans la map
	int offset_yS2 = pSurfaceMotion->clip_rect.y;	//Offset en y de la surface en mouvement dans la map
	SDL_PixelFormat* formatS1 = pSurfaceMap->format;	//Pointeur du format de pixels de la surface de la map
	SDL_PixelFormat* formatS2 = pSurfaceMotion->format;	//Pointeur du format de pixels de la surface en mouvement
	//Variables de balayage
	int x = 0, y = 0;	//Variables de balayage des x, y
	int xStart = pSurfaceMotion->clip_rect.x, xEnd = pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w, xInc = 1;	//Variables de début, de fin et d'incrément du balayage des x
	int yStart = pSurfaceMotion->clip_rect.y, yEnd = pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h, yInc = 1;	//Variables de début, de fin et d'incrément du balayage des y
	int zone[4] = { 0, 0, 0, 0 }, balayageGen = 0;
	//Variable de collision
	int collision = 0;	//Booleen de collision, 0 = pas de collision, 1 = collision

	/*Test des limites de la map et de la fenetre*/
	if (collisionSurfaceWithMapLimits(pSurfaceMap, pSurfaceMotion))	//Detection d'un dépassement de la map
		return 1;

	/*Détermination de l'ordre des zones à balayer*/
	calculOrdreBalayage(*pDirection, zone);

	for (balayageGen = 0; (balayageGen < 4) && (collision == 0); balayageGen += 1)
	{
		/*Détermination de yStart, yEnd, xStart et xEnd*/
		calculXYBalayage(pSurfaceMotion, &xStart, &xEnd, &yStart, &yEnd, zone[balayageGen]);	//Calcul des valeurs de balayage des boucles for pour optimiser la vitesse de traitement

		/*Calcul de la collision*/
		for (y = yStart; (y < yEnd) && (collision == 0); y += yInc)
		{
			for (x = xStart; (x < xEnd) && (collision == 0); x += xInc)
			{
				/*Acquisition des pixels des surfaces 1 et 2*/
				pixelS1 = ReadPixel(pSurfaceMap, MY_ABS(x), MY_ABS(y));	//Lecture du pixel de la map
				pixelS2 = ReadPixel(pSurfaceMotion, MY_ABS(x) - offset_xS2, MY_ABS(y) - offset_yS2);	//Lecture du pixel de la surface en mouvement

				/*Récupération des composantes colorimétriques*/
				SDL_GetRGBA(pixelS1, formatS1, &rS1, &gS1, &bS1, &aS1);	//Informations sur les couleurs du pixel de la surface de la map
				SDL_GetRGBA(pixelS2, formatS2, &rS2, &gS2, &bS2, &aS2);	//Informations sur les couleurs du pixel de la surface en mouvement

				/*Détermination de la collision*/
				if (aS1 != 255 || aS2 != 255)	//Si le pixel de la surface de la map ou le pixel de la surface en mouvement est transparent
					collision = 0;	//Collision = 0 -> pas de collision
				else	//Au moins l'un des pixels n'est pas transparent
				{
					collision = 1;	//Collision = 1 -> collision
					*pDirection = calculDirectionCollision(*pDirection, zone[balayageGen], checkMode);	//Calcul de la direction de la collision pour affiner le traitement
				}
			}
		}
	}
	formatS1 = NULL;	//Remise à 0 des pointeurs de format
	formatS2 = NULL;	//Remise à 0 des pointeurs de format
	return collision;
}

/**
* \fn int collisionWithMap(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
* \brief Test if the object in motion is in collision with the map.
*		 It is a perfect pixel collision test.
* \param[in] pSurfaceMap, pointer to the surface of the map.
* \param[in] pSurfaceMotion, pointer to the surface in motion.
* \returns  1 = collision, 0 = no collision
* \remarks This function does not handle the case of the motion surface not being within the map surface limits.
*          To avoid any problem, please use collisionWithMapLimits before.
*/
int collisionSurfaceWithMapBasic(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
{
	Uint32 indexSurfaceMap = 0, index;
	int x1, y1, w, h, collision = 0, x, y;
	Uint32* pixelSurfaceMap = (Uint32*)pSurfaceMap->pixels;
	Uint32* pixelSurfaceMotion = (Uint32*)pSurfaceMotion->pixels;
	SDL_PixelFormat* formatS1 = pSurfaceMap->format;	//Pointeur du format de pixels de la surface de la map
	SDL_PixelFormat* formatS2 = pSurfaceMotion->format;	//Pointeur du format de pixels de la surface en mouvement

	if (collisionSurfaceWithMapLimits(pSurfaceMap, pSurfaceMotion))
		return 1;

	x1 = pSurfaceMotion->clip_rect.x;
	y1 = pSurfaceMotion->clip_rect.y;
	h = y1 + pSurfaceMotion->h;
	w = x1 + pSurfaceMotion->w;
	for (y = y1; (y < h) && (!collision); y++)
	{
		for (x = x1; (x < w) && (!collision); x++)
		{
			index = (x - x1) + (y - y1) * pSurfaceMotion->w;
			indexSurfaceMap = x + y * pSurfaceMap->w;
			if (pixelTransparent(pixelSurfaceMotion[index], formatS2) || pixelTransparent(pixelSurfaceMap[indexSurfaceMap], formatS1))
				collision = 0;
			else collision = 1;
		}
	}

	formatS1 = NULL;	//Remise à 0 des pointeurs de format
	formatS2 = NULL;	//Remise à 0 des pointeurs de format
	return collision;
}

/**
* \fn void calculXYBalayage(SDL_Surface* pSurfaceMotion, int* xStart, int* xEnd, int* yStart, int* yEnd, int zone)
* \brief Calcul les valeurs de debut et de fin pour X et Y afin d'optimiser la detection de collision.
*
* \param[in] pSurfaceMotion, surface en deplacement
* \param[in] xStart, pointeur vers la variable xStart, debut du balayage en x
* \param[in] xEnd, pointeur vers la variable xEnd, fin du balayage en x
* \param[in] yStart, pointeur vers la variable yStart, debut du balayage en y
* \param[in] yEnd, pointeur vers la variable yEnd, fin du balayage en y
* \param[in] zone, zone de recherche de la collision
* \returns void
*/
void calculXYBalayage(SDL_Surface* pSurfaceMotion, int* xStart, int* xEnd, int* yStart, int* yEnd, int zone)
{
	int x = pSurfaceMotion->clip_rect.x, w = pSurfaceMotion->w;
	int y = pSurfaceMotion->clip_rect.y, h = pSurfaceMotion->h;
	switch (zone)
	{
	case 1:
		*xStart = x;
		*xEnd = x + (w / 2);
		*yStart = y;
		*yEnd = y + (h / 2);
		break;
	case 2:
		*xStart = -(x + w - 1);
		*xEnd = -(x + (w / 2));
		*yStart = y;
		*yEnd = y + (h / 2);
		break;
	case 3:
		*xStart = x;
		*xEnd = x + (w / 2);
		*yStart = -(y + h - 1);
		*yEnd = -(y + (h / 2));
		break;
	case 4:
		*xStart = -(x + w - 1);
		*xEnd = -(x + (w / 2));
		*yStart = -(y + h - 1);
		*yEnd = -(y + (h / 2));
		break;
	case 5:
		*xStart = x;
		*xEnd = x + (w / 2);
		*yStart = y + (2 * h / 8);
		*yEnd = y + (6 * h / 8);
		break;
	case 6:
		*xStart = -(x + w - 1);
		*xEnd = -(x + (w / 2));
		*yStart = y + (2 * h / 8);
		*yEnd = y + (6 * h / 8);
		break;
	case 7:
		*xStart = x;
		*xEnd = x + w - 1;
		*yStart = y;
		*yEnd = y + (2 * h / 8);
		break;
	case 8:
		*xStart = -(x + w - 1);
		*xEnd = -x;
		*yStart = -(y + h - 1);
		*yEnd = -(y + (6 * h / 8));
		break;
	}
}


/**
* \fn void calculOrdreBalayage(enum DIRECTION direction, int ordre[4])
* \brief Calcul l'ordre des zones à balayer.
*
* \param[in] direction, direction du deplacement de l'objet
* \param[in] ordre, tableau contenant l'ordre des zones
* \returns void
*/
void calculOrdreBalayage(enum DIRECTION direction, int ordre[4])
{
	switch (direction)
	{
	case UP:
		ordre[0] = 7;
		ordre[1] = 5;
		ordre[2] = 6;
		ordre[3] = 8;
		break;
	case DOWN:
		ordre[0] = 8;
		ordre[1] = 5;
		ordre[2] = 6;
		ordre[3] = 7;
		break;
	case UPLEFT:
		ordre[0] = 1;
		ordre[1] = 2;
		ordre[2] = 3;
		ordre[3] = 4;
		break;
	case UPRIGHT:
		ordre[0] = 2;
		ordre[1] = 1;
		ordre[2] = 4;
		ordre[3] = 3;
		break;
	case DLEFT:
		ordre[0] = 3;
		ordre[1] = 4;
		ordre[2] = 1;
		ordre[3] = 2;
		break;
	case DRIGHT:
		ordre[0] = 4;
		ordre[1] = 3;
		ordre[2] = 2;
		ordre[3] = 1;
		break;
	case LEFT:
		ordre[0] = 5;
		ordre[1] = 8;
		ordre[2] = 7;
		ordre[3] = 6;
		break;
	case RIGHT:
		ordre[0] = 6;
		ordre[1] = 8;
		ordre[2] = 7;
		ordre[3] = 5;
		break;
	}

}








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Motion functions                     /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn int dxProcess(KaamObject* pObject)
* \brief Process the delta on X to apply on the object in motion.
*
* \param[in] pObject, pointer to the object in motion.
* \returns  value of the absolute delta along x axis
*/
int dxProcess(KaamObject* pObject)
{
	return pObject->objectSurface->clip_rect.x - pObject->objectBox.x;
}

/**
* \fn int dyProcess(KaamObject* pObject)
* \brief Process the delta on X to apply on the object in motion.
*
* \param[in] pObject, pointer to the object in motion.
* \returns  value of the absolute delta along y axis
*/
int dyProcess(KaamObject* pObject)
{
	return pObject->objectSurface->clip_rect.y - pObject->objectBox.y;
}

/**
* \fn enum DIRECTION motionDirectionProcess(int dx, int dy)
* \brief Process the direction of the jump depending on the delta along the axis.
*
* \param[in] dx, delta along X axis.
* \param[in] dy, delta along Y axis.
* \return DIRECTION, motion direction
*/
enum DIRECTION motionDirectionProcess(int dx, int dy)
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
		else if (dy < 0)
			return UPRIGHT;
	}
	else if (dy < 0)
		return UPLEFT;
	else if (dy > 0)
		return DLEFT;
	return DOWN;
}

