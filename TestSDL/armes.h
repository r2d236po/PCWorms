#ifndef ARMES_H
#define ARMES_H
/* Déclaration des structures concernant les armes */

typedef struct{		/* Structure munition */
	char* nom;			/* Nom de la munition */
	int posx;			/* Position en X */
	int posy;			/* Position en Y */
	float angle;		/* Angle de la munition */
	//image?
} Munition;

typedef struct {	/* Structure arme */
	char* nom;			/* Nom de l'arme */
	float degats;		/* Dégats de l'arme */
	float radius;		/* Diamètre d'action de l'arme */
	float portee;		/* Portée de l'arme à rentrer dans l'équation de parabole (cf Gravitée) */
	int nbmuni;			/* Nombre de munitions */
	Munition muni;		/* Munition en cours d'utilisation */
	float angle;		/* Angle de l'arme (sa position est celle du worms) */
	//image?
	//son?
} Arme;

#endif