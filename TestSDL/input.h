#ifndef INPUT_H
#define INPUT_H

#include "Libraries.h"
#include "carte.h"
#include "worms.h"

/*Acquisition des Inputs*/
void getInput(Input* pInput);	//Récupère les inputs

/*Gestion des Inputs*/
int gestInput(Input* pInput, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms** wormsTab);	//Gestion globale des Inputs
void inputsCamera(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms * pWorms);	//Gestion des Inputs de la camera
void inputsJumpWorms(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap);	//Gestion des Inputs pour un saut de worms
void inputsWeapons(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Terrain* pMapTerrain, Worms** wormsTab);	//Gestion des Inputs des armes
void callNextWorms(Worms** wormsTab);
int calculIndex();

/*Initialisations*/
Input* initInput();		//Initialise les Input
void resetStructInput(Input* pInput);
Cursor initCursor(void);	//initialise la structure  cursor
void secuTextInput(Input* pInput);

#endif