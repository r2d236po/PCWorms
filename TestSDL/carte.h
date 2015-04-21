#ifndef CARTE_H
#define CARTE_H

#include "worms.h"
/* Déclaration des structures et constantes relatives à la carte jouée */

typedef struct {	/* Structure terrain */
	SDL_Texture* imageBackground; /* background */
	SDL_Surface* collisionMapSurface;
	SDL_Surface* globalMapSurface;
} Terrain;

typedef struct{
	char* nom;			/* Nom de la caisse */
	int posx;			/* Position en X */
	int posy;			/* Position en Y */
	KaamObject* boxObject;
	//image?
} Caisse;

/*Fonction de gestion du terrain*/
int initialisionTerrain(Terrain** p_pMapTerrain, SDL_Renderer * pRenderer, const char * nomImageFond, const char * nomImageMap);
void destroyMap(Terrain** p_pMapTerrain);	//détruit un terrain


/*Fonctions diverses*/
enum DIRECTION calculDirectionCollision(enum DIRECTION direction, int zone, int checkMode);

/*Détection et gestion des collisions*/
int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurfaceMap, int* xE, int* yE, SDL_Rect* pRect);








#endif