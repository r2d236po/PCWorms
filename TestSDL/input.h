#ifndef INPUT_H
#define INPUT_H

#include "Libraries.h" //Inclus toutes les librairies
#include "carte.h"

void getInput(Input* pInput, SDL_Window* pWindow);	//r�cup�re les inputs
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms* worms, SDL_Surface * surfaceTab[]);
Input* initInput();									//Initialise les Input

#endif