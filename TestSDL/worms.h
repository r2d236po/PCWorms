#ifndef WORMS_H
#define WORMS_H

#include "Libraries.h"
#include "carte.h"

#define vitesseX (float)(cos(pi / 3)* 0.95)
#define vitesseY (float)(sin(pi / 3)*1.3)
#define TAILLEINV 5 
#define widthSpriteMov 31
#define hightSpriteMov 30
#define DAMAGEFALL 0.33

typedef struct {			//Structure worms
	Equipe** equipe;
	int vie;					//Point de vie
	char nom[20];				//Nom du joueur
	char* arme;					//Arme en cours d'utilisation
	//Arme* invent[TAILLEINV];	//Inventaire des armes et objets disponibles
	enum DIRECTION dirSurface;
	SDL_Surface* wormsSurfaceLeft;
	SDL_Surface* wormsSurfaceRight;
	SDL_Surface* wormsSurfaceTomb;
	SDL_Surface* texteLifeSurface;
	SDL_Surface* texteNameSurface;
	KaamObject* wormsObject;
	SDL_Color* color;
	int indexAnim;
	//son?
} Worms;

/*Initialisation / Destruction*/
Worms* createWorms(char* name, SDL_Color* couleur);	//Créé un worms 
void destroyWorms(Worms** wormsTab, int nbWorms);	//détruit un worms

/*Deplacement d'un worms*/
void setWormsSpeed(Worms* pWorms, enum DIRECTION jumpDirection);

/*Gestion du retournement*/
int swapManagement(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap);
int swapWorms(Input* pInput, Worms* pWorms);
void swapWormsSurface(Worms* pWorms);

/*Gestion de l'animation*/
void gestionAnimationWorms(Worms* pWorms, int swap, SDL_Surface* pSurfaceMap);
int animationWorms(Worms* pWorms, int indexFrameAnim, enum DIRECTION direction);

/*Gestion de l'overlay*/
void updateGameWorms(Input* pInput, Worms** wormsTab, SDL_Surface* pSurfaceMapCollision, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay);
void wormsOverlay(Worms** wormsTab);

/*Fonctions diverses*/
int deathByLimitMap(Worms* pWorms, SDL_Surface* pSurfaceMap);
void wormsDead(Worms* pWorms, int limitMap);
void wormsFallDamages(Worms* pWorms);
void speedBombReaction(Worms* pWorms, int centerX, int centerY, int radius);
void bombReactionManagement(Input* pInput, Worms** wormsTab, SDL_Rect* cercleBox, int centerX, int centerY, int radius);

void teleportWorms(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap, SDL_Rect* pCamera);
int testTeleportPossibility(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion);

#endif // !1


