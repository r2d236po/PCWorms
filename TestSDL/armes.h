#ifndef WEAPONS_H
#define WEAPONS_H

#include "Libraries.h" //Inclus toutes les librairies

#define BombFactor 0.5
#define MAXDAMAGE 20
#define WEAPONSPEEDY 8.0
#define WEAPONSPEEDX 8.0

void explosion(int x, int y, int rayon, SDL_Surface * pSurfaceMap, SDL_Texture * pTextureDisplay);
void weaponManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms** wormsTab, int weaponIndex, SDL_Rect* pCamera);
double getAngle(int x, int y, enum DIRECTION *pDir, SDL_Rect* pCamera);
SDL_Surface* selectWeapon(int weapondIndex, enum DIRECTION dir);
void setCenterWeapons(Worms* pWorms, int* xCenter, int* yCenter);
int fireWeapon(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, enum DIRECTION dir, double angle, Worms** wormsTab, SDL_Surface* pSurface);
int moveBullet(Terrain *pMapTerrain, SDL_Surface* bulletSurface, double angle, Worms** wormsTab, enum DIRECTION dir);
int impactBulletWorms(Worms** wormsTab, SDL_Rect* pRect);
void initWeaponMode(Worms* pWorms, int* xCenter, int* yCenter);
void exitWeaponMode(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Worms* pWorms, SDL_Rect* pRect);

#endif