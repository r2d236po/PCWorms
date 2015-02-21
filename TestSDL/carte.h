#ifndef CARTE_H
#define CARTE_H

#include "Libraries.h" //Inclus toutes les librairies

/* Déclaration des structures et constantes relatives à la carte jouée */


typedef struct {	/* Structure terrain */
	//mapcollision?				
	SDL_Texture* imageBackground; /* background */
	SDL_Texture* imageMap; /* background */
	SDL_Surface* imageMapSurface;
	SDL_Surface* mapCollision;
	//mapdestruction?
} Terrain;

typedef struct{
	char* nom;			/* Nom de la caisse */
	int posx;			/* Position en X */
	int posy;			/* Position en Y */
	//image?
} Caisse;

int initialisionTerrain(Terrain ** map, SDL_Renderer * pRenderer, const char * file, const char * file2);
int detectionCollisionSurface(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Surface* pSurface2);
int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Rect* pRect);
int gestionCollision(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE);
Uint32 ReadPixel(SDL_Surface *surface, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Surface* ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height);

#endif