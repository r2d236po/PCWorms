#ifndef INPUT_H
#define INPUT_H

#include "Libraries.h"

/*Acquisition des Inputs*/
void getInput();	//Récupère les inputs

/*Gestion des Inputs*/
int gestInput(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms** wormsTab);	//Gestion globale des Inputs
void inputsCamera(SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms * pWorms);	//Gestion des Inputs de la camera
void inputsJumpWorms(Worms* pWorms, SDL_Surface* pSurfaceMap);	//Gestion des Inputs pour un saut de worms
void inputsWeapons(SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Terrain* pMapTerrain, Worms** wormsTab);	//Gestion des Inputs des armes
void callNextWorms(Worms** wormsTab);
int calculIndex();

/*Initialisations*/
Input* initInput();		//Initialise les Input
void resetStructInput();
Cursor initCursor(void);	//initialise la structure  cursor
void secuTextInput();

#endif