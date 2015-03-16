#ifndef INPUT_H
#define INPUT_H

#include "carte.h"

void getInput(Input* pInput, SDL_Window* pWindow);	//récupère les inputs
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms* worms);
Input* initInput();									//Initialise les Input

#endif