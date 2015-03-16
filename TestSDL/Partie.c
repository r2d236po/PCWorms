#include "partie.h"
#include "AffichageGeneral.h"

/* Fonctions relatives à la gestion d'une partie */

Jeu * nouveauJeu(int nbE, int nbW, char * map)
{
	Jeu * jeu = NULL;

	jeu = (Jeu*)malloc(sizeof(Jeu));

	if (jeu == NULL)
	{
		printf("Probleme lors de l'allocation de memoire du Jeu");
		return NULL;
	}
	jeu->nbEquipe = nbE;
	jeu->temps = 1000;
	strcpy(jeu->nomMap, "../assets/pictures/maptest3.png");

	jeu->equipes = malloc(nbE * sizeof(Equipe*));
	for ( int i = 0; i < nbE; i++)
	{
		jeu->equipes[i] = nouvelleEquipe("Equipe", font.couleurNameBleu, nbW );
	}
	return jeu;
}

void destroyJeu(Jeu ** game)
{
	destroyEquipe((*game)->equipes, (*game)->nbEquipe);

	free(*game);
	*game = NULL;
}


Equipe * nouvelleEquipe(char * nomE, SDL_Color couleur, int nbWorms)
{
	Equipe * team = NULL;

	team = (Equipe*)malloc(sizeof(Equipe));

	if (team == NULL)
	{
		printf("Probleme lors de l'allocation de memoire de l'Equipe");
		return NULL;
	}
	team->nbWormsStart = nbWorms;
	team->nom = nomE;
	team->color = couleur;

	/*déclaration des Worms*/
	team->worms = malloc(nbWorms * sizeof(Worms));
	for (int i = 0; i < nbWorms; i++)
	{
		team->worms[i] = createWorms("NomWorms");
	}


	team->vie = vieEquipe(team);
	return team;
}

void destroyEquipe(Equipe ** team, int nbE)
{
	int i;
	for (i = 0; i < nbE; i++)
	{
		destroyWorms(team[i]->worms, team[i]->nbWormsStart);
	}
	free(*team);
	*team = NULL;
}

int vieEquipe(Equipe * team)
{
	return 42;
}