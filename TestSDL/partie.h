#ifndef GAME_H
#define GAME_H

#include "worms.h"
#include "carte.h"
#include "Libraries.h" //Inclus toutes les librairies

#define TAILLETEAM 5
#define NBTEAM 5

/* Structures et constantes relatives à la gestion d'une partie */

typedef struct {			//Structure équipe
	int vie;					//Somme des points de vie de l'équipe
	char* nom;					//Nom d'équipe
	char* color;				//Couleur d'équipe
	Worms* equipe[TAILLETEAM];	//Tableau de Worms
	//logo?
} Equipe;



typedef struct {			//Structure d'une partie
	Equipe equipes[NBTEAM];		//Nombre d'équipe
	Terrain map;				//Carte en cours
	int temps;					//Durée

} Jeu;

#endif