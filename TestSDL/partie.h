#include "armes.h"
#include "carte.h"
#include "worms.h"

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