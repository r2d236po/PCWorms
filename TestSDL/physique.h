#ifndef PHYSIQUE_H
#define PHYSIQUE_H
#include "Libraries.h"
#include "worms.h"
#include "carte.h"

/*Gestion de la physique*/
int gestionPhysique(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Input* pInput, ...);

/*Fonction diverses*/
int checkDeplacement(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION direction);
void initGameWorms(Worms** wormsTab, Input* pInput, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Renderer* pRenderer, SDL_Rect* pCamera);


#endif // !PHYSIQUE_H
