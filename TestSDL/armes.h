#ifndef WEAPONS_H
#define WEAPONS_H

#include "Libraries.h" //Inclus toutes les librairies

#define BombFactor 0.5
#define MAXDAMAGE 20
#define WEAPONSPEEDY 25.0
#define WEAPONSPEEDX 25.0
#define TIMERGRENADE 7
#define GRENADEPATH "../assets/sprites/grenade.png"
#define GRENADESPRITE "../assets/sprites/Grenade_Sprite.png"
#define EXPLO1 "../assets/sprites/explo1.png"
#define EXPLO2 "../assets/sprites/explo2.png"
#define EXPLO3 "../assets/sprites/explo3.png"
#define EXPLO4 "../assets/sprites/explo4.png"
#define EXPLO5 "../assets/sprites/explo5.png"
#define GRENADESPEEDX 0.95
#define GRENADESPEEDY 1.3
#define NBFRAMEGRENADE 25

void explosion(int x, int y, int rayon, SDL_Surface * pSurfaceMap, SDL_Texture * pTextureDisplay);
void weaponManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms** wormsTab, int weaponIndex, SDL_Rect* pCamera);
double getAngle(int x, int y, enum DIRECTION *pDir, SDL_Rect* pCamera);
SDL_Surface* selectWeapon(int weapondIndex, enum DIRECTION dir);
void setCenterWeapons(Worms* pWorms, int* xCenter, int* yCenter);
int fireWeapon(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, enum DIRECTION dir, double angle, Worms** wormsTab, SDL_Surface* weaponSurface);
int moveBullet(Terrain *pMapTerrain, SDL_Surface* bulletSurface, double angle, Worms** wormsTab, enum DIRECTION dir);
int impactBulletWorms(Worms** wormsTab, SDL_Rect* pRect, int* index);
void initWeaponMode(Worms* pWorms, int* xCenter, int* yCenter, SDL_Rect* pRect);
void exitWeaponMode(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, Worms* pWorms, SDL_Rect* pRect);

void grenadeManagement(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, Worms** wormsTab, SDL_Rect* pCamera);
void initGrenade(SDL_Rect *pCamera, Worms *pWorms, float *speedX, float *speedY, enum DIRECTION *dir);
int animationGrenade(Terrain *pMapTerrain, SDL_Texture *pTextureDisplay, int *indexAnim, int x, int y);
int selectExplo(char *str, int indexGen);
void ajustExploWithMap(SDL_Surface* pSurfaceMap, SDL_Surface** exploSurface);

#endif