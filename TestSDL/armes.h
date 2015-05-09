#ifndef WEAPONS_H
#define WEAPONS_H

#include "Libraries.h" //Inclus toutes les librairies

#define BombFactor 0.5
#define MAXDAMAGE 20

void explosion(int x, int y, int rayon, SDL_Surface * pSurfaceMap, SDL_Texture * pTextureDisplay);
void weaponManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms *pWorms, int weaponIndex, SDL_Rect* pCamera);
double getAngle(int x, int y, enum DIRECTION *pDir, SDL_Rect* pCamera);
SDL_Surface* selectWeapon(int weapondIndex, enum DIRECTION dir);

#endif