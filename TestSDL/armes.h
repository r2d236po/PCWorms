#ifndef WEAPONS_H
#define WEAPONS_H

#include "Libraries.h" //Inclus toutes les librairies

#define BombFactor 0.5
#define MAXDAMAGE 20

void explosion(int x, int y, int rayon, SDL_Surface * pSurfaceMap, SDL_Texture * pTextureDisplay);
void weaponManagement(Input *pInput, Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms *pWorms, int weaponIndex);
double getAngle(int x, int y, enum DIRECTION *pDir);

#endif