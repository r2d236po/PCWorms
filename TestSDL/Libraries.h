#ifndef LIBRARIES_H
#define LIBRARIES_H

/*Bibliothèques de base*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*Bibliothèques supplémentaires*/
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL2_rotozoom.h>

#include "Struct.h"

/*Macros*/
#define MY_ABS(a) ((a) < 0 ? (-a) : (a))
#define SWAP(x,y) ((x ^= y), (y ^= x), (x ^= y))
#define CARRE(a) ((a) * (a))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/*Constantes*/
#define pi 3.14159265358
#define RMASK 0x000000FF
#define GMASK 0x0000FF00
#define BMASK 0x00FF0000
#define AMASK 0xFF000000

/*Chemins de map*/
#define cMAP		"../assets/pictures/map.png"
#define cMAP_HD		"../assets/pictures/mapHD2.png"
#define cMAP_TEST	"../assets/pictures/maptest.png"
#define cMAP_TEST2	"../assets/pictures/maptest2.png"
#define cMAP_TEST3	"../assets/pictures/maptest3.png"
#define cMAP_TEST4	"../assets/pictures/maptest4.png"
#define cMAP_CAM	"../assets/pictures/maptestCam.png"
#define NUMBERMAP 7
#define ICONE "../assets/pictures/icone.png"

/*Variables Globales importantes*/
SDL_Window *globalWindow;
SDL_Renderer *globalRenderer;

GlobalVariable globalVar;
FILE* logFile;

SDL_Surface* spriteDeplacement;
SDL_Surface* spriteGrenadeExplosion;
SDL_Surface* spriteNukeExplosion;
SDL_Surface* arme1;
#endif