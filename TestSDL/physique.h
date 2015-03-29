#ifndef PHYSIQUE_H
#define PHYSIQUE_H

#include "carte.h"
#include "worms.h"
#include "Libraries.h"


/*Gestion de la physique*/
int gestionPhysique(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Input* pInput, ...);
void gestionPhysiqueWorms(Input* pInput, Worms* pWorms, Terrain* map, SDL_Texture* pTextureDisplay);
void calculPositionRel(int* xRel, int* yRel, int t, float vit_X, float vit_Y, SDL_Surface* pSurface);

/*Fonction diverses*/
void finDeplacement(Input* pInput, Worms* pWorms, Terrain* pMapTerrain, int check);
enum DIRECTION calculDirectionDeplacement(int dx, int dy);
int checkDeplacement(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION direction);
void initGameWorms(Worms** wormsTab, Input* pInput, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Renderer* pRenderer, SDL_Rect* pCamera);
int rand_a_b(int a, int b);

#endif // !PHYSIQUE_H
