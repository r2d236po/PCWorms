#include "carte.h"
#include "AffichageGeneral.h"
#include "worms.h"


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Gestion du terrain                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int initialisionTerrain(Terrain** p_pMapTerrain, SDL_Renderer * pRenderer, const char * nomImageFond, const char * nomImageMap)
* \brief Initialise un terrain avec son image de fond et l'image de la map.
*
* \param[in] p_pMapTerrain, adresse du pointeur de structure Terrain a initialiser
* \param[in] pRenderer, pointeur du renderer dans lequel le Terrain est a initialiser
* \param[in] nomImageFond, nom du fichier de l'image de fond
* \param[in] nomImageMap, nom du fichier de l'image de la map
* \returns int, Indicateur de reussite de la fonction : 1 = succes, -1 = echec
*/
int initialisionTerrain(Terrain** p_pMapTerrain, SDL_Renderer * pRenderer, const char * nomImageFond, const char * nomImageMap)
{
	Terrain * pMapTerrainTemp = NULL;	//Pointeur du Terrain temporaire

	//Creation du pointeur de Terrain
	pMapTerrainTemp = (Terrain*)malloc(sizeof(Terrain));	//Allocation mémoire pour le pointeur de Terrain
	if (pMapTerrainTemp == NULL)	//Si l'allocation s'est mal passee
	{
		printf("Probleme de lors de l'allocation memoire du terrain");
		return -1;	//Retour d'erreur
	}
	pMapTerrainTemp->imageBackground = NULL;	//Initialisation à NULL du pointeur de la texture de l'image de fond
	pMapTerrainTemp->imageMapSurface = NULL;	//Initialisation à NULL du pointeur de la surface de l'image de la map

	//Creation de la texture de l'image de fond
	pMapTerrainTemp->imageBackground = loadTexture(pRenderer, nomImageFond);	//Chargement de l'image de fond dans la texture
	if (pMapTerrainTemp->imageBackground == NULL)	//Si le chargement a échoué
	{
		printf("Probleme de lors de la creation de la texture Background");
		destroyMap(&pMapTerrainTemp);	//Destruction du Terrain
		return -1;	//Retour d'erreur
	}

	//Creation de la surface de la map qui servira pour la collision
	pMapTerrainTemp->imageMapSurface = loadImage(nomImageMap);	//Chargement de l'image de la map dans la surface
	if (pMapTerrainTemp->imageMapSurface == NULL) //Verification qu'il n'y a pas eu d'erreur lors de l'allocation en mémoire
	{
		printf("Probleme de lors de la creation de la surface de la map");
		destroyMap(&pMapTerrainTemp);	//Destruction du Terrain
		return -1;	//Retour d'erreur
	}
	(*p_pMapTerrain) = pMapTerrainTemp; //Récupération du pointeur du Terrain
	pMapTerrainTemp = NULL;	//Remise à NULL du pointeur temporaire
	return 1;	//Retour sans erreur
}

/**
* \fn void destroyMap(Terrain** p_pMapTerrain)
* \brief Détruit une structure Terrain et remet son pointeur à NULL.
*
* \param[in] p_pMapTerrain, adresse du pointeur de la structure du Terrain à détruire.
*/
void destroyMap(Terrain** p_pMapTerrain)
{
	if ((*p_pMapTerrain)->imageBackground != NULL)	//Si le pointeur de la texture de l'image de fond n'est pas NULL
	{
		SDL_DestroyTexture((*p_pMapTerrain)->imageBackground);	//Destruction de la texture de l'image de fond
		(*p_pMapTerrain)->imageBackground = NULL;	//Remise à NULL du pointeur
	}
	if ((*p_pMapTerrain)->imageMapSurface != NULL)	//Si le pointeur de la surface de l'image de la map n'est pas NULL
	{
		SDL_FreeSurface((*p_pMapTerrain)->imageMapSurface);	//Destruction de la surface de l'image de la map
		(*p_pMapTerrain)->imageMapSurface = NULL;	//Remise à NULL du pointeur
	}
	free(*p_pMapTerrain);	//Liberation de la mémoire du pointeur du Terrain
	*p_pMapTerrain = NULL;	//Remise à NULL du pointeur
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
* \param[in] pSurfaceMotion, surface de l'objet en mouvement
* \param[in] pDirecion, pointeur de la direction du deplacement du worms
* \return 1 = depassement de la map, 0 = pas de depassement de la map
*/
int limitMap(int mapHight, int mapWidth, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirecion)
{
	int xSurface = pSurfaceMotion->clip_rect.x;
	int ySurface = pSurfaceMotion->clip_rect.y;
	if (xSurface < 0)
	{
		pSurfaceMotion->clip_rect.x = 0;
		if (pDirecion != NULL)
			*pDirecion = LEFT;
		return 1;
	}
	else if (xSurface + pSurfaceMotion->w > mapWidth)
	{
		pSurfaceMotion->clip_rect.x = mapWidth - pSurfaceMotion->clip_rect.w;
		if (pDirecion != NULL)
			*pDirecion = RIGHT;
		return 1;
	}
	if (ySurface < 0)
	{
		pSurfaceMotion->clip_rect.y = 0;
		if (pDirecion != NULL)
			*pDirecion = UP;
		return 1;
	}
	else if (ySurface + pSurfaceMotion->h > mapHight)
	{
		pSurfaceMotion->clip_rect.y = mapHight - pSurfaceMotion->clip_rect.h + 1;
		if (pDirecion != NULL)
			*pDirecion = DOWN;
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
* \param[in] retournement, variable indiquant s'il y a eu retournement du worms
* \return DIRECTION, direction de la collision
*/

enum DIRECTION calculDirectionCollision(int x, int y, enum DIRECTION impulse, int w, int h, int retournement)
{
	if ((impulse == UPRIGHT || impulse == UPLEFT) && !retournement)
	{
		if ((x < 2 * w / 10) || (x >= 8 * w / 10))
			return impulse;
		if (y > 2 * h / 3)
			return DOWN;
		else return UP;
	}
	else if (impulse == RIGHT || impulse == LEFT)
	{
		if (!retournement)
		{
			if (y <= (7 * h / 8))
				return impulse;
			else return DOWN;
		}
		else
		{
			if (x > (w / 2))
				return RIGHT;
			else return  LEFT;
		}
	}
	else if ((impulse == DRIGHT || impulse == DLEFT))
	{
		if (x >= (3 * w / 11) && x <= (8 * w / 11) && (y > h / 2))
			return DOWN;
		else if (x < w / 2)
			return DLEFT;
		else if (x > w / 2)
			return DRIGHT;
		else return impulse;
	}
	else if (impulse == UP)
	{
		if ((x < w / 2) && (y > 5 * h / 8))
			return DLEFT;
		else return impulse;
	}
	else return impulse;
}








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////               Manipulation des pixels                  /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn Uint32 ReadPixel(SDL_Surface* pSurface, int x, int y)
* \brief Lit un pixel aux coordonnees x et y d'une surface.
*
* \param[in] pSurface, surface dans laquelle on lit le pixel
* \param[in] x, abscisse dans la surface
* \param[in] y, ordonne dans la surface
* \returns Uint32, pixel lu aux coordonnees contenant les 4 canaux (RGBA)
* \remarks Faire attention que x et y soient bien compris dans la surface.
*/

Uint32 ReadPixel(SDL_Surface* pSurface, int x, int y)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)pSurface->pixels;
	//Get the requested pixel
	return pixels[(y * pSurface->w) + x];
}


/**
* \fn void DrawPixel(SDL_Surface* pSurface, int x, int y, Uint32 pixelToWrite)
* \brief Ecrit un pixel aux coordonnees x et y d'une surface.
*
* \param[in] pSurface, surface dans laquelle on ecrit le pixel
* \param[in] x, abscisse dans la surface
* \param[in] y, ordonne dans la surface
* \param[in] pixelToWrite, pixel a ecrire
*/
void DrawPixel(SDL_Surface* pSurface, int x, int y, Uint32 pixelToWrite)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)pSurface->pixels;

	//Set the pixel
	if ((x >= pSurface->clip_rect.x) && (x <= (pSurface->clip_rect.x + pSurface->w)) && (y >= pSurface->clip_rect.y) && (y <= (pSurface->clip_rect.y + pSurface->h)))
		pixels[(y * pSurface->w) + x] = pixelToWrite;
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
	Uint8 rS1 = 0, gS1 = 0, bS1 = 0, aS1 = 0;
	Uint32 pixelS2 = 0;
	Uint8 rS2 = 0, gS2 = 0, bS2 = 0, aS2 = 0;
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
			SDL_GetRGBA(pixelS1, formatS1, &rS1, &gS1, &bS1, &aS1);
			//Acquisition pixel surface 2
			pixelS2 = ReadPixel(pSurfaceMotion, x - offset_xS2, y - offset_yS2);
			SDL_GetRGBA(pixelS2, formatS2, &rS2, &gS2, &bS2, &aS2);
			//Détermination de la collision
			if (aS1 != 255 || aS2 != 255)	//Si le pixel de la surface de la map ou le pixel de la surface en mouvement est transparent
				collision = 0;	//Collision = 0 -> pas de collision
			else	//Au moins l'un des pixels n'est pas transparent
			{
				collision = 1;	//Collision = 0 -> pas de collision
			}
		}
	}
	formatS1 = NULL;	//Remise à 0 des pointeurs de format
	formatS2 = NULL;	//Remise à 0 des pointeurs de format
	return collision;
}


/**
* \fn int detectionCollisionSurfaceV2(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection, int retournement)
* \brief Detecte s'il y a collision entre deux surfaces selon le sens de déplacement du worms.
*
* \param[in] pSurfaceMap, pointeur vers la surface de la map
* \param[in] pSurfaceMotion, pointeur vers la surface en mouvement dans la map
* \param[in] pDirection, pointeur de la direction du deplacement du worms
* \param[in] retournement, variable indiquant s'il y a eu retournement du worms
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int detectionCollisionSurfaceV2(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection, int retournement)
{
	//Variables d'acquisitions
	Uint32 pixelS1 = 0;
	Uint8 rS1 = 0, gS1 = 0, bS1 = 0, aS1 = 0;
	Uint32 pixelS2 = 0;
	Uint8 rS2 = 0, gS2 = 0, bS2 = 0, aS2 = 0;
	int offset_xS2 = pSurfaceMotion->clip_rect.x;
	int offset_yS2 = pSurfaceMotion->clip_rect.y;
	SDL_PixelFormat* formatS1 = pSurfaceMap->format;
	SDL_PixelFormat* formatS2 = pSurfaceMotion->format;
	//Variables de balayage
	int x = 0, y = 0;
	int xStart = pSurfaceMotion->clip_rect.x, xEnd = pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w, xInc = 1;	//Variables de balayage des x
	int yStart = pSurfaceMotion->clip_rect.y, yEnd = pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h, yInc = 1;	//Variables de balayage des y
	//Variable de collision
	int collision = 0;	//Booleen de collision, 0 = pas de collision, 1 = collision
	//Test des limites de la map et de la fenetre
	if (limitMap(pSurfaceMap->h, pSurfaceMap->w, pSurfaceMotion, pDirection))	//Detection d'un dépassement de la map
		return 1;
	//Détermination de yStart, yEnd, xStart et xEnd
	calculXYBalayage(pSurfaceMotion, *pDirection, &xStart, &xEnd, &yStart, &yEnd);	//Calcul des valeurs de balayage des boucles for pour optimiser la vitesse de traitement
	//Calcul de la collision
	for (y = yStart; (y < yEnd) && (collision == 0); y += yInc)
	{
		for (x = xStart; (x < xEnd) && (collision == 0); x += xInc)
		{
			//Acquisition des pixels des surfaces 1 et 2
			pixelS1 = ReadPixel(pSurfaceMap, MY_ABS(x), MY_ABS(y));	//Lecture du pixel de la map
			pixelS2 = ReadPixel(pSurfaceMotion, MY_ABS(x) - offset_xS2, MY_ABS(y) - offset_yS2);	//Lecture du pixel de la surface en mouvement
			//Récupération des composantes colorimétriques
			SDL_GetRGBA(pixelS1, formatS1, &rS1, &gS1, &bS1, &aS1);	//Informations sur les couleurs du pixel de la surface de la map
			SDL_GetRGBA(pixelS2, formatS2, &rS2, &gS2, &bS2, &aS2);	//Informations sur les couleurs du pixel de la surface en mouvement
			//Détermination de la collision
			if (aS1 != 255 || aS2 != 255)	//Si le pixel de la surface de la map ou le pixel de la surface en mouvement est transparent
				collision = 0;	//Collision = 0 -> pas de collision
			else	//Au moins l'un des pixels n'est pas transparent
			{
				collision = 1;	//Collision = 0 -> pas de collision
				*pDirection = calculDirectionCollision(MY_ABS(x) - offset_xS2, MY_ABS(y) - offset_yS2, *pDirection, pSurfaceMotion->w, pSurfaceMotion->h, retournement);	//Calcul de la direction de la collision pour affiner le traitement
			}
		}
	}
	formatS1 = NULL;	//Remise à 0 des pointeurs de format
	formatS2 = NULL;	//Remise à 0 des pointeurs de format
	return collision;
}

/**
* \fn void calculXYBalayage(SDL_Surface* pSurfaceMotion, enum DIRECTION dir, int* xStart, int* xEnd, int* yStart, int* yEnd)
* \brief Calcul les valeurs de debut et de fin pour X et Y afin d'optimiser la detection de collision.
*
* \param[in] psurfaceMotion, surface en deplacement
* \param[in] dir, direction du deplacement du worms
* \param[in] xStart, pointeur vers la variable xStart, debut du balayage en x
* \param[in] xEnd, pointeur vers la variable xEnd, fin du balayage en x
* \param[in] yStart, pointeur vers la variable yStart, debut du balayage en y
* \param[in] yEnd, pointeur vers la variable yEnd, fin du balayage en y
* \returns void
*/
void calculXYBalayage(SDL_Surface* pSurfaceMotion, enum DIRECTION dir, int* xStart, int* xEnd, int* yStart, int* yEnd)
{
	//Détermination de yStart, yEnd
	if (dir == DOWN)
	{
		*yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;
		*yEnd = -(2 * pSurfaceMotion->clip_rect.h / 3 + pSurfaceMotion->clip_rect.y) + 1;
	}
	//Détermination de xStart, xEnd
	else if (dir == UPRIGHT)
	{
		*xStart = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w) + 1;
		*xEnd = -(pSurfaceMotion->clip_rect.x) + 1;
		*yEnd = pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h;
	}
	else if (dir == UPLEFT)
	{
		*yEnd = pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h;
	}
	else if (dir == RIGHT)
	{
		*xStart = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w) + 1;
		*xEnd = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w / 2) + 1;
	}
	else if (dir == LEFT)
	{
		*xEnd = (pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w / 2);
	}
	else if (dir == DRIGHT)
	{
		*yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;
		*yEnd = -(pSurfaceMotion->clip_rect.y) + 1;
		*xStart = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w) + 1;
		*xEnd = -(pSurfaceMotion->clip_rect.x) + 1;
	}
	else if (dir == DLEFT)
	{
		*yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;
		*yEnd = -(pSurfaceMotion->clip_rect.y) + 1;
		*xEnd = (pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w);
	}
}


/**
* \fn int gestionCollision(int vitesse, SDL_Surface* surfaceMap, SDL_Surface* surfaceMotion, enum DIRECTION* pDirection, int retournement)
* \brief replace le worms en cas de collision.
*
* \param[in] vitesse, vitesse de deplacement du worms, sert pour le replacement
* \param[in] surfaceMotion, surface en deplacement
* \param[in] surfaceMap, surface de la map
* \param[in] pDirection, direction du deplacement du worms, peut etre modifie par la fonction
* \param[in] retournement, variable indiquant s'il y a eu retournement du worms
* \returns int collision, indique s'il y a eu collision
*/
int gestionCollision(int vitesse, SDL_Surface* surfaceMap, SDL_Surface* surfaceMotion, enum DIRECTION* pDirection, int retournement)
{
	int t = 0;
	int collision = 0;
	while (detectionCollisionSurfaceV2(surfaceMap, surfaceMotion, pDirection, retournement) && t < 60)
	{
		switch (*pDirection)
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