#ifndef WORMS_H
#define WORMS_H

#include "Libraries.h"

#define vitesseX (float)(cos(pi / 3)* 0.95)
#define vitesseY (float)(sin(pi / 3)*1.3)
#define TAILLEINV 5 
#define WIDTHSPRITEMOVE 31
#define HIGHTSPRITEMOVE 30
#define DAMAGEFALL 0.33
#define WORMSANIMSPEED 1

/*Initialisation / Destruction*/
Worms* createWorms(Equipe* team, char* name, SDL_Color* couleur);	//Créé un worms 
void destroyWorms(Worms** wormsTab, int nbWorms);	//détruit un worms

/*Deplacement d'un worms*/
void setWormsSpeed(Worms* pWorms, enum DIRECTION jumpDirection);

/*Gestion du retournement*/
int swapManagement(Worms* pWorms, SDL_Surface* pSurfaceMap);
int swapWorms(Worms* pWorms);
void swapWormsSurface(Worms* pWorms);

/*Gestion de l'animation*/
void gestionAnimationWorms(Worms* pWorms, int swap, SDL_Surface* pSurfaceMap, int random);
int animationWorms(Worms* pWorms, int indexFrameAnim, enum DIRECTION direction, int random);
void randomAnimationWorms(Worms* pWorms, SDL_Surface* pSurfaceMap);
int randomWorms();

/*Gestion de l'overlay*/
void updateGameWorms(Jeu* jeu, Worms** wormsTab, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera);
void wormsOverlay(Worms** wormsTab);

/*Fonctions diverses*/
int deathByLimitMap(Worms* pWorms, SDL_Surface* pSurfaceMap);
void wormsDead(Worms* pWorms, int limitMap);
void wormsFallDamages(Worms* pWorms);
void speedBombReaction(Worms* pWorms, int centerX, int centerY, int radius);
void bombReactionManagement(Worms** wormsTab, SDL_Rect* cercleBox, int centerX, int centerY, int radius);

void teleportWorms(Worms* pWorms, SDL_Surface* pSurfaceMap, SDL_Rect* pCamera);
int testTeleportPossibility(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion);

#endif // !1


