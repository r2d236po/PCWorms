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
* \fn void destroyMap(Terrain** map)
* \brief Détruit une structure Terrain et remet son pointeur à NULL.
*
* \param[in] map, adresse du pointeur de la structure du Terrain à détruire.
*/
void destroyMap(Terrain** map)
{
	if ((*map)->imageBackground != NULL)
	{
		SDL_DestroyTexture((*map)->imageBackground);
		(*map)->imageBackground = NULL;
	}
	if ((*map)->imageMapSurface != NULL)
	{
		SDL_FreeSurface((*map)->imageMapSurface);
		(*map)->imageMapSurface = NULL;
	}
	free((*map));
	*map = NULL;
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
int detectionCollisionSurface(SDL_Surface* pSurface, SDL_Surface* pSurface2)
{
	//Variables d'acquisitions
	Uint32 pixelS1 = 0;
	Uint8 r = 0, g = 0, b = 0, a = 0;
	Uint32 pixelS2 = 0;
	Uint8 r2 = 0, g2 = 0, b2 = 0, a2 = 0;
	int offset_xS1 = pSurface->clip_rect.x;
	int offset_yS1 = pSurface->clip_rect.y;
	int offset_xS2 = pSurface2->clip_rect.x;
	int offset_yS2 = pSurface2->clip_rect.y;
	SDL_PixelFormat* formatS1 = pSurface->format;
	SDL_PixelFormat* formatS2 = pSurface2->format;
	//Variables de balayage
	int x = 0, y = 0;
	//Varable de collision
	int collision = 0;

	//Test des limites de la map et de la fenetre
	if ((pSurface->w - pSurface2->clip_rect.x < pSurface2->clip_rect.w) || (pSurface2->clip_rect.x < 0) || (pSurface2->clip_rect.y < 0) || (pSurface->h - pSurface2->clip_rect.y) < pSurface2->clip_rect.h)
	{
		return 1;
	}
	for (y = pSurface2->clip_rect.y; (y < pSurface2->clip_rect.h + pSurface2->clip_rect.y) && (collision == 0); y++)
	{
		for (x = pSurface2->clip_rect.x; (x < pSurface2->clip_rect.x + pSurface2->clip_rect.w) && (collision == 0); x++)
		{
			//Acquisition pixel surface 1
			pixelS1 = ReadPixel(pSurface, x - offset_xS1, y - offset_yS1);
			SDL_GetRGBA(pixelS1, formatS1, &r, &g, &b, &a);
			//Acquisition pixel surface 2
			pixelS2 = ReadPixel(pSurface2, x - offset_xS2, y - offset_yS2);
			SDL_GetRGBA(pixelS2, formatS2, &r2, &g2, &b2, &a2);
			//Détermination de la collision
			if (a != 255 || a2 != 255)
			{
				collision = 0;
			}
			else
			{
				collision = 1;
			}
		}
	}
	formatS1 = NULL;
	formatS2 = NULL;
	return collision;
}


int gestionPhysique(Terrain* map, Input* pInput, ...)
{
	const double g = 9.81; //9.81 m/s ==> 37081.8 pix/s = 37 pix/ms
	int xRel = 0, yRel = 0;
	static enum DIRECTION dir = DOWN;
	char retournement = 0;
	static int  t = 0;
	static int stop = 0;
	Worms* worms = NULL;
	//Arme* weapon = NULL;
	va_list list;

	va_start(list, pInput);
	switch (va_arg(list, int))
	{
	case 0:
		worms = va_arg(list, Worms*);
		if (pInput->jumpOnGoing)
		{
			//On remet à zero x et y par rapport à sa position absolu de départ
			worms->wormsSurface->clip_rect.x = worms->xAbs;
			worms->wormsSurface->clip_rect.y = worms->yAbs;
			stop = 0;
		}
		if (!pInput->jumpOnGoing)
			dir = DOWN;
		else if (pInput->direction == UP)
			dir = UP;
		else
			dir = worms->dir;
		/*Réalisation du saut*/

		//On calcul les écarts relatifs sur x et y
		xRel = (int)(worms->vitx *t);
		yRel = (int)((worms->vity*t) - ((g*t*t) / 2000));
		//On calcule maintenant les valeurs de x et y
		worms->wormsSurface->clip_rect.x += xRel;
		worms->wormsSurface->clip_rect.y -= yRel;
		t += 7;
		//détection de possibles dépassement de la map
		//replaceWorms(worms, map->imageMapSurface);
		//detection du sens du worms
		retournement = retournementWorms(pInput, worms);
		//Fonction de déplacement du worms
		deplacementWorms(pInput, worms, map->imageMapSurface, &dir);
		//Si on a eu une collision (donc on est en fin de saut) on réattribut les nouvelles coordonnées absolues
		if (gestionCollision(pInput->acceleration, worms->wormsSurface, map->imageMapSurface, &dir, retournement))
		{
			worms->xAbs = worms->wormsSurface->clip_rect.x;
			worms->yAbs = worms->wormsSurface->clip_rect.y;
			t = 0;
			stop = 1;
 			pInput->jump = pInput->jumpOnGoing = 0;
			worms->vitx = 0;
			worms->vity = 0; 
		}
		break;
	case 1:
		//cas d'une arme
		break;
	}
	va_end(list);
	return 0;
}


/**
* \fn int detectionCollisionSurfaceV2(SDL_Surface* pSurface, SDL_Surface* pSurface2, enum DIRECTION dir)
* \brief Detecte s'il y a collision entre deux surfaces selon le sens de déplacement du worms.
*
* \param[in] pSurface, pointeur vers la surface de la map
* \param[in] pSurface2, pointeur vers la surface en mouvement dans la map
* \param[in] dir, direction du deplacement du worms
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int detectionCollisionSurfaceV2(SDL_Surface* pSurface, SDL_Surface* pSurface2, enum DIRECTION *dir)
{
	//Variables d'acquisitions
	Uint32 pixelS1 = 0;
	Uint8 r = 0, g = 0, b = 0, a = 0;
	Uint32 pixelS2 = 0;
	Uint8 r2 = 0, g2 = 0, b2 = 0, a2 = 0;
	int offset_xS2 = pSurface2->clip_rect.x;
	int offset_yS2 = pSurface2->clip_rect.y;
	SDL_PixelFormat* formatS1 = pSurface->format;
	SDL_PixelFormat* formatS2 = pSurface2->format;
	//Variables de balayage
	int x = 0, y = 0;
	int xStart = pSurface2->clip_rect.x, xEnd = pSurface2->clip_rect.x + pSurface2->clip_rect.w, xInc = 1;
	int yStart = pSurface2->clip_rect.y, yEnd = pSurface2->clip_rect.y + pSurface2->clip_rect.h, yInc = 1;
	//Variable de collision
	int collision = 0;

	//Test des limites de la map et de la fenetre
	if (limitMap(pSurface->h, pSurface->w, pSurface2, dir))
		return 1;
	//Détermination de yStart, yEnd, yInc
	if (*dir == DOWN)
	{
		yStart = -(pSurface2->clip_rect.y + pSurface2->clip_rect.h) + 1;
		yEnd = -pSurface2->clip_rect.y + 1;
	}
	//Détermination de xStart, xEnd, xInc
	if (*dir == RIGHT)
	{
		xStart = -(pSurface2->clip_rect.x + pSurface2->clip_rect.w) + 1;
		xEnd = -pSurface2->clip_rect.x + 1;
	}

	//Calcul de la collision
	for (y = yStart; (y < yEnd) && (collision == 0); y += yInc)
	{
		for (x = xStart; (x < xEnd) && (collision == 0); x += xInc)
		{
			//Acquisition des pixels des surfaces 1 et 2
			pixelS1 = ReadPixel(pSurface, MY_ABS(x), MY_ABS(y));
			pixelS2 = ReadPixel(pSurface2, MY_ABS(x) - offset_xS2, MY_ABS(y) - offset_yS2);
			//Récupération des composantes colorimétriques
			SDL_GetRGBA(pixelS1, formatS1, &r, &g, &b, &a);
			SDL_GetRGBA(pixelS2, formatS2, &r2, &g2, &b2, &a2);
			//Détermination de la collision
			if (a != 255 || a2 != 255)
				collision = 0;
			else
			{
				collision = 1;
				*dir = calculDirection(MY_ABS(x) - offset_xS2, MY_ABS(y) - offset_yS2, *dir, pSurface2->w, pSurface2->h);
			}
		}
	}
	formatS1 = NULL;
	formatS2 = NULL;
	return collision;
}
/**
* \fn int limitMap(unsigned int mapHight, unsigned int mapWidth, SDL_Surface* pSurface, enum DIRECTION* dir)
* \brief Test les limites de la map avec l'objet en mouvement.
*
* \param[in] mapHight, hauteur de la map
* \param[in] mapWidth, largeur de la map
* \param[in] pSurface, surface de l'objet en mouvement
* \param[in] dir, direction du deplacement du worms, peut etre modifie
* \return 1 = depassement de la map, 0 = pas de depassement de la map
*/
int limitMap(int mapHight,int mapWidth, SDL_Surface* pSurface, enum DIRECTION* dir)
{
	int xSurface = pSurface->clip_rect.x;
	int ySurface = pSurface->clip_rect.y;
	if (xSurface < 0)
	{
		*dir = LEFT;
		pSurface->clip_rect.x = 0;
		return 1;
	}
	else if (xSurface + pSurface->w > mapWidth)
	{
		pSurface->clip_rect.x = mapWidth - pSurface->clip_rect.w;
		*dir = RIGHT;
		return 1;
	}
	if (ySurface < 0)
	{
		*dir = UP;
		pSurface->clip_rect.y = 0;
		return 1;
	}
	else if (ySurface + pSurface->h > mapHight)
	{
		pSurface->clip_rect.y = mapHight - pSurface->clip_rect.h;
		*dir = DOWN;
		return 1;
	}
	return 0;
}

/**
* \fn void gestionCollision(Input* pInput, Worms* worms, SDL_Surface* surfaceCollision, enum DIRECTION dir, int retournement)
* \brief replace le worms en cas de collision.
*
* \param[in] pInput, inputs
* \param[in] worms, le worms qu'on déplace
* \param[in] surfaceCollision, surface de la map
* \param[in] dir, direction du deplacement du worms
* \param[in] retournement, indicateur de retournement du worms
* \returns collision, indique s'il y a eu collision
*/
int gestionCollision(int vitesse, SDL_Surface* surfaceMotion, SDL_Surface* surfaceCollision, enum DIRECTION* dir, char retournement)
{
	int t = 0;
	int collision = 0;
	while (detectionCollisionSurfaceV2(surfaceCollision, surfaceMotion, dir) && t < surfaceMotion->w)
	{
		switch (*dir)
		{
		case RIGHT:
			if (retournement)
			{
				*dir = LEFT;
				retournement = 0;
			}
			else surfaceMotion->clip_rect.x -= vitesse;
			break;
		case LEFT:
			if (retournement)
			{
				*dir = RIGHT;
				retournement = 0;
			}
			else surfaceMotion->clip_rect.x += vitesse;
			break;
		case DOWN:
			surfaceMotion->clip_rect.y -= vitesse;
			break;
		case UP:
			surfaceMotion->clip_rect.y += vitesse;
			break;
		default:
			break;
		}
		t++;
		collision = 1;
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
	if ((impulse == RIGHT && x < (w / 2)))
	{
		return LEFT;
	}
	if (impulse == LEFT && x >(w / 2))
	{
		return RIGHT;
	}
	if (y >= (5 * h / 8))
	{
		return DOWN;
	}
	else if (y > (h / 8) && y < (6 * h / 8))
	{
		return impulse; //retourne soit RIGHT si impulse est droite soit LEFT si impulse est gauche
	}
	return impulse;
}