#include "worms.h"
#include "AffichageGeneral.h"
#include "physique.h"
#include "my_stdrFct.h"
#include "KaamEngine.h"









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////              Initialisation / Destruction              /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn Worms* createWorms(const char *file)
* \brief Créé et initialise une structure worms.
*
* \param[in] file, chaine de caractères correspondant au nom du fichier image du worms.
*
* \returns pointeur vers la structure worms créée, NULL si echec
*/
Worms* createWorms(char* name)
{
	Worms* worms = NULL;
	SDL_Surface* wormsSurfaceLeft = NULL;
	SDL_Surface* wormsSurfaceRight = NULL;
	SDL_Surface* texteSurface = NULL;
	SDL_Surface* tombeSurface = NULL;
	SDL_Rect clip = initRect(445, 28, widthSpriteMov, hightSpriteMov);
	fprintf(logFile, "createWorms : START :\n\n");
	worms = (Worms*)malloc(sizeof(Worms));
	if (worms == NULL)
	{
		fprintf(logFile, "createWorms : FAILURE, allocating memory to worms.\n\n");
		return NULL;
	}
	worms->wormsSurfaceLeft = NULL;
	worms->wormsSurfaceRight = NULL;
	worms->texteSurface = NULL;
	wormsSurfaceLeft = SDL_CreateRGBSurface(0, widthSpriteMov, hightSpriteMov, 32, RMASK, GMASK, BMASK, AMASK);
	wormsSurfaceRight = SDL_CreateRGBSurface(0, widthSpriteMov, hightSpriteMov, 32, RMASK, GMASK, BMASK, AMASK);
	//wormsSurfaceRight = loadImage("../assets/pictures/wormsd.png");
	tombeSurface = loadImage("../assets/pictures/Tombe2_SD.png");
	if (tombeSurface == NULL)
	{
		fprintf(logFile, "createWorms : FAILURE, loadImage.\n\n");
		destroyWorms(&worms, 1);
		return NULL;
	}
	if (wormsSurfaceLeft == NULL || wormsSurfaceRight == NULL)
	{
		fprintf(logFile, "createWorms : FAILURE, createRGBSurface : %s.\n\n", SDL_GetError());
		destroyWorms(&worms, 1);
		return NULL;
	}
	SDL_BlitSurface(spriteDeplacement, &clip, wormsSurfaceLeft, NULL);
	clip.x = 24;
	clip.y = 84;
	SDL_BlitSurface(spriteDeplacement, &clip, wormsSurfaceRight, NULL);

	//texteSurface = TTF_RenderText_Blended(font.FontName, worms->nom, font.couleurNameBleu);

	worms->texteSurface = texteSurface;
	worms->wormsSurfaceLeft = wormsSurfaceLeft;
	worms->wormsSurfaceRight = wormsSurfaceRight;
	worms->wormsSurfaceTomb = tombeSurface;

	//Initialisations liees a l'image du worms
	clip.x = 300;
	clip.y = 100;
	clip.w = wormsSurfaceRight->w;
	clip.h = wormsSurfaceRight->h;
	if ((worms->wormsObject = KaamInitObject(clip, 0.0, 0.0, DOWN, 0)) == NULL)
	{
		fprintf(logFile, "createWorms : FAILURE, KaamInitObject.\n\n");
		destroyWorms(&worms, 1);
		return NULL;
	}
	KaamInitSurfaceObject(worms->wormsObject, (Uint32*)wormsSurfaceRight->pixels, wormsSurfaceRight->w * wormsSurfaceRight->h);

	//initialisation des variables autres
	worms->vie = 100;
	strcpy(worms->nom, name);
	//worms->invent = initInvent(Worms* worms); A FAIRE
	worms->indexAnim = 0;
	worms->dirSurface = RIGHT;
	worms->arme = NULL;

	//remise à null des pointeurs temporaires
	wormsSurfaceLeft = NULL;
	wormsSurfaceRight = NULL;
	texteSurface = NULL;
	tombeSurface = NULL;

	//Enregistrement log
	fprintf(logFile, "\ncreateWorms : SUCCESS.\n\n");
	return worms;
}

/**
* \fn void destroyWorms(Worms** wormsTab)
* \brief Détruit une structure Worms et remet ses pointeurs à NULL.
*
* \param[in] wormsTab, tableau de worms a detruire.
*/
void destroyWorms(Worms** wormsTab, int nbWorms)
{
	int i;
	for (i = 0; i < nbWorms; i++)
	{
		if (wormsTab[i]->wormsObject->objectSurface != NULL)
		{
			SDL_FreeSurface(wormsTab[i]->wormsObject->objectSurface);
			(wormsTab[i])->wormsObject->objectSurface = NULL;
		}
		if ((wormsTab[i])->wormsSurfaceLeft != NULL)
		{
			SDL_FreeSurface((wormsTab[i])->wormsSurfaceLeft);
			(wormsTab[i])->wormsSurfaceLeft = NULL;
		}
		if ((wormsTab[i])->wormsSurfaceRight != NULL)
		{
			SDL_FreeSurface((wormsTab[i])->wormsSurfaceRight);
			(wormsTab[i])->wormsSurfaceRight = NULL;
		}
		if ((wormsTab[i])->wormsSurfaceTomb != NULL)
		{
			SDL_FreeSurface((wormsTab[i])->wormsSurfaceTomb);
			(wormsTab[i])->wormsSurfaceTomb = NULL;
		}
		if ((wormsTab[i])->texteSurface != NULL)
		{
			SDL_FreeSurface((wormsTab[i])->texteSurface);
			(wormsTab[i])->texteSurface = NULL;
		}
		if ((wormsTab[i])->wormsObject != NULL)
		{
			free((wormsTab[i])->wormsObject);
			(wormsTab[i])->wormsObject = NULL;
		}
		free(wormsTab[i]);
	}
	*wormsTab = NULL;
	fprintf(logFile, "destroyWorms : DONE.\n");
}











//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Gestion du retournement                /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn void gestionRetournement(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap)
* \brief Detecte un retournement du worms.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] pWorms, pointeur du worms a tester
* \returns void
*/
int swapManagement(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap)
{
	int indexBoucle = 0, correction = 1;
	if (swapWorms(pInput, pWorms))
	{
		swapWormsSurface(pWorms);
		pInput->direction = NONE;
		while (indexBoucle < 60 && detectionCollisionSurface(pSurfaceMap, pWorms->wormsObject->objectSurface))
		{
			if (indexBoucle > 10)
				correction = -1;
			if (pWorms->dirSurface == RIGHT)
				pWorms->wormsObject->objectSurface->clip_rect.x -= correction;
			else pWorms->wormsObject->objectSurface->clip_rect.x += correction;
			indexBoucle++;
		}
		resetAbsoluteCoordinates(pWorms->wormsObject->objectSurface,
			&pWorms->wormsObject->absoluteCoordinate.x,
			&pWorms->wormsObject->absoluteCoordinate.y);
		return 1;
	}
	return 0;
}

/**
* \fn int retournementWorms(Input* pInput, Worms* pWorms)
* \brief Detecte un retournement du worms.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] pWorms, pointeur du worms a tester
* \returns booleen indiquant s'il y a eu retournement :
*			1 = retournement
*			0 = pas de retournement
*/
int swapWorms(Input* pInput, Worms* pWorms)
{
	return (pInput->direction == RIGHT || pInput->direction == LEFT) && (pWorms->dirSurface != pInput->direction);
}

/**
* \fn void swapWormsSurface(Worms* pWorms)
* \brief Change la surface de sens.
*
* \param[in] pWorms, pointeur du worms a tester
* \returns void
* \remark Faire attention a la vitesse d'execution du programme et rajouter un booleen de securite dans la fonction l'appelant
*/
void swapWormsSurface(Worms* pWorms)
{
	int w = pWorms->wormsObject->objectSurface->w, h = pWorms->wormsObject->objectSurface->h;
	if (pWorms->dirSurface == RIGHT)
		pWorms->dirSurface = LEFT;
	else pWorms->dirSurface = RIGHT;
	if (pWorms->dirSurface == RIGHT)
	{
		memcpy(pWorms->wormsObject->objectSurface->pixels, pWorms->wormsSurfaceRight->pixels, w*h*sizeof(Uint32));
	}
	else memcpy(pWorms->wormsObject->objectSurface->pixels, pWorms->wormsSurfaceLeft->pixels, w*h*sizeof(Uint32));
	/*Offset entre la droite et la gauche*/
	/*if (pWorms->dirSurface == RIGHT)
	pWorms->wormsObject->objectSurface->clip_rect.x += 10;
	else pWorms->wormsObject->objectSurface->clip_rect.x -= 10;*/
}







//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Gestion du deplacement                 /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn void setWormsSpeed(Worms* pWorms, enum DIRECTION jumpDirection)
* \brief Set the speed and direction of a jump for a worms.
*
* \param[in] pWorms, pointer to the worms to swap
* \param[in] jumpDirection, direction of the jump
* \returns void
*/
void setWormsSpeed(Worms* pWorms, enum DIRECTION jumpDirection)
{
	switch (jumpDirection)
	{
	case UP:
		setSpeed(&pWorms->wormsObject->Xspeed, &pWorms->wormsObject->Yspeed, 0.0, vitesseY);
		pWorms->wormsObject->motionDirection = UP;
		break;
	case RIGHT:
		setSpeed(&pWorms->wormsObject->Xspeed, &pWorms->wormsObject->Yspeed, vitesseX, vitesseY);
		pWorms->wormsObject->motionDirection = UPRIGHT;
		break;
	case LEFT:
		setSpeed(&pWorms->wormsObject->Xspeed, &pWorms->wormsObject->Yspeed, -vitesseX, vitesseY);
		pWorms->wormsObject->motionDirection = UPLEFT;
		break;
	}
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Gestion de l'animation                 /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn void gestionAnimationWorms(Worms* pWorms, int swap)
* \brief Manage animation of the worms.
*
* \param[in] pWorms, pointer to the worms to swap
* \param[in] pSurfaceMap, pointer to the map surface
* \param[in] swap, indicates if a swap occured.
* \returns void
*/
void gestionAnimationWorms(Worms* pWorms, SDL_Surface* pSurfaceMap, int swap)
{
	int indexBoucle = 0, correction = 1;
	if (swap)
		pWorms->indexAnim = 0;
	animationWorms(pWorms, pWorms->indexAnim, pWorms->dirSurface);
	while (indexBoucle < 60 && detectionCollisionSurface(pSurfaceMap, pWorms->wormsObject->objectSurface))
	{
		if (indexBoucle > 10)
			correction = -1;
		if (pWorms->dirSurface == RIGHT)
			pWorms->wormsObject->objectSurface->clip_rect.x -= correction;
		else pWorms->wormsObject->objectSurface->clip_rect.x += correction;
		indexBoucle++;
	}
	if (pWorms->indexAnim >= 14)
		pWorms->indexAnim = 0;
	else pWorms->indexAnim++;
}

/**
* \fn void animationWorms(Worms* pWorms, int indexFrameAnim, enum DIRECTION direction)
* \brief Realise les animations de deplacement.
*
* \param[in] pWorms, worms a animer
* \param[in] indexFrameAnim, index de la frame a afficher pour l'animation en cours
* \param[in] direction, direction du deplacement
* \returns 1 = affichage frame OK, 0 = problem copy frame
*/
int animationWorms(Worms* pWorms, int indexFrameAnim, enum DIRECTION direction)
{
	SDL_Rect clip;
	switch (direction)
	{
	case RIGHT:
		if (pWorms->dirSurface == RIGHT)
			clip = initRect((24 + indexFrameAnim * 31), 84, widthSpriteMov, hightSpriteMov);
		else	clip = initRect((11 + indexFrameAnim * 31), 28, widthSpriteMov, hightSpriteMov);
		break;
	case LEFT:
		if (pWorms->dirSurface == LEFT)
			clip = initRect((445 - indexFrameAnim * 31), 28, widthSpriteMov, hightSpriteMov);
		else	clip = initRect((458 - indexFrameAnim * 31), 84, widthSpriteMov, hightSpriteMov);
		break;
	case UP:
		break;
	case DOWN:
		break;
	}
	return copySurfacePixels(spriteDeplacement, &clip, pWorms->wormsObject->objectSurface, NULL);
}







//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Gestion de l'overlay                  /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn void updateGameWorms(Input* pInput, Worms** wormsTab, SDL_Texture* pTextureDisplay, SDL_Surface* pSurfaceMap)
* \brief Update les surfaces des worms et gere l'overlay si besoin et la multiple physique.
*
* \param[in] pInput, pointeur de la structure d'inputs
* \param[in] wormsTab, tableau des worms
* \param[in] pTextureDisplay, texture globale d'affichage
* \param[in] pSurfaceMap, surface de la map
* \returns void
*/
void updateGameWorms(Input* pInput, Worms** wormsTab, SDL_Texture* pTextureDisplay, SDL_Surface* pSurfaceMap)
{
	int indexWorms, indexWorms1, indexWorms2;
	for (indexWorms = 0; indexWorms < globalVar.nbWormsEquipe * globalVar.nbEquipe; indexWorms++)
	{
		if (indexWorms == pInput->wormsPlaying || wormsTab[indexWorms]->wormsObject->reactToBomb == 1
			|| !testGround(pSurfaceMap, wormsTab[indexWorms]->wormsObject->objectSurface, 1))
		{
			if (wormsTab[indexWorms]->vie > 0 
				|| (wormsTab[indexWorms]->vie == 0 && !testGround(pSurfaceMap, wormsTab[indexWorms]->wormsObject->objectSurface, 2)))
			{
				KaamWormsMotionManagement(pInput, wormsTab[indexWorms], pSurfaceMap);
			}
			if (deathByLimitMap(wormsTab[indexWorms], pSurfaceMap))
				resetInputs(pInput);
			if (pInput->deplacement)
			{
				pInput->deplacement = 0;
				pInput->raffraichissement = 1;
			}
		}
		updateTextureFromMultipleSurface(pTextureDisplay, pSurfaceMap, wormsTab[indexWorms]->wormsObject->objectSurface, &wormsTab[indexWorms]->wormsObject->objectBox);
		if (wormsOverlay(wormsTab, &indexWorms1, &indexWorms2))
		{
			if (indexWorms1 == pInput->wormsPlaying)
				updateWormsOverlay(wormsTab, pTextureDisplay, pSurfaceMap, indexWorms2, indexWorms1);
			if (indexWorms2 == pInput->wormsPlaying)
			{
				SWAP(indexWorms1, indexWorms2);
				updateWormsOverlay(wormsTab, pTextureDisplay, pSurfaceMap, indexWorms2, indexWorms1);
			}
			pInput->raffraichissement = 1;
		}
	}
}

/**
* \fn int wormsOverlay(Worms** wormsTab, int* indexWorms1, int* indexWorms2)
* \brief Regarde si deux worms se supperpose.
* TO DO :
*			Verifier la superposition de tous les worms.
* \param[in] wormsTab, tableau de worms.
* \param[in] indexWorms1, pointeur vers l'indice du worms 1 en supperposition
* \param[in] indexWorms2, pointeur vers l'indice du worms 2 en supperposition
* \return error, 1 = SUCCESS, 0 = FAIL
*/
int wormsOverlay(Worms** wormsTab, int* indexWorms1, int* indexWorms2)
{
	int i = 0;
	for (i = 1; i < globalVar.nbWormsEquipe * globalVar.nbEquipe; i++)
	{
		if (collisionRectWithRect(&wormsTab[i]->wormsObject->objectSurface->clip_rect, &wormsTab[i - 1]->wormsObject->objectSurface->clip_rect))
		{
			*indexWorms1 = i - 1;
			*indexWorms2 = i;
			return 1;
		}
	}
	return 0;
}










//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Fonctions Diverses                   /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int deathByLimitMap(Worms* pWorms, SDL_Surface* pSurfaceMap)
* \brief Detecte si on est au fond de la map et remplace la surface du worms par une tombe.
*
* \param[in] pWorms, pointeur du worms a tester
* \param[in] pSurfaceMap, surface de la map
* \returns int, 1 = Dead, 0 = vivant
*/
int deathByLimitMap(Worms* pWorms, SDL_Surface* pSurfaceMap)
{
	int dead = 0;
	if ((pWorms->wormsObject->objectSurface->clip_rect.y + pWorms->wormsObject->objectSurface->h >= pSurfaceMap->h - 1))
	{
		pWorms->wormsObject->objectSurface->clip_rect.y = pSurfaceMap->h - pWorms->wormsObject->objectSurface->h;
		wormsDead(pWorms, 1);
		dead = 1;
	}
	return dead;
}

/**
* \fn void wormsDead(Worms* pWorms, int limitMap)
* \brief load the death surface in the worms.
*
* \param[in] pWorms, pointer to the worms.
* \param[in] limitMap, indicates if it is a death by limit of the map.
* \returns void
*/
void wormsDead(Worms* pWorms, int limitMap)
{
	if (pWorms->vie <= 0 || limitMap == 1)
	{
		memcpy(pWorms->wormsObject->objectSurface->pixels, pWorms->wormsSurfaceTomb->pixels, pWorms->wormsSurfaceTomb->w * pWorms->wormsSurfaceTomb->h *sizeof(Uint32));
		pWorms->vie = 0;
	}
}

/**
* \fn void bombReactionManagement(Input* pInput, Worms** wormsTab, SDL_Rect* cercleBox, int centerX, int centerY, int radius)
* \brief Detects if a worms is within the range of a weapon and applies dammage if so.
*
* \param[in] pInput, pointer to the Input structure.
* \param[in] wormsTab, array of worms.
* \param[in] cercleBox, box which contains the cercle of the explosion.
* \param[in] centerX, X coordinate of the center of the cercle of the explosion.
* \param[in] centerY, Y coordinate of the center of the cercle of the explosion.
* \param[in] radius, radius of the explosion.
* \returns void
*/
void bombReactionManagement(Input* pInput, Worms** wormsTab, SDL_Rect* cercleBox, int centerX, int centerY, int radius)
{
	int indexWorms = 0;
	for (indexWorms = 0; indexWorms < globalVar.nbWormsEquipe * globalVar.nbEquipe; indexWorms++)
	{
		if (collisionRectWithRect(cercleBox, &wormsTab[indexWorms]->wormsObject->objectBox))
		{
			if (indexWorms != pInput->wormsPlaying || !pInput->jumpOnGoing)
			{
				wormsTab[indexWorms]->wormsObject->reactToBomb = 1;
				speedBombReaction(wormsTab[indexWorms], centerX, centerY, radius);
			}
			continue;
		}
		/*for (i = 0; i < 4; i++)
		{
		p.x = wormsTab[indexWorms]->wormsObject->objectBox.x + (i == 1)*wormsTab[indexWorms]->wormsObject->objectBox.w;
		p.y = wormsTab[indexWorms]->wormsObject->objectBox.y + (i == 2 || i == 3)*wormsTab[indexWorms]->wormsObject->objectBox.h;
		if (collisionPointWithCercle(p, centerX, centerY, radius))
		{
		wormsTab[indexWorms]->wormsObject->reactToBomb = 1;
		speedBombReaction(wormsTab[indexWorms], centerX, centerY, radius);
		continue;
		}
		}
		p.x = centerX;
		p.y = centerY;
		if (collisionPointWithRect(p, &wormsTab[indexWorms]->wormsObject->objectBox))
		{
		wormsTab[indexWorms]->wormsObject->reactToBomb = 1;
		speedBombReaction(wormsTab[indexWorms], centerX, centerY, radius);
		continue;
		}*/
	}
	pInput->bombe = 0;
}

/**
* \fn void speedBombReaction(Worms* pWorms, int centerX, int centerY, int radius
* \brief Applies speed and dammage according to the position of the worms to the center of the explosion.
*
* \param[in] pWorms, pointer to the worms.
* \param[in] centerX, X coordinate of the center of the cercle of the explosion.
* \param[in] centerY, Y coordinate of the center of the cercle of the explosion.
* \param[in] radius, radius of the explosion.
* \returns void
*/
void speedBombReaction(Worms* pWorms, int centerX, int centerY, int radius)
{
	int middleX = pWorms->wormsObject->objectBox.x + pWorms->wormsObject->objectBox.w / 2;
	int middleY = pWorms->wormsObject->objectBox.y + pWorms->wormsObject->objectBox.h / 2;
	float signeX = 1.0, signeY = 1.0, distanceX = 0, distanceY = 0;
	if (middleX < centerX)
	{
		signeX = -1.0;
		if (middleY > centerY)
		{
			signeY = -1.0;
		}
	}
	//calcul de la distance par rapport au centre
	distanceX = MY_ABS((middleX - centerX)) / (float)radius;
	distanceY = MY_ABS((middleY - centerY)) / (float)radius;
	double decrease = sqrt(CARRE(distanceX) + CARRE(distanceY));
	if (decrease <= 0.35)
	{
		pWorms->wormsObject->Xspeed = (float)(vitesseX * BombFactor * signeX*1.0 / 0.35);
		pWorms->wormsObject->Yspeed = (float)(vitesseY * BombFactor * signeY*1.0 / 0.35);
		pWorms->vie -= (int)(MAXDAMMAGE * 1.0 / 0.35);
	}
	else
	{
		pWorms->wormsObject->Xspeed = (float)(vitesseX * BombFactor * signeX * 1.0 / decrease);
		pWorms->wormsObject->Yspeed = (float)(vitesseY * BombFactor * signeY * 1.0 / decrease);
		pWorms->vie -= (int)(MAXDAMMAGE * 1.0 / decrease);
	}
	wormsDead(pWorms, 0);
}



