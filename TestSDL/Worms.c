#include "worms.h"
#include "AffichageGeneral.h"
#include "Libraries.h"

/**
* \fn Worms* createWorms(const char *file)
* \brief Créé et initialise une structure worms.
*
* \param[in] file, chaine de caractères correspondant au nom du fichier image du worms.
*
* \returns pointeur vers la structure worms créé, NULL si echec
*/
Worms* createWorms(const char *file1, const char *file2)
{
	Worms * worms = NULL;
	SDL_Surface* wormsSurface = NULL;
	SDL_Surface * wormsSurfaceLeft = NULL;
	SDL_Surface * wormsSurfaceRight = NULL;
	worms = (Worms*)malloc(sizeof(Worms));
	if (worms == NULL)
	{
		printf("Probleme lors de l'allocation de memoire du worms");
		return NULL;
	}
	worms->wormsSurfaceLeft = NULL;
	worms->wormsSurfaceRight = NULL;
	worms->wormsSurface = NULL;
	worms->vitx = 0;
	worms->vity = (float)(sin(3.1415 / 3) * 0.6);

	wormsSurfaceLeft = loadImage(file1);
	wormsSurfaceRight = loadImage(file2);
	if (wormsSurfaceLeft == NULL || wormsSurfaceRight == NULL)
	{
		printf("Erreur création surface worms, %s", SDL_GetError());
		destroyWorms(&worms);
		return NULL;
	}
	wormsSurface = SDL_CreateRGBSurface(0, wormsSurfaceLeft->w, wormsSurfaceLeft->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	if (wormsSurface == NULL)
	{
		printf("Erreur allocation memoire");
		destroyWorms(&worms);
		return NULL;
	}
	worms->wormsSurfaceLeft = wormsSurfaceLeft;
	worms->wormsSurfaceRight = wormsSurfaceRight;
	//A changer avec un truc adapté à la map ?
	worms->wormsSurface = wormsSurface;
	worms->wormsSurface->pixels = worms->wormsSurfaceRight->pixels;
	worms->wormsSurface->clip_rect.x = 100;
	worms->wormsSurface->clip_rect.y = 100;
	worms->wormsRect.x = worms->wormsSurface->clip_rect.x;
	worms->wormsRect.y = worms->wormsSurface->clip_rect.y;
	worms->wormsRect.w = wormsSurfaceLeft->clip_rect.w;
	worms->wormsRect.h = wormsSurfaceLeft->clip_rect.h;
	worms->xAbs = worms->wormsSurface->clip_rect.x;
	worms->yAbs = worms->wormsSurface->clip_rect.y;
	wormsSurface = NULL;
	wormsSurfaceLeft = NULL;
	wormsSurfaceRight = NULL;
	return worms;
}

/**
* \fn void destroyWorms(Worms** worms)
* \brief Détruit une structure Worms et remet son pointeur à NULL.
*
* \param[in] worms, adresse du pointeur de la structure du Worms à détruire.
*/
void destroyWorms(Worms** worms)
{
	if ((*worms)->wormsSurface != NULL)
	{
		SDL_FreeSurface((*worms)->wormsSurface);
		(*worms)->wormsSurface = NULL;
	}
	if ((*worms)->wormsSurfaceLeft != NULL)
	{
		SDL_FreeSurface((*worms)->wormsSurfaceLeft);
		(*worms)->wormsSurfaceLeft = NULL;
	}
	if ((*worms)->wormsSurfaceRight != NULL)
	{
		SDL_FreeSurface((*worms)->wormsSurfaceRight);
		(*worms)->wormsSurfaceRight = NULL;
	}
	free(*worms);
	*worms = NULL;
}


//Déplace un worms
void deplacementWorms(Input* pInput, Worms* worms, SDL_Surface* surfaceCollision)
{
	int  t = 0;
	int collision = 0;
	enum DIRECTION dir = NONE;
	if (pInput->right)
	{
		worms->wormsSurface->clip_rect.x += pInput->acceleration;
		worms->wormsSurface->pixels = worms->wormsSurfaceRight->pixels;
		dir = RIGHT;
		collision = detectionCollisionSurface(surfaceCollision, worms->wormsSurface, &dir);
		if (collision)
		{
			worms->wormsSurface->clip_rect.x -= pInput->acceleration;
			collision = detectionCollisionSurface(surfaceCollision, worms->wormsSurface, &dir);
		}
		while (collision && t < 5)
		{
			worms->wormsSurface->clip_rect.x += pInput->acceleration;
			collision = detectionCollisionSurface(surfaceCollision, worms->wormsSurface, &dir);
			t++;
		}
		t = 0;
		dir = NONE;
		pInput->right = 0;
	}
	if (pInput->left)
	{

		worms->wormsSurface->clip_rect.x -= pInput->acceleration;
		worms->wormsSurface->pixels = worms->wormsSurfaceLeft->pixels;
		dir = LEFT;
		collision = detectionCollisionSurface(surfaceCollision, worms->wormsSurface, &dir);
		if (collision)
		{
			worms->wormsSurface->clip_rect.x += pInput->acceleration;
			collision = detectionCollisionSurface(surfaceCollision, worms->wormsSurface, &dir);
		}
		while (collision && t < 5)
		{
			worms->wormsSurface->clip_rect.x -= pInput->acceleration;
			collision = detectionCollisionSurface(surfaceCollision, worms->wormsSurface, &dir);
			t++;
		}
		t = 0;
		dir = NONE;
		pInput->left = 0;
	}
	if (pInput->down)
	{
		worms->wormsSurface->clip_rect.y += pInput->acceleration;
		collision = detectionCollisionSurface(surfaceCollision, worms->wormsSurface, &dir);
		if (collision)
		{
			worms->wormsSurface->clip_rect.y -= pInput->acceleration;
		}
		dir = NONE;
		pInput->down = 0;
	}
	if (pInput->up)
	{
		worms->wormsSurface->clip_rect.y -= pInput->acceleration;
		collision = detectionCollisionSurface(surfaceCollision, worms->wormsSurface, &dir);
		if (collision)
		{
			worms->wormsSurface->clip_rect.y += pInput->acceleration;
		}
		dir = NONE;
		pInput->up = 0;
	}
}