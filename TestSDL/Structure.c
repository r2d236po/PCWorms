#include <stdio.h>
#include <stdlib.h>
#define TAILLEINV 5
#define TAILLETEAM 5
#define NBTEAM 5

void Structure()
{
	//Structures déjà déclarées dans l'ordre : muni, arme, worms, equipe, terrain, jeu, caisse

	typedef struct{		//Structure munition
		char* nom;			//Nom de la munition
		int posx;			//Position en X
		int posy;			//Position en Y
		float angle;		//Angle de la munition
		//image?
	} Munition;

	typedef struct {	//Structure arme
		char* nom;			//Nom de l'arme
		float degats;		//Dégats de l'arme
		float radius;		//Diamètre d'action de l'arme
		float portee;		//Portée de l'arme à rentrer dans l'équation de parabole (cf Gravitée)
		int nbmuni;			//Nombre de munitions
		Munition muni;		//Munition en cours d'utilisation
		float angle;		//Angle de l'arme (sa position est celle du worms)
		//image?
		//son?
	} Arme;

	typedef struct {		//Structure worms
		int vie;				//Point de vie
		char* nom;				//Nom du joueur
		char* arme;				//Arme en cours d'utilisation
		Arme* invent[TAILLEINV];//Inventaire des armes et objets disponibles
		int posx;				//Position en X
		int posy;				//Position en Y
		int dir;				//Direction : gauche ou droite
		//image?
		//son?
	} Worms;

	typedef struct {			//Structure équipe
		int vie;					//Somme des points de vie de l'équipe
		char* nom;					//Nom d'équipe
		char* color;				//Couleur d'équipe
		Worms* equipe[TAILLETEAM];	//Tableau de Worms
		//logo?
	} Equipe;

	typedef struct {			//Structure terrain
		//mapcollision?				
		//background?
		//mapdestruction?
	} Terrain;

	typedef struct {			//Structure d'une partie
		Equipe equipes[NBTEAM];		//Nombre d'équipe
		Terrain map;				//Carte en cours
		int temps;					//Durée

	} Jeu;

	typedef struct{
		char* nom;			//Nom de la caisse
		int posx;			//Position en X
		int posy;			//Position en Y
		//image?
	} Caisse;
}

