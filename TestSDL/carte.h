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

/*Fonction de gestion du terrain*/
int initialisionTerrain(Terrain ** map, SDL_Renderer * pRenderer, const char * file, const char * file2);
void destroyMap(Terrain** map);	//détruit un terrain

/*Gestion de la physique*/
int gestionPhysique(Terrain* map, SDL_Texture* display, Input* pInput, ...);

/*Fonctions diverses*/
enum DIRECTION calculDirection(int x, int y, enum DIRECTION impulse, int w, int h);
int checkJump(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* dir);
int endJumpTest(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion);	//Test la fin d'un saut
int limitMap(int mapHight, int mapWidth, SDL_Surface* pSurface, enum DIRECTION* dir); //Détermine si un objet est hors map et le replace si il faut
enum DIRECTION calculDirectionSaut(int xRel, int yRel, enum DIRECTION sensSaut, Input* pInput);

/*Manipulation des pixels*/
Uint32 ReadPixel(SDL_Surface *surface, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

/*Détection et gestion des collisions*/
int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurfaceMap, int* xE, int* yE, SDL_Rect* pRect);
int detectionCollisionSurface(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion);
int detectionCollisionSurfaceV2(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* dir);
int gestionCollision(int vitesse, SDL_Surface* surfaceMotion, SDL_Surface* surfaceMap, enum DIRECTION *dir);

#endif