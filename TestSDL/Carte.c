#include "carte.h"
#include "AffichageGeneral.h"
#include "worms.h"
#include "my_stdrFct.h"


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

	fprintf(logFile, "initialisationTerrain : START :\n\n");
	//Creation du pointeur de Terrain
	pMapTerrainTemp = (Terrain*)malloc(sizeof(Terrain));	//Allocation mémoire pour le pointeur de Terrain
	if (pMapTerrainTemp == NULL)	//Si l'allocation s'est mal passee
	{
		fprintf(logFile, "initialisationTerrain : FAILURE, allocation memoire de pMapTerrainTemp.\n\n");
		return -1;	//Retour d'erreur
	}
	pMapTerrainTemp->imageBackground = NULL;	//Initialisation à NULL du pointeur de la texture de l'image de fond
	pMapTerrainTemp->imageMapSurface = NULL;	//Initialisation à NULL du pointeur de la surface de l'image de la map

	//Creation de la texture de l'image de fond
	pMapTerrainTemp->imageBackground = loadTexture(pRenderer, nomImageFond);	//Chargement de l'image de fond dans la texture
	if (pMapTerrainTemp->imageBackground == NULL)	//Si le chargement a échoué
	{
		fprintf(logFile, "initialisationTerrain : FAILURE, loadTexture : imageBackground.\n\n");
		destroyMap(&pMapTerrainTemp);	//Destruction du Terrain
		return -1;	//Retour d'erreur
	}

	//Creation de la surface de la map qui servira pour la collision
	pMapTerrainTemp->imageMapSurface = loadImage(nomImageMap);	//Chargement de l'image de la map dans la surface
	if (pMapTerrainTemp->imageMapSurface == NULL) //Verification qu'il n'y a pas eu d'erreur lors de l'allocation en mémoire
	{
		fprintf(logFile, "initialisationTerrain : FAILURE, loadImage : imageMapSurface.\n\n");
		destroyMap(&pMapTerrainTemp);	//Destruction du Terrain
		return -1;	//Retour d'erreur
	}
	(*p_pMapTerrain) = pMapTerrainTemp; //Récupération du pointeur du Terrain
	pMapTerrainTemp = NULL;	//Remise à NULL du pointeur temporaire
	fprintf(logFile, "\ninitialisationTerrain : SUCCESS.\n\n");
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
	fprintf(logFile, "destroyMap : DONE.\n");
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
* \fn enum DIRECTION calculDirection(enum DIRECTION direction, int zone, int checkMode)
*
* \brief Determine la direction de la collision.
*
* \param[in] direction, direction du déplacement
* \param[in] zone, zone de la collision
* \return DIRECTION, direction de la collision
*/

enum DIRECTION calculDirectionCollision(enum DIRECTION direction, int zone, int checkMode)
{
	if (direction == RIGHT || direction == LEFT)
	{
		switch (zone)
		{
		case 7:
			return UP;
		case 8:
			return DOWN;
		default:
			break;
		}
	}
	if (checkMode && direction == UP && zone == 8)
		return DOWN;
	return direction;
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
* \fn int detectionCollisionSurfaceV2(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection)
* \brief Detecte s'il y a collision entre deux surfaces selon le sens de déplacement du worms.
*
* \param[in] pSurfaceMap, pointeur vers la surface de la map
* \param[in] pSurfaceMotion, pointeur vers la surface en mouvement dans la map
* \param[in] pDirection, pointeur de la direction du deplacement du worms
* \returns int, indicateur de collision : 1 = collision, 0 sinon
*/
int detectionCollisionSurfaceV2(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection, int checkMode)
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
	if (limitMap(pSurfaceMap->h, pSurfaceMap->w, pSurfaceMotion, pDirection))	//Detection d'un dépassement de la map
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
					collision = 1;	//Collision = 0 -> pas de collision
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
* \fn void calculXYBalayage(SDL_Surface* pSurfaceMotion, int* xStart, int* xEnd, int* yStart, int* yEnd, int zone)
* \brief Calcul les valeurs de debut et de fin pour X et Y afin d'optimiser la detection de collision.
*
* \param[in] psurfaceMotion, surface en deplacement
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


/**
* \fn int gestionCollision(int vitesse, SDL_Surface* surfaceMap, SDL_Surface* surfaceMotion, enum DIRECTION* pDirection)
* \brief replace le worms en cas de collision.
*
* \param[in] vitesse, vitesse de deplacement du worms, sert pour le replacement
* \param[in] surfaceMotion, surface en deplacement
* \param[in] surfaceMap, surface de la map
* \param[in] pDirection, direction du deplacement du worms, peut etre modifie par la fonction
* \returns int collision, indique s'il y a eu collision
*/
int gestionCollision(int vitesse, SDL_Surface* surfaceMap, SDL_Surface* surfaceMotion, enum DIRECTION* pDirection)
{
	int t = 0;
	int collision = 0;
	while (detectionCollisionSurfaceV2(surfaceMap, surfaceMotion, pDirection, 0) && t < 60)	//Tant qu'on détecte une collision et que la boucle ne s'est pas effectuée plus de 60 fois (évite des boucles infinies)
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