#include "carte.h"
#include "AffichageGeneral.h"
#include "Libraries.h" //Inclus toutes les librairies


/**
* \fn int initialisionTerrain(Terrain** map, SDL_Renderer * pRenderer, const char * file, const char * file2)
* \brief Initialise un terrain avec son image de fond et l'image de la map.
*
* \param[in] surface, surface dans laquelle on lit le pixel
* \param[in] file, nom du fichier de l'image de fond
* \param[in] file2, nom du fichier de l'image de la map
* \returns int, Indicateur de reussite de la fonction : 1 = succes, -1 = echec
*/
int initialisionTerrain(Terrain** map, SDL_Renderer * pRenderer, const char * file, const char * file2)
{
	Terrain * mapTemp = NULL;
	SDL_Rect back = { 0, 0, 0, 0 };

	//Creation du pointeur de Terrain
	mapTemp = (Terrain*)malloc(sizeof(Terrain));
	if (mapTemp == NULL)
	{
		printf("Probleme de lors de l'allocation memoire du terrain");
		return -1;
	}
	mapTemp->imageBackground = NULL;
	mapTemp->imageMapSurface = NULL;

	//Creation de la texture de l'image de fond
	mapTemp->imageBackground = loadTexture(pRenderer, file);
	if (mapTemp->imageBackground == NULL)
	{
		printf("Probleme de lors de la creation de la texture Background");
		destroyMap(&mapTemp);
		return -1;
	}

	//Creation de la surface de la map qui servira pour la collision
	mapTemp->imageMapSurface = loadImage(file2);
	if (mapTemp->imageMapSurface == NULL) //Verification qu'il n'y a pas eu d'erreur lors de l'allocation en mémoire
	{
		printf("Probleme de lors de la creation de la surface de la map");
		destroyMap(&mapTemp);
		return -1;
	}
	SDL_RenderCopy(pRenderer, mapTemp->imageBackground, NULL, &back);
	SDL_RenderPresent(pRenderer);
	(*map) = mapTemp; //récupération du pointeur du terrain
	mapTemp = NULL;
	return 1;
}


/**
* \fn Uint32 ReadPixel(SDL_Surface *surface, int x, int y)
* \brief Lit un pixel aux coordonnees x et y d'une surface.
*
* \param[in] surface, surface dans laquelle on lit le pixel
* \param[in] x, abscisse dans la surface
* \param[in] y, ordonne dans la surface
* \returns Uint32, pixel lu aux coordonnees contenant les 4 canaux (RGBA)
* \remarks Faire attention que x et y soient bien compris dans la surface.
*/
Uint32 ReadPixel(SDL_Surface *surface, int x, int y)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	//Get the requested pixel
	return pixels[(y * surface->w) + x];
}

/**
* \fn void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
* \brief Ecrit un pixel aux coordonnees x et y d'une surface.
*
* \param[in] surface, surface dans laquelle on ecrit le pixel
* \param[in] x, abscisse dans la surface
* \param[in] y, ordonne dans la surface
* \param[in] pixel, pixel a ecrire
* \remarks Faire attention que x et y soient bien compris dans la surface.
*/
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;

	//Set the pixel
	pixels[(y * surface->w) + x] = pixel;
}

/**
* \fn int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Rect* pRect)
* \brief Detecte s'il y a collision entre un rectangle et une surface.
*
* \param[in] pRenderer, pointeur vers le renderer de la fenetre
* \param[in] pSurface, pointeur vers la surface de la surface a tester
* \param[in] xE, pointeur vers la variable contenant l'abscisse de la collision
* \param[in] yE, pointeur vers la variable contenant l'ordonne de la collision
* \param[in] pRect, pointeur vers le rectangle a tester
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Rect* pRect)
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	Uint32 p = ReadPixel(pSurface, 0, 0);
	Uint8 r = 0, g = 0, b = 0, a = 0;
	int x = 0, y = 0, w = 0, h = 0;
	rect = *pRect;
	int collision = 0;
	SDL_GetRendererOutputSize(pRenderer, &w, &h);
	if ((w - rect.x <= rect.w) || (rect.x <= 0) || (rect.y <= 0) || (h - rect.y) <= rect.h)
	{
		return 1;
	}
	for (y = rect.y; (y <= rect.h + rect.y) && (collision == 0); y++)
	{
		if (y == rect.y || y == (rect.h + rect.y))
		{
			for (x = rect.x; (x <= rect.x + rect.w) && (collision == 0); x++)
			{
				p = ReadPixel(pSurface, x, y);
				SDL_GetRGBA(p, pSurface->format, &r, &g, &b, &a);
				if (a < 100) //transparence
				{
					collision = 0;
				}
				else
				{
					*xE = x;
					*yE = y;
					collision = 1;
				}
			}
		}
		else
		{
			p = ReadPixel(pSurface, rect.x, y);
			SDL_GetRGBA(p, pSurface->format, &r, &g, &b, &a);
			if (a < 100) //transparence
			{
				collision = 0;
			}
			else
			{
				*xE = rect.x + rect.w;
				*yE = y;
				collision = 1;
			}
			p = ReadPixel(pSurface, rect.x + rect.w, y);
			SDL_GetRGBA(p, pSurface->format, &r, &g, &b, &a);
			if (a < 100) //transparence
			{
				collision = 0;
			}
			else
			{
				*xE = rect.x + rect.w;
				*yE = y;
				collision = 1;
			}
		}
	}
	return collision;
}

/**
* \fn int detectionCollisionSurface(SDL_Surface* pSurface, SDL_Surface* pSurface2)
* \brief Detecte s'il y a collision entre deux surfaces.
*
* \param[in] pSurface, pointeur vers la surface de la map
* \param[in] pSurface2, pointeur vers la surface en mouvement dans la map
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int detectionCollisionSurface(SDL_Surface* pSurface, SDL_Surface* pSurface2, enum DIRECTION* dir)
{
	Uint32 p = ReadPixel(pSurface, 0, 0);
	Uint8 r = 0, g = 0, b = 0, a = 0;
	Uint32 p2 = ReadPixel(pSurface, 0, 0);
	Uint8 r2 = 0, g2 = 0, b2 = 0, a2 = 0;
	int x = 0, y = 0;
	int collision = 0;
	if ((pSurface->w - pSurface2->clip_rect.x < pSurface2->clip_rect.w) || (pSurface2->clip_rect.x < 0) || (pSurface2->clip_rect.y < 0) || (pSurface->h - pSurface2->clip_rect.y) < pSurface2->clip_rect.h)
	{
		return 1;
	}
	for (y = pSurface2->clip_rect.y; (y < pSurface2->clip_rect.h + pSurface2->clip_rect.y) && (collision == 0); y++)
	{
		for (x = pSurface2->clip_rect.x; (x < pSurface2->clip_rect.x + pSurface2->clip_rect.w) && (collision == 0); x++)
		{
			p = ReadPixel(pSurface, x, y);
			p2 = ReadPixel(pSurface2, x - pSurface2->clip_rect.x, y - pSurface2->clip_rect.y);
			SDL_GetRGBA(p, pSurface->format, &r, &g, &b, &a);
			SDL_GetRGBA(p2, pSurface2->format, &r2, &g2, &b2, &a2);
			if (a != 255 || a2 != 255) //transparence
			{
				collision = 0;
			}
			else
			{
				if (dir != NULL)
				{
					*dir = calculDirection(x - pSurface2->clip_rect.x, y - pSurface2->clip_rect.y, *dir, pSurface2->w, pSurface2->h);
				}
				collision = 1;
			}
		}
	}
	return collision;
}

/**
* \fn int calculDirection(int y, int x)
*
* \brief Deteremine la direction de la collision.
*
* \param[in] y, ordonnée de la collision
* \param[in] x, abscisse de la collision
* \param[in] impulse, direction du déplacement
* \param[in] w, largeur de l'objet
* \param[in] h, hauteur de l'objet
* \return int, direction de la collision
*/

enum DIRECTION calculDirection(int x, int y, enum DIRECTION impulse, int w, int h)
{
	if ((impulse == RIGHT && x < (w / 2)) || (impulse == LEFT && x >(w / 2)))
	{
		return NONE;
	}
	if (y > (h / 8) && y < (7 * h / 8))
	{
		return impulse; //retourne soit RIGHT si impulse est droite soit LEFT si impulse est gauche
	}
	else if (y <= (h / 8))
	{
		return UP;
	}
	else if (y >= (7 * h / 8))
	{
		return DOWN;
	}
	return NONE;
}



int gestionPhysique(SDL_Renderer* pRenderer, Terrain* map, SDL_Texture* pDisplay, Input* pInput, Uint32* tPrevious, ...)
{
	const double g = 9.81; //9.81 m/s ==> 37081.8 pix/s = 37 pix/ms
	const double pi = 3.1415;
	float vit_y = 0;
	float vit = 0;
	static int xRel = 0, yRel = 0, t = 0;
	enum DIRECTION dir;
	Worms* worms = NULL;
	//Arme* weapon = NULL;
	Uint32 time = 0;
	Uint32 tNow = SDL_GetTicks();
	va_list list;

	time = tNow - (*tPrevious);

	va_start(list, tPrevious);
	switch (va_arg(list, int))
	{
	case 0:
		worms = va_arg(list, Worms*);
		worms->wormsSurface->clip_rect.x = worms->xAbs;
		worms->wormsSurface->clip_rect.y = worms->yAbs;
		xRel = (int)(worms->vitx *time);
		yRel = (int)((worms->vity*time) - ((g*time*time) / 2000));

		//On calcule maintenant les valeurs abs
		worms->wormsSurface->clip_rect.x = worms->wormsSurface->clip_rect.x + xRel;
		if (pInput->jump)
		{
			worms->vitx = (float) (cos(pi / 3)*0.6);
			worms->wormsSurface->clip_rect.y = worms->wormsSurface->clip_rect.y - yRel;
			pInput->jump = 0;
		}
		else worms->wormsSurface->clip_rect.y = worms->wormsSurface->clip_rect.y + yRel;

		t += 10;
		if (detectionCollisionSurface(map->imageMapSurface, worms->wormsSurface, &dir))
		{
			worms->wormsSurface->clip_rect.x = worms->wormsSurface->clip_rect.x - xRel;
			worms->wormsSurface->clip_rect.y = worms->wormsSurface->clip_rect.y - yRel;
			worms->xAbs = worms->wormsSurface->clip_rect.x;
			worms->yAbs = worms->wormsSurface->clip_rect.y;
			t = 0;
		}
		break;
	case 1:
		//cas d'une arme
		break;
	}

	*tPrevious = SDL_GetTicks();
	va_end(list);
	return 0;
}