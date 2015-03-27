#include "partie.h"
#include "AffichageGeneral.h"

/* Fonctions relatives à la gestion d'une partie */
/**
* \fn Jeu * nouveauJeu(int nbE, int nbW, char * map)
* \brief
*
* \param[in] nbe,
* \param[in] nbW,
* \param[in] map,
* \returns jeu, pointeur vers le jeu cree
*/
Jeu * nouveauJeu(int nbE, int nbW, char * map)
{
	Jeu * jeu = NULL;
	if (logFile != NULL)
		fprintf(logFile, "nouveauJeu : START :\n\n");
	jeu = malloc(sizeof(Jeu));
	if (jeu == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "nouveauJeu : FAILURE, allocation memoire jeu.\n\n");
		return NULL;
	}

	jeu->nbEquipe = nbE;
	jeu->temps = 1000;
	strcpy(jeu->nomMap, map);

	jeu->equipes = malloc(nbE * sizeof(Equipe*));
	if (jeu->equipes == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "nouveauJeu : FAILURE, allocation memoire jeu->equipes.\n\n");
		destroyJeu(&jeu);
		return NULL;
	}
	for (int i = 0; i < nbE; i++)
	{
		jeu->equipes[i] = nouvelleEquipe("Equipe", globalVar.couleurNameBleu, nbW);
		if (jeu->equipes[i] == NULL)
		{
			if (logFile != NULL)
				fprintf(logFile, "nouveauJeu : FAILURE, nouvelleEquipe.\n\n");
		}
	}
	if (logFile != NULL)
		fprintf(logFile, "nouveauJeu : SUCCESS.\n\n");
	return jeu;
}
/**
* \fn void destroyJeu(Jeu ** game)
* \brief Detruit un jeu et libere la memoire.
*
* \param[in] game, adresse du pointeur du jeu a detruire
* \returns void
*/
void destroyJeu(Jeu ** game)
{
	if ((*game)->equipes != NULL)
		destroyEquipe((*game)->equipes, (*game)->nbEquipe);
	free(*game);
	*game = NULL;
	if (logFile != NULL)
		fprintf(logFile, "destroyJeu : DONE.\n");
}

/**
* \fn Equipe * nouvelleEquipe(char * nomE, SDL_Color couleur, int nbWorms)
* \brief
*
* \param[in] nomE, nom de l'equipe
* \param[in] couleur, couleur de l'equipe
* \param[in] nbWorms, nombre de worms dans l'equipe
* \returns equipe, pointeur vers l'equipe cree
*/
Equipe * nouvelleEquipe(char * nomE, SDL_Color couleur, int nbWorms)
{
	Equipe * team = NULL;
	if (logFile != NULL)
		fprintf(logFile, "nouvelleEquipe : START :\n\n");
	team = malloc(sizeof(Equipe));
	if (team == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "nouvelleEquipe : FAILURE, allocation memoire team.\n\n");
		return NULL;
	}

	team->nbWormsStart = nbWorms;
	team->nom = nomE;
	team->color = couleur;

	/*déclaration des Worms*/
	team->worms = malloc(nbWorms * sizeof(Worms));
	if (team->worms == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "nouvelleEquipe : FAILURE, allocation memoire team->worms.\n\n");
		destroyEquipe(&team, globalVar.nbEquipe);
		return NULL;
	}
	for (int i = 0; i < nbWorms; i++)
	{
		team->worms[i] = createWorms("NomWorms");
		if (team->worms[i] == NULL)
		{
			if (logFile != NULL)
				fprintf(logFile, "nouvelleEquipe : FAILURE, createWorms.\n\n");
			destroyEquipe(&team, globalVar.nbEquipe);
			break;
		}
	}
	team->vie = vieEquipe(team);
	if (logFile != NULL)
		fprintf(logFile, "nouvelleEquipe : SUCCESS.\n\n");
	return team;
}

/**
* \fn void destroyEquipe(Equipe ** team, int nbE)
* \brief Detruit les equipes en jeu.
*
* \param[in] game, tableau contenant toutes les equipes
* \param[in] nbE, nombre d'equipes
* \returns void
*/
void destroyEquipe(Equipe ** team, int nbE)
{
	int i;
	for (i = 0; i < nbE; i++)
	{
		if (team[i]->worms != NULL)
			destroyWorms(team[i]->worms, team[i]->nbWormsStart);
	}
	free(*team);
	*team = NULL;
	if (logFile != NULL)
		fprintf(logFile, "destroyEquipe : DONE.\n");
}

/**
* \fn int vieEquipe(Equipe * team)
* \brief
*
* \param[in] team,
* \returns
*/
int vieEquipe(Equipe * team)
{
	int i, vie = 0;
	for (i = 0; i < team->nbWormsStart; i++)
	{
		vie += team->worms[i]->vie;
	}
	return vie;
}

/**
* \fn void mainInit(int nbE, int nbWpE)
* \brief
*
* \param[in] nbe, nombnre d'equipes
* \param[in] nbWpe, nombre de worms par equipe
* \returns void
*/
void mainInit(int nbE, int nbWpE)
{
	setSDLColor(&globalVar.couleurNameBleu, 238, 131, 127);
	setSDLColor(&globalVar.couleurNameRouge, 130, 125, 255);
	setSDLColor(&globalVar.couleurNameVert, 119, 250, 123);
	setSDLColor(&globalVar.couleurNameJaune, 120, 255, 255);

	globalVar.FontName = TTF_OpenFont("../assets/fonts/Worms_3D_Font.ttf", 20);

	globalVar.nbEquipe = nbE;
	globalVar.nbWormsEquipe = nbWpE;
	if (logFile != NULL)
		fprintf(logFile, "mainInit : DONE.\n\tnombre d'equipes : %d.\n\tnombre de worms par equipe : %d.\n\n", nbE, nbWpE);
}

/**
* \fn void destroyPolice()
* \brief detruit la police globale.
*
* \returns void
*/
void destroyPolice()
{
	TTF_CloseFont(globalVar.FontName);
}