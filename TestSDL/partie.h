#ifndef GAME_H
#define GAME_H

#include "Libraries.h"

#define DEFAULTPATH "../assets/logAndResult/ResultGame.txt"

Jeu * nouveauJeu(char * map);
void destroyJeu(Jeu ** game);
Equipe * nouvelleEquipe(char * nomE, SDL_Color couleur, int nbWorms, int indexTeam);
void destroyEquipe(Equipe** team, int nbE);
int getLifeTeam(Equipe* team);
void updateTeamLife(Equipe** equipeTab);
int mainInit();					// Initialise les polices et les variables globales
void destroyPolice();
int saveGame(Jeu* jeu);
int isGameEnd(Equipe** equipeTab);

#endif