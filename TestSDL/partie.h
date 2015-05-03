#ifndef GAME_H
#define GAME_H

#include "worms.h"
#include "carte.h"

#define DEFAULTPATH "../assets/logAndResult/ResultGame.txt"

/* Structures et constantes relatives à la gestion d'une partie */

typedef struct Equipe{			//Structure équipe
	struct Worms** worms;				//Tableau de Worms
	int nbWormsStart;			//Nombre de Worms au début de la partie
	int vie;					//Somme des points de vie de l'équipe
	char nom[50];				//Nom d'équipe
	SDL_Color color;			//Couleur d'équipe
	//logo?
} Equipe;

typedef struct {				//Structure d'une partie
	Equipe** equipes;			//Tableau d'équipes
	int nbEquipe;				//Nombre d'équipe
	Terrain* pMapTerrain;		//Carte en cours
	int temps;					//Durée
	char nomMap[100];
} Jeu;


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