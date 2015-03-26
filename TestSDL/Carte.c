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
* \param[in] pDirecion, pointeur de la direction du deplacement du worms, NULL si non utile
* \return 1 = depassement de la map, 0 = pas de depassement de la map
*/
int limitMap(int mapHight, int mapWidth, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirecion)
{
	int xSurface = pSurfaceMotion->clip_rect.x;	//stockage de la valeur x de la surface en déplacement pour simplifier la lecture
	int ySurface = pSurfaceMotion->clip_rect.y;	//stockage de la valeur y de la surface en déplacement pour simplifier la lecture
	if (xSurface < 0)	//Si la surface sors de la map par la gauche
	{
		pSurfaceMotion->clip_rect.x = 0;	//Set de la valeur de x à 0 pour coller à la map
		if (pDirecion != NULL)	//Si le pointeur de direction n'est pas NULL
			*pDirecion = LEFT;	//Set de la direction à gauche
		return 1;	//return 1 = dépassement de la map, 0 = pas de dépassement
	}
	else if (xSurface + pSurfaceMotion->w > mapWidth)	//Si la surface sors de la map par la droite
	{
		pSurfaceMotion->clip_rect.x = mapWidth - pSurfaceMotion->clip_rect.w;	//Set de la valeur de x à la largeur de la map - la largeur de l'objet pour coller à la map
		if (pDirecion != NULL)	//Si le pointeur de direction n'est pas NULL
			*pDirecion = RIGHT;	//Set de la direction à droite
		return 1;	//return 1 = dépassement de la map, 0 = pas de dépassement
	}
	if (ySurface < 0)	//Si la surface sors de la map par le dessus
	{
		pSurfaceMotion->clip_rect.y = 0;	//Set de la valeur de y pour coller au haut de la map
		if (pDirecion != NULL)	//Si le pointeur de direction n'est pas NULL
			*pDirecion = UP;	//Set de la direction à haut
		return 1;	//return 1 = dépassement de la map, 0 = pas de dépassement
	}
	else if (ySurface + pSurfaceMotion->h > mapHight)	//Si la surface sors de la map par le dessous
	{
		pSurfaceMotion->clip_rect.y = mapHight - pSurfaceMotion->clip_rect.h + 1;	//Set de la valeur de y à hauteur de la map - hauteur de l'objet pour coller à la map
		if (pDirecion != NULL)	//Si le pointeur de direction n'est pas NULL
			*pDirecion = DOWN;	//Set de la direction à bas
		return 1;	//return 1 = dépassement de la map, 0 = pas de dépassement
	}
	return 0;	//return 1 = dépassement de la map, 0 = pas de dépassement
}


/**
* \fn enum DIRECTION calculDirection(int x, int y, enum DIRECTION direction, int w, int h)
*
* \brief Determine la direction de la collision.
*
* \param[in] y, ordonnée de la collision
* \param[in] x, abscisse de la collision
* \param[in] direction, direction du déplacement
* \param[in] w, largeur de l'objet
* \param[in] h, hauteur de l'objet
* \param[in] retournement, variable indiquant s'il y a eu retournement du worms
* \return DIRECTION, direction de la collision
*/

enum DIRECTION calculDirectionCollision(int x, int y, enum DIRECTION direction, int w, int h, int retournement)
{
	if ((direction == UPRIGHT || direction == UPLEFT) && !retournement)	//Si on se on dirige en diagonale (pendant un saut ou un lancer)
	{
		if ((x < 2 * w / 10) || (x >= 8 * w / 10))	//Si le point d'impact en x se trouve sur les bords
			return direction;	//On retourne la direction du déplacement 
		if (y > 2 * h / 3)	//Si la valeur de y dépasse un certain seuil (ici 1 /3 de la taille de l'objet)
			return DOWN;	//On retourne DOWN
		else return UP;	//Sinon si le point d'impact est en haut et au centre on retourne UP
	}
	else if (direction == RIGHT || direction == LEFT)	//Si l'objet se déplace latéralement
	{
		if (!retournement)	//Si on est pas en train de se retourner
		{
			if (y <= (7 * h / 8))	//Si y est au dessus d'un certain seuil (si on est pas au sol)
				return direction;	//On retourne la direction de déplacement
			else return DOWN;	//Sinon si on a un contact au sol on retourne DOWN (ça évite une téléportation en arrière)
		}
		else //Sinon si on s'est retourné
		{
			if (x > (w / 2))	//Si x au dessus de la moitié horizontale de l'objet
				return RIGHT;	//On retourne RIGHT
			else return  LEFT;	//Sinon on retourne LEFT
		}
	}
	else if ((direction == DRIGHT || direction == DLEFT))	//Si on se déplace en diagonale descendante (pendant la fin d'un saut ou en fin de lancer d'une grenade)
	{
		if (x >= (3 * w / 11) && x <= (8 * w / 11) && (y > h / 2))	//Si x est au "centre" de l'objet et que y est dans la moitié basse de celui-ci
			return DOWN;	//On retourne DOWN
		/*else if (x < w / 2)
			return DLEFT;
		else if (x > w / 2)
			return DRIGHT;*/
		else return direction;	//Sinon on retourne la direction originale
	}
	else if (direction == UP)
	{
		if ((x < w / 2) && (y > 5 * h / 8))
			return DLEFT;
		else if ((x > w / 2) && (y > 5 * h / 8))
			return DRIGHT;
		else return direction;
	}
	else return direction;	//Sinon on retourne la direction de départ
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
	Uint32 *pixels = (Uint32 *)pSurface->pixels;	//Récupère le tableau de pixels de la surface
	//Get the requested pixel
	return pixels[(y * pSurface->w) + x];	//Return le pixel aux coordonnées x et y passées en paramètre
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
	Uint32 *pixels = (Uint32 *)pSurface->pixels;	//Récupère le tableau de pixel de la surface

	//Set the pixel
	if ((x >= pSurface->clip_rect.x) && (x <= (pSurface->clip_rect.x + pSurface->w)) && (y >= pSurface->clip_rect.y) && (y <= (pSurface->clip_rect.y + pSurface->h)))	//Test que le pixel est bien dans la surface
		pixels[(y * pSurface->w) + x] = pixelToWrite;	//Ecrit le pixel aux coordonnées passées en paramètre
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
	Uint32 pixelS1 = 0;	//Variable stockant le pixel en cours de lecture de la surface de la map
	Uint8 rS1 = 0, gS1 = 0, bS1 = 0, aS1 = 0;	//Variables stockant les informations sur les couleurs du pixel lu de la surface de la map
	Uint32 pixelS2 = 0;	//Variable stockant le pixel en cours de lecture de la surface en mouvement
	Uint8 rS2 = 0, gS2 = 0, bS2 = 0, aS2 = 0;	//Variables stockant les informations sur les couleurs du pixel lu de la surface en mouvement
	int offset_xS2 = pSurfaceMotion->clip_rect.x;	//Offset en x de la surface en mouvement dans la map
	int offset_yS2 = pSurfaceMotion->clip_rect.y;	//Offset en y de la surface en mouvement dans la map
	SDL_PixelFormat* formatS1 = pSurfaceMap->format;	//Pointeur du format de pixels de la surface de la map
	SDL_PixelFormat* formatS2 = pSurfaceMotion->format;	//Pointeur du format de pixels de la surface en mouvement
	//Variables de balayage
	int x = 0, y = 0;
	int xStart = pSurfaceMotion->clip_rect.x, xEnd = pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w;	//Variables de début, de fin et d'incrément du balayage des x
	int yStart = pSurfaceMotion->clip_rect.y, yEnd = pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h;	//Variables de début, de fin et d'incrément du balayage des y
	//Varable de collision
	int collision = 0;	//Booleen de collision, 0 = pas de collision, 1 = collision
	//Test des limites de la map et de la fenetre
	if (limitMap(pSurfaceMap->h, pSurfaceMap->w, pSurfaceMotion, NULL))	//Detection d'un dépassement de la map
		return 1;

	for (y = yStart; (y < yEnd) && (collision == 0); y++)
	{
		for (x = xStart; (x < xEnd) && (collision == 0); x++)
		{
			//Acquisition pixel surface 1
			pixelS1 = ReadPixel(pSurfaceMap, x, y);	//Lecture du pixel de la map
			SDL_GetRGBA(pixelS1, formatS1, &rS1, &gS1, &bS1, &aS1);	//Informations sur les couleurs du pixel de la surface de la map
			//Acquisition pixel surface 2
			pixelS2 = ReadPixel(pSurfaceMotion, x - offset_xS2, y - offset_yS2);	//Lecture du pixel de la surface en mouvement
			SDL_GetRGBA(pixelS2, formatS2, &rS2, &gS2, &bS2, &aS2);	//Informations sur les couleurs du pixel de la surface en mouvement
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
* \param[in] direction, direction du deplacement du worms
* \param[in] xStart, pointeur vers la variable xStart, debut du balayage en x
* \param[in] xEnd, pointeur vers la variable xEnd, fin du balayage en x
* \param[in] yStart, pointeur vers la variable yStart, debut du balayage en y
* \param[in] yEnd, pointeur vers la variable yEnd, fin du balayage en y
* \returns void
*/
void calculXYBalayage(SDL_Surface* pSurfaceMotion, enum DIRECTION direction, int* xStart, int* xEnd, int* yStart, int* yEnd)
{
	//Détermination de yStart, yEnd
	if (direction == DOWN)	//Si la surface se dirige vers le bas
	{
		*yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;	//On balaye en partant du bas, on prends la valeur négative pour conserver une incrémentation positive et une boucle identique
		*yEnd = -(2 * pSurfaceMotion->clip_rect.h / 3 + pSurfaceMotion->clip_rect.y) + 1;	//On arrête le balayage au tiers haut, pour une detection vers le bas, c'est suffisant
	}
	//Détermination de xStart, xEnd
	else if (direction == UPRIGHT)	//Si la surface se dirige en diagonale haute droite
	{
		*xStart = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w) + 1;
		*xEnd = -(pSurfaceMotion->clip_rect.x) + 1;
		*yEnd = pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h;
	}
	else if (direction == UPLEFT)	//Si la surface se dirige en diagonale haute gauche
	{
		*yEnd = pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h;
	}
	else if (direction == RIGHT)	//Si la surface se dirige vers la droite
	{
		*xStart = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w) + 1;	//On démarre le balayage à droite
		*xEnd = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w / 2) + 1;	//On arrête le balayage à la moitié de la surface, pour une détection de côté, c'est suffisant
	}
	else if (direction == LEFT)	//Si la surface se dirige vers la gauche
	{
		*xEnd = (pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w / 2);	//On arrête le balayage à la moitié de la surface, pour une détection de côté, c'est suffisant
	}
	else if (direction == DRIGHT)	//Si la surface se dirige en diagonale basse droite
	{
		*yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;	//On démarre le balayage en bas
		*yEnd = -(2 * pSurfaceMotion->clip_rect.h / 3 + pSurfaceMotion->clip_rect.y) + 1;	//On s'arrête au tiers haut
		*xStart = -(pSurfaceMotion->clip_rect.x + pSurfaceMotion->clip_rect.w) + 1;	//On démarre le balayage à droite
		*xEnd = -(pSurfaceMotion->clip_rect.x) + 1;
	}
	else if (direction == DLEFT)	//Si la surface se dirige en diagonale basse gauche
	{
		*yStart = -(pSurfaceMotion->clip_rect.y + pSurfaceMotion->clip_rect.h) + 1;	//On démarre le balayage en bas
		*yEnd = -(2 * pSurfaceMotion->clip_rect.h / 3 + pSurfaceMotion->clip_rect.y) + 1;	//On s'arrête au tiers haut
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
	while (detectionCollisionSurfaceV2(surfaceMap, surfaceMotion, pDirection, retournement) && t < 60)	//Tant qu'on détecte une collision et que la boucle ne s'est pas effectuée plus de 60 fois (évite des boucles infinies)
	{
		switch (*pDirection)	//Switch de la valeur de derriere le pointeur pDirection, indiquant le sens de détection de la collision
		{
		case RIGHT:	//Cas d'une détection à droite
			surfaceMotion->clip_rect.x -= vitesse;	//Soustraction de la vitesse de déplacement sur l'axe x (0à gauche de l'écran et x à droite de l'écran)
			break;
		case LEFT:	//Cas d'une détection à gauche
			surfaceMotion->clip_rect.x += vitesse;	//Addition de la vitesse de déplacement sur l'axe x
			break;
		case DOWN:	//Cas d'une détection en bas
			surfaceMotion->clip_rect.y -= vitesse;	//Soustraction de la vitesse de déplacement sur l'axe y (0 en haut de l'écran et y en bas de l'écran)
			break;
		case UP:	//Cas d'une détection en hauteur
			surfaceMotion->clip_rect.y += vitesse;	//Addition de la vitesse de déplacement sur l'axe y
			break;
		case UPRIGHT:	//Cas d'une détection en diagonale haute droite
			surfaceMotion->clip_rect.x -= vitesse;	//Soustraction de la vitesse de déplacement sur l'axe x
			surfaceMotion->clip_rect.y += vitesse;	//Addition de la vitesse de déplacement sur l'axe y
			break;
		case UPLEFT:	//Cas d'une détection en diagonale haute gauche
			surfaceMotion->clip_rect.x += vitesse;	//Addition de la vitesse de déplacement sur l'axe x
			surfaceMotion->clip_rect.y += vitesse;	//Addition de la vitesse de déplacement sur l'axe y
			break;
		case DRIGHT:	//Cas d'une détection en diagonale basse droite
			surfaceMotion->clip_rect.x -= vitesse;	//Soustraction de la vitesse de déplacement sur l'axe x
			surfaceMotion->clip_rect.y -= vitesse;	//Soustraction de la vitesse de déplacement sur l'axe y 
			break;
		case DLEFT:	//Cas d'une détection en diagonale basse gauche
			surfaceMotion->clip_rect.x += vitesse;	//Addition de la vitesse de déplacement sur l'axe x
			surfaceMotion->clip_rect.y -= vitesse;	//Soustraction de la vitesse de déplacement sur l'axe y 
			break;
		default:
			break;
		}
		t++;	//Incrémentation du limiteur de boucle
		collision = 1;	//Mise à 1 de l'indicateur de collision
	}
	return collision;	//Return l'indice de collision, 0 = pas de collision, 1 = collision
}