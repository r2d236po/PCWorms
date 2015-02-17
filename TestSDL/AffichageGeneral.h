#ifndef DISPLAY_H
#define DISPLAY_H


#include "Libraries.h" //Inclus toutes les librairies


int afficheImage(SDL_Window *pWindow, SDL_Surface * image); /*TODO: réecrire les protos*/
int mainFenetre();
int mainFenetre2(); //main de test pour le renderer
void AfficherPoint(SDL_Renderer r);
SDL_Surface * loadImage(const char * file);
SDL_Window * creerFenetre(const int w, const int h, const char * nom);

#endif
