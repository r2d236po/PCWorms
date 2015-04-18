#include "partie.h"
#include "AffichageGeneral.h"
#include "my_stdrFct.h"

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
	jeu = (Jeu*)malloc(sizeof(Jeu));
	if (jeu == NULL)
	{
		fprintf(logFile, "nouveauJeu : FAILURE, allocating memory to jeu.\n\n");
		return NULL;
	}

	jeu->nbEquipe = nbE;
	jeu->temps = 1000;
	strcpy(jeu->nomMap, map);

	jeu->equipes = malloc(nbE * sizeof(Equipe*));
	if (jeu->equipes == NULL)
	{
		fprintf(logFile, "nouveauJeu : FAILURE, allocating memory to jeu->equipes.\n\n");
		destroyJeu(&jeu);
		return NULL;
	}
	for (int i = 0; i < nbE; i++)
	{
		jeu->equipes[i] = nouvelleEquipe("Equipe", globalVar.couleurBleu, nbW);
		if (jeu->equipes[i] == NULL)
		{
			fprintf(logFile, "nouveauJeu : FAILURE, nouvelleEquipe.\n\n");
		}
	}
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
	fprintf(logFile, "nouvelleEquipe : START :\n\n");

	team = (Equipe*)malloc(sizeof(Equipe));
	if (team == NULL)
	{
		fprintf(logFile, "nouvelleEquipe : FAILURE, allocating memory to team.\n\n");
		return NULL;
	}
	team->nbWormsStart = nbWorms;
	strcpy(team->nom, nomE);
	team->color = couleur;

	/*déclaration des Worms*/
	team->worms = malloc(nbWorms*sizeof(Worms*));
	if (team->worms == NULL)
	{
		fprintf(logFile, "nouvelleEquipe : FAILURE, allocating memory to team->worms.");
		destroyEquipe(&team, globalVar.nbEquipe);
		return NULL;
	}

	for (int i = 0; i < nbWorms; i++)
	{
		team->worms[i] = createWorms("Gerard");
		if (team->worms[i] == NULL)
		{
			fprintf(logFile, "nouvelleEquipe : FAILURE, createWorms.\n\n");
			destroyEquipe(&team, globalVar.nbEquipe);
			break;
		}
	}
	team->vie = vieEquipe(team);
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
int mainInit(int nbE, int nbWpE)
{
	/*Initialisation SDL_TTF*/
	if (TTF_Init() == -1)
	{
		fprintf(logFile, "initSWR : FAILURE, initialisation de TTF_Init : %s.\n\n", TTF_GetError());
		return -1;
	}

	setSDLColor(&globalVar.couleurBleu, 238, 131, 127);
	setSDLColor(&globalVar.couleurRouge, 130, 125, 255);
	setSDLColor(&globalVar.couleurVert, 119, 250, 123);
	setSDLColor(&globalVar.couleurJaune, 120, 255, 255);

	globalVar.FontName = NULL;
	globalVar.FontName = TTF_OpenFont("../assets/fonts/Worms_3D_Font.ttf", 16);  //  RETOURNE UN PUTAIN DE POINTEUR NULL
	if (globalVar.FontName == NULL)
	{
		fprintf(logFile, "Font loading : FAILURE.\n");
		fprintf(logFile, "error : %s\n", TTF_GetError());
		return -1;
	}

	globalVar.nbEquipe = nbE;
	globalVar.nbWormsEquipe = nbWpE;
	globalVar.teamPlaying = 0;
	globalVar.wormsPlaying = 0;
	globalVar.indexWormsTab = 0;

	fprintf(logFile, "mainInit : DONE.\n\tnombre d'equipes : %d.\n\tnombre de worms par equipe : %d.\n\n", nbE, nbWpE);

	return 0;
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

/**
* \fn int saveGame(Jeu* jeu)
* \brief Sauvegarde les résultat de la partie
*
* \param[in] jeu, pointeur vers la structure du jeu
* \returns  1 = succes, 0 = fail
*/
int saveGame(Jeu* jeu)
{
	FILE* file = fopen("Resultat de la Partie.txt", "w+");
	int indexEquipe = 0, indexWorms = 0;
	if (file == NULL)
	{
		fprintf(logFile, "saveGame : FAILURE, ouverture du fichier d'enregistrement.");
		return 0;
	}
	fprintf(file, "Resultat de la partie : \n\n");
	fprintf(file, "\tTemps de jeu : %d secondes.\n", 1000 - jeu->temps);
	fprintf(file, "\tMap jouée : %s.\n", jeu->nomMap);
	fprintf(file, "\tNombre d'equipes : %d.\n", jeu->nbEquipe);
	fprintf(file, "\tNombre de worms par equipe : %d.\n\n", globalVar.nbWormsEquipe);
	for (indexEquipe = 0; indexEquipe < jeu->nbEquipe; indexEquipe++)
	{
		fprintf(file, "Bravo à l'équipe %d composée des braves : \n", indexEquipe + 1);
		for (indexWorms = 0; indexWorms < globalVar.nbWormsEquipe; indexWorms++)
		{
			fprintf(file, "\t%s,", jeu->equipes[indexEquipe]->worms[indexWorms]->nom);
			if (jeu->equipes[indexEquipe]->worms[indexWorms]->vie == 0)
				fprintf(file, "\tTu es mort comme un héros !\n");
			else fprintf(file, "\tFélicitation pour avoir fait un massacre chez l'ennemie !\n");
		}
		fprintf(file, "\n\n");
	}
	fprintf(logFile, "saveGame : SUCCESS.\n");
	fclose(file);
	return 1;
}