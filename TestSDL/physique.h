#ifndef PHYSIQUE_H
#define PHYSIQUE_H

#include "carte.h"
#include "worms.h"
#include "Libraries.h"


/*Gestion de la physique*/
int gestionPhysique(Terrain* map, SDL_Texture* display, Input* pInput, ...);
void gestionPhysiqueWorms(Input* pInput, Worms* worms, Terrain* map, SDL_Texture* display);
void calculPositionRel(int* xRel, int* yRel, int t, float vit_X, float vit_Y, SDL_Surface* pSurface);

/*Fonction diverses*/
void finDeplacement(Input* pInput, Worms* worms, Terrain* map, int check);
enum DIRECTION calculDirectionDeplacement(int xRel, int yRel, enum DIRECTION sensSaut, Input* pInput);
int checkDeplacement(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION direction);

#endif // !PHYSIQUE_H
