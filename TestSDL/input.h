#ifndef INPUT_H
#define INPUT_H

#include "carte.h"

/*Acquisition des Inputs*/
void getInput(Input* pInput, SDL_Window* pWindow);	//récupère les inputs

/*Gestion des Inputs*/
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms* worms);
void inputsCamera(Input* pInput, SDL_Texture* pTexture, SDL_Rect* camera, SDL_Renderer * pRenderer);
void inputsJumpWorms(Input* pInput, Worms* worms);

/*Initialisations*/
Input* initInput();		//Initialise les Input
Cursor initCursor(void);	//initialise la structure  cursor
#endif