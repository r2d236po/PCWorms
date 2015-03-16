#include "worms.h"
#include "AffichageGeneral.h"

/**
* \fn Worms* createWorms(const char *file)
* \brief Créé et initialise une structure worms.
*
* \param[in] file, chaine de caractères correspondant au nom du fichier image du worms.
*
* \returns pointeur vers la structure worms créé, NULL si echec
*/
Worms* createWorms(const char name)
{
	Worms * worms = NULL;
	SDL_Surface* wormsSurface = NULL;
	SDL_Surface * wormsSurfaceLeft = NULL;
	SDL_Surface * wormsSurfaceRight = NULL;
	SDL_Surface * texteSurface = NULL;
	worms = (Worms*)malloc(sizeof(Worms));
	if (worms == NULL)
	{
		printf("Probleme lors de l'allocation de memoire du worms");
		return NULL;
	}
	worms->wormsSurfaceLeft = NULL;
	worms->wormsSurfaceRight = NULL;
	worms->wormsSurface = NULL;
	worms->texteSurface = NULL;
	wormsSurfaceLeft = loadImage("../assets/pictures/wormsg.png");
	wormsSurfaceRight = loadImage("../assets/pictures/wormsd.png");

	//worms->nom = name;

	if (wormsSurfaceLeft == NULL || wormsSurfaceRight == NULL)
	{
		printf("Erreur création surface worms, %s", SDL_GetError());
		destroyWorms(&worms, 1);
		return NULL;
	}
	wormsSurface = SDL_CreateRGBSurface(0, wormsSurfaceLeft->w, wormsSurfaceLeft->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	if (wormsSurface == NULL)
	{
		printf("Erreur allocation memoire");
		destroyWorms(&worms, 1);
		return NULL;
	}

	//texteSurface = TTF_RenderText_Blended(font.FontName, worms->nom, font.couleurNameBleu);

	worms->texteSurface = texteSurface;
	worms->wormsSurfaceLeft = wormsSurfaceLeft;
	worms->wormsSurfaceRight = wormsSurfaceRight;
	worms->wormsSurface = wormsSurface;
	worms->wormsSurface->pixels = worms->wormsSurfaceRight->pixels;

	//A changer avec un truc adapté à la map ?
	worms->wormsSurface->clip_rect.x = 100;
	worms->wormsSurface->clip_rect.y = 100;
	worms->wormsRect.x = worms->wormsSurface->clip_rect.x;
	worms->wormsRect.y = worms->wormsSurface->clip_rect.y;
	worms->wormsRect.w = wormsSurfaceLeft->clip_rect.w;
	worms->wormsRect.h = wormsSurfaceLeft->clip_rect.h;

	//initialisation des variables autres
	worms->xAbs = worms->wormsSurface->clip_rect.x;
	worms->yAbs = worms->wormsSurface->clip_rect.y;
	worms->vitx = 0;
	worms->vity = 0;
	worms->dir = RIGHT;

	//remise à null des pointeurs temporaires
	wormsSurface = NULL;
	wormsSurfaceLeft = NULL;
	wormsSurfaceRight = NULL;
	texteSurface = NULL;
	return worms;
}

/**
* \fn void destroyWorms(Worms** worms)
* \brief Détruit une structure Worms et remet ses pointeurs à NULL.
*
* \param[in] worms, adresse du pointeur de la structure du Worms à détruire.
*/
void destroyWorms(Worms** worms, int nbWorms)
{
	int i;
	for ( i = 0; i < nbWorms; i++)
	{
		if (worms[i]->wormsSurface != NULL)
		{
			SDL_FreeSurface(worms[i]->wormsSurface);
			(worms[i])->wormsSurface = NULL;
		}
		if ((worms[i])->wormsSurfaceLeft != NULL)
		{
			SDL_FreeSurface((worms[i])->wormsSurfaceLeft);
			(worms[i])->wormsSurfaceLeft = NULL;
		}
		if ((worms[i])->wormsSurfaceRight != NULL)
		{
			SDL_FreeSurface((worms[i])->wormsSurfaceRight);
			(worms[i])->wormsSurfaceRight = NULL;
		}
		free(worms[i]);
	}
	
	*worms = NULL;
}


/**
* \fn void deplacementWorms(Input* pInput, Worms* worms, SDL_Surface* surfaceCollision, int* retournement, enum DIRECTION* dir)
* \brief Deplace un worms dans l'espace.
*
* \param[in] pInput, pointeur de la structure contenant les Input.
* \param[in] worms, pointeur du worms a deplacer
* \param[in] surfaceCollision, pointeur vers la surface de la map
* \param[in/out] retournement, parametre indiquant un retournement du worms, peut etre modifie par la fonction
* \param[in/out] dir, direction de deplacement du worms, peut etre modifie par la fonction
*/
void deplacementWorms(Input* pInput, Worms* worms, SDL_Surface* surfaceCollision, enum DIRECTION* dir)
{
	int deplacement = 0;
	if (pInput->direction != UP)
	{
		if (!pInput->jumpOnGoing)
		{
			switch (pInput->direction) 
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
			if (pInput->direction == RIGHT || pInput->direction == LEFT)
			{
				worms->wormsSurface->clip_rect.x += deplacement;
			}
			else if (pInput->direction == UP || pInput->direction == DOWN)
			{
				worms->wormsSurface->clip_rect.y += deplacement;
			}
		}
		if ((worms->wormsSurface->clip_rect.x + worms->wormsSurface->w) > surfaceCollision->w || worms->wormsSurface->clip_rect.x < 0)
		{
			worms->wormsSurface->clip_rect.x -= deplacement;
		}
		if ((worms->wormsSurface->clip_rect.y + worms->wormsSurface->h) > surfaceCollision->h || worms->wormsSurface->clip_rect.y < 0)
		{
			worms->wormsSurface->clip_rect.y -= deplacement;
		}
		if (detectionCollisionSurfaceV2(surfaceCollision, worms->wormsSurface, dir)&& pInput->direction != NONE)
			*dir = pInput->direction;
	}
	pInput->direction = NONE;
}


char retournementWorms(Input* pInput, Worms* worms)
{
	char retournement = 0;
	if (pInput->direction != NONE)
	{
		if ((pInput->direction == RIGHT || pInput->direction == LEFT) && (worms->dir != pInput->direction))
		{
			retournement = 1;
			worms->dir = pInput->direction;
			if (worms->dir == RIGHT)
			{
				worms->wormsSurface->pixels = worms->wormsSurfaceRight->pixels;
			}
			else worms->wormsSurface->pixels = worms->wormsSurfaceLeft->pixels;
		}
	}
	return retournement;
}


