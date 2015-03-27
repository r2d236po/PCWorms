#include "worms.h"
#include "AffichageGeneral.h"
#include "physique.h"

/**
* \fn Worms* createWorms(const char *file)
* \brief Créé et initialise une structure worms.
*
* \param[in] file, chaine de caractères correspondant au nom du fichier image du worms.
*
* \returns pointeur vers la structure worms créé, NULL si echec
*/
Worms* createWorms(char* name)
{
	Worms * worms = NULL;
	SDL_Surface* wormsSurface = NULL;
	SDL_Surface * wormsSurfaceLeft = NULL;
	SDL_Surface * wormsSurfaceRight = NULL;
	SDL_Surface * texteSurface = NULL;
	SDL_Surface * tombeSurface = NULL;
	if (logFile != NULL)
		fprintf(logFile, "createWorms : START :\n\n");
	worms = (Worms*)malloc(sizeof(Worms));
	if (worms == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "createWorms : FAILURE, allocation memoire worms.\n\n");
		return NULL;
	}
	worms->wormsSurfaceLeft = NULL;
	worms->wormsSurfaceRight = NULL;
	worms->wormsSurface = NULL;
	worms->texteSurface = NULL;
	wormsSurfaceLeft = loadImage("../assets/pictures/worms_G.png");
	wormsSurfaceRight = loadImage("../assets/pictures/worms_D.png");
	tombeSurface = loadImage("../assets/pictures/Tombe2_SD.png");
	if (wormsSurfaceLeft == NULL || wormsSurfaceRight == NULL || tombeSurface == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "createWorms : FAILURE, loadImage.\n\n");
		destroyWorms(&worms, 1);
		return NULL;
	}
	wormsSurface = SDL_CreateRGBSurface(0, wormsSurfaceLeft->w, wormsSurfaceLeft->h, 32, RMASK, GMASK, BMASK, AMASK);
	if (wormsSurface == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "createWorms : FAILURE, createRGBSurface : %s.\n\n", SDL_GetError());
		destroyWorms(&worms, 1);
		return NULL;

	}

	//texteSurface = TTF_RenderText_Blended(font.FontName, worms->nom, font.couleurNameBleu);

	worms->texteSurface = texteSurface;
	worms->wormsSurfaceLeft = wormsSurfaceLeft;
	worms->wormsSurfaceRight = wormsSurfaceRight;
	worms->wormsSurface = wormsSurface;
	worms->wormsSurface->pixels = worms->wormsSurfaceRight->pixels;
	worms->wormsSurfaceTomb = tombeSurface;

	//Initialisations liees a l'image du worms
	worms->wormsSurface->clip_rect.x = 300;
	worms->wormsSurface->clip_rect.y = 100;
	worms->wormsRect.x = worms->wormsSurface->clip_rect.x;
	worms->wormsRect.y = worms->wormsSurface->clip_rect.y;
	worms->wormsRect.w = wormsSurfaceLeft->clip_rect.w;
	worms->wormsRect.h = wormsSurfaceLeft->clip_rect.h;

	//initialisation des variables autres
	worms->vie = 100;
	worms->nom = name;
	//worms->invent = initInvent(Worms* worms); A FAIRE
	worms->xAbs = worms->wormsSurface->clip_rect.x;
	worms->yAbs = worms->wormsSurface->clip_rect.y;
	worms->vitx = 0;
	worms->vity = 0;
	worms->dirSurface = RIGHT;
	worms->dir = DOWN;
	worms->arme = NULL;

	//remise à null des pointeurs temporaires
	wormsSurface = NULL;
	wormsSurfaceLeft = NULL;
	wormsSurfaceRight = NULL;
	texteSurface = NULL;
	tombeSurface = NULL;

	//Enregistrement log
	if (logFile != NULL)
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
		if (wormsTab[i]->wormsSurface != NULL)
		{
			SDL_FreeSurface(wormsTab[i]->wormsSurface);
			(wormsTab[i])->wormsSurface = NULL;
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
		free(wormsTab[i]);
	}
	*wormsTab = NULL;
	if (logFile != NULL)
		fprintf(logFile, "destroyWorms : DONE.\n");
}


/**
* \fn void deplacementWorms(Input* pInput, Worms* pWorms, SDL_Surface* surfaceCollision, int* retournement, enum DIRECTION* dir)
* \brief Deplace un worms dans l'espace.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] pWorms, pointeur du worms a deplacer
* \param[in] surfaceCollision, pointeur vers la surface de la map
* \param[in/out] retournement, parametre indiquant un retournement du worms, peut etre modifie par la fonction
* \param[in/out] dir, direction de deplacement du worms, peut etre modifie par la fonction
*/
void deplacementWorms(Input* pInput, Worms* pWorms, SDL_Surface* surfaceCollision)
{
	int deplacement = 0;
	if (pInput->direction != UP)
	{
		switch (pWorms->dir)
		{
		case RIGHT:
			deplacement = pInput->acceleration;
			break;
		case LEFT:
			deplacement = -pInput->acceleration;
			break;
		case UP:
			deplacement = -pInput->acceleration;
			break;
		case DOWN:
			deplacement = pInput->acceleration;
			break;
		}
		if (pWorms->dir == RIGHT || pWorms->dir == LEFT)
		{
			pWorms->wormsSurface->clip_rect.x += deplacement;
		}
		else if (pWorms->dir == UP || pWorms->dir == DOWN)
		{
			pWorms->wormsSurface->clip_rect.y += deplacement;
		}
		if ((pWorms->wormsSurface->clip_rect.x + pWorms->wormsSurface->w) > surfaceCollision->w || pWorms->wormsSurface->clip_rect.x < 0)
		{
			pWorms->wormsSurface->clip_rect.x -= deplacement;
		}
		if ((pWorms->wormsSurface->clip_rect.y + pWorms->wormsSurface->h) > surfaceCollision->h || pWorms->wormsSurface->clip_rect.y < 0)
		{
			pWorms->wormsSurface->clip_rect.y -= deplacement;
		}
		pInput->direction = NONE;
	}

}



/**
* \fn char retournementWorms(Input* pInput, Worms* pWorms)
* \brief Detecte un retournement du worms.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] pWorms, pointeur du worms a tester
* \returns retournement, booleen indiquant s'il y a eu retournements
*/
char retournementWorms(Input* pInput, Worms* pWorms)
{
	char retournement = 0;
	if (pInput->direction != NONE)
	{
		if ((pInput->direction == RIGHT || pInput->direction == LEFT) && (pWorms->dirSurface != pInput->direction))
		{
			retournement = 1;
		}
	}
	return retournement;
}

/**
* \fn void swapSurface(Worms* pWorms)
* \brief Change la surface de sens.
*
* \param[in] pWorms, pointeur du worms a tester
* \returns void
* \remark Faire attention a la vitesse d'execution du programme et rajouter un booleen de securite dans la fonction l'appelant
*/
void swapSurface(Worms* pWorms)
{
	if (pWorms->dirSurface == RIGHT)
		pWorms->dirSurface = LEFT;
	else pWorms->dirSurface = RIGHT;
	if (pWorms->dirSurface == RIGHT)
	{
		pWorms->wormsSurface->pixels = pWorms->wormsSurfaceRight->pixels;
	}
	else pWorms->wormsSurface->pixels = pWorms->wormsSurfaceLeft->pixels;
	if (pWorms->dirSurface == RIGHT)
		pWorms->wormsSurface->clip_rect.x += 10;
	else pWorms->wormsSurface->clip_rect.x -= 10;
}

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
	if ((pWorms->wormsSurface->clip_rect.y + pWorms->wormsSurface->h == pSurfaceMap->h) && checkDeplacement(pSurfaceMap, pWorms->wormsSurface, DOWN))
	{
		pWorms->wormsSurface->pixels = pWorms->wormsSurfaceTomb->pixels;
		pWorms->vie = 0;
		dead = 1;
	}
	return dead;
}

/**
* \fn void updateWorms(Worms** wormsTab, SDL_Surface* pSurfaceMap,Input* pInput, SDL_Texture* textureDisplay)
* \brief Update les surfaces des worms et gere l'overlay si besoin.
*
* \param[in] wormsTab, tableau des worms
* \param[in] pSurfaceMap, surface de la map
* \param[in] pInput, pointeur de la structure d'inputs
* \param[in] textureDisplay, texture globale d'affichage
* \returns void
*/
void updateWorms(Worms** wormsTab, SDL_Surface* pSurfaceMap, Input* pInput, SDL_Texture* pTextureDisplay)
{
	int i = 0;
	for (i = 0; i < globalVar.nbWormsEquipe; i++)
	{
		updateGlobaleTexture(pSurfaceMap, wormsTab[i]->wormsSurface, pTextureDisplay, &wormsTab[i]->wormsRect);
		if (wormsOverlay(wormsTab))
			updateWormsOverlay(wormsTab, pTextureDisplay, globalVar.nbWormsEquipe - pInput->wormsPlaying - 1, pInput->wormsPlaying);
	}
}





