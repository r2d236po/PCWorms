#ifndef WORMS_H
#define WORMS_H

#include "Libraries.h"
#include "armes.h"

#define vitesseX (float)(cos(pi / 3)* 0.95)
#define vitesseY (float)(sin(pi / 3)*1.3)
#define TAILLEINV 5
#define widthSpriteMov 31
#define hightSpriteMov 30

typedef struct {		//Structure worms
	int vie;				//Point de vie
	char nom[20];				//Nom du joueur
	char* arme;				//Arme en cours d'utilisation
	Arme* invent[TAILLEINV];//Inventaire des armes et objets disponibles
	enum DIRECTION dirSurface;
	SDL_Surface* wormsSurfaceLeft;
	SDL_Surface* wormsSurfaceRight;
	SDL_Surface* wormsSurfaceTomb;
	SDL_Surface* texteSurface;
	KaamObject* wormsObject;
	int indexAnim;
	//son?
} Worms;

/*Initialisation / Destruction*/
Worms* createWorms(char* name);	//Créé un worms à partir d'une image
void destroyWorms(Worms** wormsTab, int nbWorms);	//détruit un worms

/*Deplacement d'un worms*/
void setWormsSpeed(Worms* pWorms, enum DIRECTION jumpDirection);

/*Gestion du retournement*/
int swapManagement(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap);
int swapWorms(Input* pInput, Worms* pWorms);
void swapWormsSurface(Worms* pWorms);

/*Gestion de l'animation*/
void gestionAnimationWorms(Worms* pWorms, SDL_Surface* pSurfaceMap, int swap);
int animationWorms(Worms* pWorms, int indexFrameAnim, enum DIRECTION direction);

/*Gestion de l'overlay*/
void updateGameWorms(Input* pInput, Worms** wormsTab, SDL_Texture* pTextureDisplay, SDL_Surface* pSurfaceMap);
int wormsOverlay(Worms** wormsTab, int* indexWorms1, int* indexWorms2);

/*Fonctions diverses*/
int deathByLimitMap(Worms* pWorms, SDL_Surface* pSurfaceMap);
void wormsDead(Worms* pWorms, int limitMap);
void speedBombReaction(Worms* pWorms, int centerX, int centerY, int radius);
void bombReactionManagement(Input* pInput, Worms** wormsTab, SDL_Rect* cercleBox, int centerX, int centerY, int radius);

#endif // !1


