#include <SDL/SDL.h>
#include <SDL\SDL_image.h> //test avec png
#include <stdio.h>


int afficheImage(SDL_Window *pWindow, SDL_Surface * image); /*TODO: réecrire les protos*/
void mainFenetre();
SDL_Surface * loadImage(const char * file);
SDL_Window * creerFenetre(const int x, const int y, const char * nom);