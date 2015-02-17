#ifndef DISPLAY_H
#define DISPLAY_H


#include "Libraries.h" //Inclus toutes les librairies


int afficheImage(SDL_Window *pWindow, SDL_Surface * image); /*TODO: réecrire les protos*/
int mainFenetre();
int mainFenetre2(); //main de test pour le renderer
int sandboxRenderer();
void afficherPoint(SDL_Renderer * r);
void clearScreen(SDL_Renderer * r);
void deplacementRectangle(SDL_Renderer * rend, SDL_Rect * rect, int x2, int y2); //déplace un rectangle dans l'espace en fonction des coordonnées de la souris
SDL_Surface * loadImage(const char * file);
SDL_Window * creerFenetre(const int w, const int h, const char * nom);

#endif
