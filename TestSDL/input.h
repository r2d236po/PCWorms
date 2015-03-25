#ifndef INPUT_H
#define INPUT_H

#include "carte.h"

/*Acquisition des Inputs*/
void getInput(Input* pInput, SDL_Window* pWindow);	//Récupère les inputs

/*Gestion des Inputs*/
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms** wormsTab);	//Gestion globale des Inputs
void inputsCamera(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, SDL_Renderer * pRenderer);	//Gestion des Inputs de la camera
void inputsJumpWorms(Input* pInput, Worms* pWorms);	//Gestion des Inputs pour un saut de worms
void inputsWeapons(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Terrain* pMapTerrain, SDL_Renderer * pRenderer);	//Gestion des Inputs des armes

/*Initialisations*/
Input* initInput();		//Initialise les Input
Cursor initCursor(void);	//initialise la structure  cursor
#endif