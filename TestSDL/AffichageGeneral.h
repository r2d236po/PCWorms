#include <SDL/SDL.h>
#include <SDL\SDL_image.h> //test avec png
#include <stdio.h>


int afficheImage(SDL_Window *pWindow, SDL_Surface * image); /*TODO: réecrire les protos*/
int mainFenetre();
int mainFenetre2(); //main de test pour le renderer
SDL_Surface * loadImage(const char * file);
SDL_Window * creerFenetre(const int w, const int h, const char * nom);
