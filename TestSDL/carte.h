

#ifndef CARTE_H
#define CARTE_H

#include "Libraries.h" //Inclus toutes les librairies
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
int detectionCollisionSurface(SDL_Surface* pSurface, SDL_Surface* pSurface2, enum DIRECTION* dir);
int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Rect* pRect); 
int gestionPhysique(SDL_Renderer* pRenderer, Terrain* map, SDL_Texture* pDisplay, Input* pInput, Uint32* tPrevious, ...);
Uint32 ReadPixel(SDL_Surface *surface, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
enum DIRECTION calculDirection(int x, int y, enum DIRECTION impulse, int w, int h);
#endif