#ifndef CARTE_H
#define CARTE_H

#include "worms.h"
/* Déclaration des structures et constantes relatives à la carte jouée */

typedef struct {	/* Structure terrain */
	SDL_Texture* imageBackground; /* background */
	SDL_Surface* imageMapSurface;
} Terrain;

typedef struct{
	char* nom;			/* Nom de la caisse */
	int posx;			/* Position en X */
	int posy;			/* Position en Y */
	//image?
} Caisse;

int initialisionTerrain(Terrain ** map, SDL_Renderer * pRenderer, const char * file, const char * file2);
void destroyMap(Terrain** map);	//détruit un terrain
int detectionCollisionSurface(SDL_Surface* pSurface, SDL_Surface* pSurface2);
int detectionCollisionSurfaceV2(SDL_Surface* pSurface, SDL_Surface* pSurface2, enum DIRECTION* dir, Input* pInput);
int limitMap(int mapHight,int mapWidth, SDL_Surface* pSurface, enum DIRECTION* dir);
int gestionCollision(int vitesse, SDL_Surface* surfaceMotion, SDL_Surface* surfaceCollision, enum DIRECTION *dir, Input* pInput);
int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Rect* pRect);
int gestionPhysique(Terrain* map, Input* pInput, ...);
Uint32 ReadPixel(SDL_Surface *surface, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
enum DIRECTION calculDirection(int x, int y, enum DIRECTION impulse, int w, int h, Input* pInput);
#endif