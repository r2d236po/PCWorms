#include <stdio.h>
#include <stdlib.h>
#define TAILLETEAM 5
#define TAILLEINV 5

int test(int argc, char** argv)
{	
	typedef struct {			//Déclaration de la structure de chaque Worms
		int vie;				//Point de vie
		char* nom;				//Nom du joueur
		char* arme;				//Arme en cours d'utilisation
		char* invent[TAILLEINV];//Inventaire des armes et objets disponibles
		int posx;				//Position en X
		int posy;				//Position en Y
		int dir;				//Direction : gauche ou droite
		//image?
		//son?
	} Worms;

	typedef struct {				//Structure équipe
		int vie;					//Somme des points de vie de l'équipe
		char* nom;					//Nom d'équipe
		char* color;				//Couleur d'équipe
		Worms* equipe[TAILLETEAM];	//Tableau de Worms
	} Equipe;

	typedef struct {		//Déclaration de la structure de chaque Worms
		char* nom;			//Nom de l'arme
		float degats;		//Dégats de l'arme
		float radius;		//Diamètre d'action de l'arme
		float portee;		//Portée de l'arme à rentrer dans l'équation de parabole (cf Gravitée)
		int nbmuni;			//Nombre de munitions
		char* munition;		//Munition en cours d'utilisation
		float angle;		//Angle de l'arme (sa position est celle du worms)
		//image?
		//son?
	} Arme;

	typedef struct{
		char* nom;			//Nom de la munition
		int posx;			//Position en X
		int posy;			//Position en Y
		float angle;		//Angle de la munition
		//image?
	} Munition;
}

