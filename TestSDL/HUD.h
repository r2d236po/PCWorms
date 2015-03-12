#ifndef HUD_H
#define HUD_H

#include "Libraries.h" //Inclus toutes les librairies

/*SDL_Color couleurNameBleu = { 127, 131, 238 };
SDL_Color couleurNameRouge = { 255, 125, 130 };
SDL_Color couleurNameVert = { 123, 250, 119 };
SDL_Color couleurNameJaune = { 255, 255, 120 };*/

void initialisationPolice(TTF_Font ** NameFont);	// Initialise les polices
void destroyPolice(TTF_Font ** NameFont);

#endif