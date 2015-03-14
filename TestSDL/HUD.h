#ifndef HUD_H
#define HUD_H

#include "Libraries.h" //Inclus toutes les librairies

/*SDL_Color couleurNameBleu = { 127, 131, 238 };
SDL_Color couleurNameRouge = { 255, 125, 130 };
SDL_Color couleurNameVert = { 123, 250, 119 };
SDL_Color couleurNameJaune = { 255, 255, 120 };*/

void initialisationPolice();	// Initialise les polices
void destroyPolice();
void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b);

#endif