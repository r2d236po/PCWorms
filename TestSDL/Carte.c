#include "carte.h"
#include "AffichageGeneral.h"


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Gestion du terrain                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

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








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                Gestion de la physique                  /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int gestionPhysique(Terrain* map, SDL_Texture* display, Input* pInput, ...)
{
	const double g = 9.81; //9.81 m/s ==> 37081.8 pix/s = 37 pix/ms
	int xRel = 0, yRel = 0;
	static enum DIRECTION dir = DOWN;
	enum DIRECTION dir2 = NONE;
	static char retournement = 0;
	static int  t = 0;
	int collision = 0;
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
			if (worms->dir == UP)
				dir = UP;
			else if (dir != UP && !retournement)
				dir = worms->dir;
		}
		else if (!pInput->jumpOnGoing)
			dir = DOWN;
		/*Réalisation du saut*/

		//On calcul les écarts relatifs sur x et y
		xRel = (int)(worms->vitx *t);
		yRel = (int)((worms->vity*t) - ((g*t*t) / 2000));
		//On calcule maintenant les valeurs de x et y
		worms->wormsSurface->clip_rect.x += xRel;
		worms->wormsSurface->clip_rect.y -= yRel;
		t += 7;
		//Determination de la direction du saut
		dir = calculDirectionSaut(xRel, yRel, worms->dir, pInput);
		//detection du sens du worms
		if (!retournement)
			retournement = retournementWorms(pInput, worms);
		//Fonction de déplacement du worms si non saut
		if (!pInput->jumpOnGoing)
			deplacementWorms(pInput, worms, map->imageMapSurface, &dir);
		if (retournement && (dir != DOWN) && !pInput->jumpOnGoing)
		{
			switch (worms->dirSurface)
			{
			case RIGHT:
				dir2 = LEFT;
				break;
			case LEFT:
				dir2 = RIGHT;
				break;
			}
			collision = gestionCollision(pInput->acceleration, worms->wormsSurface, map->imageMapSurface, &dir2);
		}
		else collision = gestionCollision(pInput->acceleration, worms->wormsSurface, map->imageMapSurface, &dir);
		//Si on a eu une collision (donc on est en fin de saut) on réattribut les nouvelles coordonnées absolues
		if (collision)
		{
			if (worms->xAbs != worms->wormsSurface->clip_rect.x || dir == DOWN || dir == UP)
				worms->yAbs = worms->wormsSurface->clip_rect.y;
			worms->xAbs = worms->wormsSurface->clip_rect.x;
			t = 0;
			pInput->jump = 0;
			if (endJumpTest(map->imageMapSurface, worms->wormsSurface))
				pInput->jumpOnGoing = 0;
			worms->vitx = 0;
			worms->vity = 0;
			pInput->direction = NONE;
			worms->dir = DOWN;
			retournement = 0;
		}
		updateGlobaleTexture(map->imageMapSurface, worms->wormsSurface, display, &worms->wormsRect);
		break;
	case 1:
		//cas d'une arme
		break;
	}
	va_end(list);
	return 0;
}








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Fonctions diverses                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int limitMap(unsigned int mapHight, unsigned int mapWidth, SDL_Surface* pSurface, enum DIRECTION* dir)
* \brief Test les limites de la map avec l'objet en mouvement.
*
* \param[in] mapHight, hauteur de la map
* \param[in] mapWidth, largeur de la map
* \param[in] pSurface, surface de l'objet en mouvement
* \param[in] dir, direction du deplacement du worms, peut etre modifie par la fonction
* \return 1 = depassement de la map, 0 = pas de depassement de la map
*/
int limitMap(int mapHight, int mapWidth, SDL_Surface* pSurface, enum DIRECTION* dir)
{
	int xSurface = pSurface->clip_rect.x;
	int ySurface = pSurface->clip_rect.y;
	if (xSurface < 0)
	{
		pSurface->clip_rect.x = 0;
		if (dir != NULL)
			*dir = LEFT;
		return 1;
	}
	else if (xSurface + pSurface->w > mapWidth)
	{
		pSurface->clip_rect.x = mapWidth - pSurface->clip_rect.w;
		if (dir != NULL)
			*dir = RIGHT;
		return 1;
	}
	if (ySurface < 0)
	{
		pSurface->clip_rect.y = 0;
		if (dir != NULL)
			*dir = UP;
		return 1;
	}
	else if (ySurface + pSurface->h > mapHight)
	{
		pSurface->clip_rect.y = mapHight - pSurface->clip_rect.h + 1;
		if (dir != NULL)
			*dir = DOWN;
		return 1;
	}
	return 0;
}


/**
* \fn enum DIRECTION calculDirection(int x, int y, enum DIRECTION impulse, int w, int h)
*
* \brief Determine la direction de la collision.
*
* \param[in] y, ordonnée de la collision
* \param[in] x, abscisse de la collision
* \param[in] impulse, direction du déplacement
* \param[in] w, largeur de l'objet
* \param[in] h, hauteur de l'objet
* \return DIRECTION, direction de la collision
*/

enum DIRECTION calculDirection(int x, int y, enum DIRECTION impulse, int w, int h)
{

	if (x >= 7 * w / 8 && impulse != DOWN)
	{
		if (y <= 2 * h / 8 && impulse != RIGHT)
		{
			return UPRIGHT;
		}
		else if (y >= 6 * h / 8)
		{
			return DRIGHT;
		}
		else return RIGHT;
	}
	else if (x <= 1 * w / 8 && impulse != DOWN)
	{
		if (y <= 2 * h / 8)
		{
			return UPLEFT;
		}
		else if (y >= 6 * h / 8)
		{
			return DLEFT;
		}
		else return LEFT;
	}
	else if (y <= 3 * h / 8)
		return UP;
	else if (y >= 7 * h / 8)
		return DOWN;
	else return impulse;
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
enum DIRECTION calculDirectionSaut(int xRel, int yRel, enum DIRECTION sensObjet, Input* pInput)
{
	if (pInput->jumpOnGoing)
	{
		switch (sensObjet)
		{
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
		return DOWN;
	return sensObjet;
}


/**
* \fn int endJumpTest(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
*
* \brief Deteremine la fin d'un saut.
*
* \param[in] pSurfaceMap, surface de la map
* \param[in] pSurfaceMotion, surface en mouvement dans la map
* \return int, 1 = fin de saut, 0 = saut non fini
*/
int endJumpTest(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
{
	enum DIRECTION dir = DOWN;
	pSurfaceMotion->clip_rect.y += 1;
	if (pSurfaceMotion->clip_rect.y + pSurfaceMotion->h > pSurfaceMap->h)
	{
		pSurfaceMotion->clip_rect.y -= 1;
		return 1;
	}
	if (detectionCollisionSurfaceV2(pSurfaceMap, pSurfaceMotion, &dir))
	{
		pSurfaceMotion->clip_rect.y -= 1;
		return 1;
	}
	return 0;
}








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////               Manipulation des pixels                  /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

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








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////         Fonctions de détection de collision            /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurfaceMap, int* xE, int* yE, SDL_Rect* pRect)
* \brief Detecte s'il y a collision entre un rectangle et une surface.
*
* \param[in] pRenderer, pointeur vers le renderer de la fenetre
* \param[in] pSurfaceMap, pointeur vers la surface de la surface a tester
* \param[in] xE, pointeur vers la variable contenant l'abscisse de la collision
* \param[in] yE, pointeur vers la variable contenant l'ordonne de la collision
* \param[in] pRect, pointeur vers le rectangle a tester
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurfaceMap, int* xE, int* yE, SDL_Rect* pRect)
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	Uint32 p = ReadPixel(pSurfaceMap, 0, 0);
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
				p = ReadPixel(pSurfaceMap, x, y);
				SDL_GetRGBA(p, pSurfaceMap->format, &r, &g, &b, &a);
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
			p = ReadPixel(pSurfaceMap, rect.x, y);
			SDL_GetRGBA(p, pSurfaceMap->format, &r, &g, &b, &a);
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
			p = ReadPixel(pSurfaceMap, rect.x + rect.w, y);
			SDL_GetRGBA(p, pSurfaceMap->format, &r, &g, &b, &a);
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
* \fn int detectionCollisionSurface(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
* \brief Detecte s'il y a collision entre deux surfaces.
*
* \param[in] pSurfaceMap, pointeur vers la surface de la map
* \param[in] pSurfaceMotion, pointeur vers la surface en mouvement dans la map
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int detectionCollisionSurface(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
{
	//Variables d'acquisitions
	Uint32 pixelS1 = 0;
	Uint8 r = 0, g = 0, b = 0, a = 0;
	Uint32 pixelS2 = 0;
	Uint8 r2 = 0, g2 = 0, b2 = 0, a2 = 0;
	int offset_xS1 = pSurfaceMap->clip_rect.x;
	int offset_yS1 = pSurfaceMap->clip_rect.y;
	int offset_xS2 = pSurfaceMotion->clip_rect.x;
	int offset_yS2 = pSurfaceMotion->clip_rect.y;
	SDL_PixelFormat* formatS1 = pSurfaceMap->format;
	SDL_PixelFormat* formatS2 = pSurfaceMotion->format;
	//Variables de balayage
	int x = 0, y = 0;
	//Varable de collision
	int collision = 0;
	//Test des limites de la map et de la fenetre
	if (limitMap(pSurfaceMap->h, pSurfaceMap->w, pSurfaceMotion, NULL))
		return 1;

	for (y = pSurfaceMotion->clip_rect.y; (y < pSurfaceMotion->clip_rect.h + pSurfaceMotion->clip_rect.y) && (collision == 0); y++)
	{
		for (x = pSurfaceMotion->clip_rect.x; (x < pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w) && (collision == 0); x++)
		{
			//Acquisition pixel surface 1
			pixelS1 = ReadPixel(pSurfaceMap, x - offset_xS1, y - offset_yS1);
			SDL_GetRGBA(pixelS1, formatS1, &r, &g, &b, &a);
			//Acquisition pixel surface 2
			pixelS2 = ReadPixel(pSurfaceMotion, x - offset_xS2, y - offset_yS2);
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


/**
* \fn int detectionCollisionSurfaceV2(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION dir)
* \brief Detecte s'il y a collision entre deux surfaces selon le sens de déplacement du worms.
*
* \param[in] pSurfaceMap, pointeur vers la surface de la map
* \param[in] pSurfaceMotion, pointeur vers la surface en mouvement dans la map
* \param[in] dir, direction du deplacement du worms
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int detectionCollisionSurfaceV2(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION *dir)
{
	//Variables d'acquisitions
	Uint32 pixelS1 = 0;
	Uint8 r = 0, g = 0, b = 0, a = 0;
	Uint32 pixelS2 = 0;
	Uint8 r2 = 0, g2 = 0, b2 = 0, a2 = 0;
	int offset_xS2 = pSurfaceMotion->clip_rect.x;
	int offset_yS2 = pSurfaceMotion->clip_rect.y;
	SDL_PixelFormat* formatS1 = pSurfaceMap->format;
	SDL_PixelFormat* formatS2 = pSurfaceMotion->format;
	//Variables de balayage
	int x = 0, y = 0;
	int xStart = pSurfaceMotion->clip_rect.x, xEnd = pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w, xInc = 1;
	int yStart = pSurfaceMotion->clip_rect.y, yEnd = pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h, yInc = 1;
	//Variable de collision
	int collision = 0;
	//Test des limites de la map et de la fenetre
	if (limitMap(pSurfaceMap->h, pSurfaceMap->w, pSurfaceMotion, dir))
		return 1;
	//Détermination de yStart, yEnd, yInc
	if (*dir == DOWN)
	{
		yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;
		yEnd = -pSurfaceMotion->clip_rect.y + 1;
	}
	//Détermination de xStart, xEnd, xInc
	if (*dir == RIGHT)
	{
		xStart = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w) + 1;
		xEnd = -pSurfaceMotion->clip_rect.x + 1;
	}
	if (*dir == DRIGHT)
	{
		yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;
		yEnd = -pSurfaceMotion->clip_rect.y + 1;
		yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;
		yEnd = -pSurfaceMotion->clip_rect.y + 1;
	}
	//Calcul de la collision
	for (y = yStart; (y < yEnd) && (collision == 0); y += yInc)
	{
		for (x = xStart; (x < xEnd) && (collision == 0); x += xInc)
		{
			//Acquisition des pixels des surfaces 1 et 2
			pixelS1 = ReadPixel(pSurfaceMap, MY_ABS(x), MY_ABS(y));
			pixelS2 = ReadPixel(pSurfaceMotion, MY_ABS(x) - offset_xS2, MY_ABS(y) - offset_yS2);
			//Récupération des composantes colorimétriques
			SDL_GetRGBA(pixelS1, formatS1, &r, &g, &b, &a);
			SDL_GetRGBA(pixelS2, formatS2, &r2, &g2, &b2, &a2);
			//Détermination de la collision
			if (a != 255 || a2 != 255)
				collision = 0;
			else
			{
				collision = 1;
				*dir = calculDirection(MY_ABS(x) - offset_xS2, MY_ABS(y) - offset_yS2, *dir, pSurfaceMotion->w, pSurfaceMotion->h);
			}
		}
	}
	formatS1 = NULL;
	formatS2 = NULL;
	return collision;
}


/**
* \fn void gestionCollision(Input* pInput, Worms* worms, SDL_Surface* surfaceCollision, enum DIRECTION dir, int retournement)
* \brief replace le worms en cas de collision.
*
* \param[in] vitesse, vitesse de deplacement du worms, sert pour le replacement
* \param[in] surfaceMotion, surface en deplacement
* \param[in] surfaceMap, surface de la map
* \param[in] dir, direction du deplacement du worms, peut etre modifie par la fonction
* \returns int collision, indique s'il y a eu collision
*/
int gestionCollision(int vitesse, SDL_Surface* surfaceMotion, SDL_Surface* surfaceMap, enum DIRECTION* dir)
{
	int t = 0;
	int collision = 0;
	while (detectionCollisionSurfaceV2(surfaceMap, surfaceMotion, dir) && t < surfaceMotion->w)
	{
		switch (*dir)
		{
		case RIGHT:
			surfaceMotion->clip_rect.x -= vitesse;
			break;
		case LEFT:
			surfaceMotion->clip_rect.x += vitesse;
			break;
		case DOWN:
			surfaceMotion->clip_rect.y -= vitesse;
			break;
		case UP:
			surfaceMotion->clip_rect.y += vitesse;
			break;
		case UPRIGHT:
			surfaceMotion->clip_rect.x -= vitesse;
			surfaceMotion->clip_rect.y += vitesse;
			break;
		case UPLEFT:
			surfaceMotion->clip_rect.x += vitesse;
			surfaceMotion->clip_rect.y += vitesse;
			break;
		case DRIGHT:
			surfaceMotion->clip_rect.x -= vitesse;
			surfaceMotion->clip_rect.y -= vitesse;
			break;
		case DLEFT:
			surfaceMotion->clip_rect.x += vitesse;
			surfaceMotion->clip_rect.y -= vitesse;
			break;
		default:
			break;
		}
		t++;
		collision = 1;
	}
	return collision;
}