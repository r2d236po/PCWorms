#include "carte.h"
#include "AffichageGeneral.h"
#include "worms.h"
#include "my_stdrFct.h"
#include "memory.h"


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Gestion du terrain                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int initialisionTerrain(Terrain** p_pMapTerrain, const char * nomImageFond, const char * nomImageMap)
* \brief Initialise un terrain avec son image de fond et l'image de la map.
*
* \param[in] p_pMapTerrain, adresse du pointeur de structure Terrain a initialiser
* \param[in] nomImageFond, nom du fichier de l'image de fond
* \param[in] nomImageMap, nom du fichier de l'image de la map
* \returns int, Indicateur de reussite de la fonction : 1 = succes, -1 = echec
*/
int initialisionTerrain(Terrain** p_pMapTerrain, const char * nomImageFond, const char * nomImageMap)
{
	Terrain * pMapTerrainTemp = NULL;	//Pointeur du Terrain temporaire

	fprintf(logFile, "initialisationTerrain : START :\n\n");
	//Creation du pointeur de Terrain
	pMapTerrainTemp = (Terrain*)my_malloc(sizeof(Terrain));	//Allocation mémoire pour le pointeur de Terrain
	if (pMapTerrainTemp == NULL)	//Si l'allocation s'est mal passee
	{
		fprintf(logFile, "initialisationTerrain : FAILURE, allocation memoire de pMapTerrainTemp.\n\n");
		decreaseMalloc();
		return -1;	//Retour d'erreur
	}
	pMapTerrainTemp->imageBackground = NULL;	//Initialisation à NULL du pointeur de la texture de l'image de fond
	pMapTerrainTemp->collisionMapSurface = NULL;	//Initialisation à NULL du pointeur de la surface de l'image de la map

	//Creation de la texture de l'image de fond
	pMapTerrainTemp->imageBackground = loadTexture(nomImageFond);	//Chargement de l'image de fond dans la texture
	if (pMapTerrainTemp->imageBackground == NULL)	//Si le chargement a échoué
	{
		fprintf(logFile, "initialisationTerrain : FAILURE, loadTexture : imageBackground.\n\n");
		destroyMap(&pMapTerrainTemp);	//Destruction du Terrain
		return -1;	//Retour d'erreur
	}

	//Creation de la surface de la map qui servira pour la collision
	pMapTerrainTemp->collisionMapSurface = loadImage(nomImageMap);	//Chargement de l'image de la map dans la surface
	if (pMapTerrainTemp->collisionMapSurface == NULL) //Verification qu'il n'y a pas eu d'erreur lors de l'allocation en mémoire
	{
		fprintf(logFile, "initialisationTerrain : FAILURE, loadImage : collisionMapSurface.\n\n");
		destroyMap(&pMapTerrainTemp);	//Destruction du Terrain
		return -1;	//Retour d'erreur
	}
	pMapTerrainTemp->globalMapSurface = my_CreateRGBSurface(pMapTerrainTemp->collisionMapSurface->flags,
		pMapTerrainTemp->collisionMapSurface->w, pMapTerrainTemp->collisionMapSurface->h, 32, RMASK, GMASK, BMASK, AMASK);
	if (pMapTerrainTemp->globalMapSurface == NULL) //Verification qu'il n'y a pas eu d'erreur lors de l'allocation en mémoire
	{
		fprintf(logFile, "initialisationTerrain : FAILURE, createRGBsurface : %s.\n\n", SDL_GetError());
		destroyMap(&pMapTerrainTemp);	//Destruction du Terrain
		return -1;	//Retour d'erreur
	}
	memcpy(pMapTerrainTemp->globalMapSurface->pixels, pMapTerrainTemp->collisionMapSurface->pixels, pMapTerrainTemp->collisionMapSurface->h*pMapTerrainTemp->collisionMapSurface->w*sizeof(Uint32));
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
		my_freeTexture((*p_pMapTerrain)->imageBackground);	//Destruction de la texture de l'image de fond
		(*p_pMapTerrain)->imageBackground = NULL;	//Remise à NULL du pointeur
	}
	if ((*p_pMapTerrain)->collisionMapSurface != NULL)	//Si le pointeur de la surface de l'image de la map n'est pas NULL
	{
		my_freeSurface((*p_pMapTerrain)->collisionMapSurface);	//Destruction de la surface de l'image de la map
		(*p_pMapTerrain)->collisionMapSurface = NULL;	//Remise à NULL du pointeur
	}
	if ((*p_pMapTerrain)->globalMapSurface != NULL)	//Si le pointeur de la surface de l'image de la map n'est pas NULL
	{
		my_freeSurface((*p_pMapTerrain)->globalMapSurface);	//Destruction de la surface de l'image de la map
		(*p_pMapTerrain)->globalMapSurface = NULL;	//Remise à NULL du pointeur
	}
	my_free(*p_pMapTerrain);	//Liberation de la mémoire du pointeur du Terrain
	*p_pMapTerrain = NULL;	//Remise à NULL du pointeur
	fprintf(logFile, "destroyMap : DONE.\n");
}