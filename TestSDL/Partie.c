#include "partie.h"
#include "AffichageGeneral.h"
#include "my_stdrFct.h"
#include "HUD.h"

/* Fonctions relatives à la gestion d'une partie */
/**
* \fn Jeu * nouveauJeu(int nbE, int nbW, char * map)
* \brief
*
* \param[in] map,
* \returns jeu, pointeur vers le jeu cree
*/
Jeu * nouveauJeu(char * map)
{
	Jeu * jeu = NULL;
	int i = 0;
	if (logFile != NULL)
		fprintf(logFile, "nouveauJeu : START :\n\n");
	if (globalVar.nbEquipe > 4)
		return NULL;
	jeu = (Jeu*)malloc(sizeof(Jeu));
	if (jeu == NULL)
	{
		fprintf(logFile, "nouveauJeu : FAILURE, allocating memory to jeu.\n\n");
		return NULL;
	}

	jeu->nbEquipe = globalVar.nbEquipe;
	jeu->temps = TEMPSPARTIE;
	strcpy(jeu->nomMap, map);

	jeu->equipes = malloc(globalVar.nbEquipe * sizeof(Equipe*));
	if (jeu->equipes == NULL)
	{
		fprintf(logFile, "nouveauJeu : FAILURE, allocating memory to jeu->equipes.\n\n");
		destroyJeu(&jeu);
		return NULL;
	}
	for (i = 0; i < globalVar.nbEquipe; i++)
	{
		jeu->equipes[i] = nouvelleEquipe(globalVar.teamNames[i], globalVar.colorTab[i], globalVar.nbWormsEquipe[i], i);
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
* \fn Equipe * nouvelleEquipe(char * nomE, SDL_Color couleur, int nbWorms, int indexTeam)
* \brief
*
* \param[in] nomE, nom de l'equipe
* \param[in] couleur, couleur de l'equipe
* \param[in] nbWorms, nombre de worms dans l'equipe
* \returns equipe, pointeur vers l'equipe cree
*/
Equipe * nouvelleEquipe(char * nomE, SDL_Color couleur, int nbWorms, int indexTeam)
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
		team->worms[i] = createWorms(team, globalVar.wormsNames[i + indexTeam * 4], &team->color);
		if (team->worms[i] == NULL)
		{
			fprintf(logFile, "nouvelleEquipe : FAILURE, createWorms.\n\n");
			destroyEquipe(&team, globalVar.nbEquipe);
			break;
		}
	}
	team->vie = getLifeTeam(team);
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
* \fn int getLifeTeam(Equipe * team)
* \brief Calculate the life of a team depending of the life of every worms in this team.
*
* \param[in] team, pointer of the team.
* \returns life of the team.
*/
int getLifeTeam(Equipe* team)
{
	int i, vie = 0;
	for (i = 0; i < team->nbWormsStart; i++)
	{
		vie += team->worms[i]->vie;
	}
	return vie;
}

/**
* \fn int updateTeamLife(Equipe** equipeTab)
* \brief Updates the life of every team in game.
*
* \param[in] equipeTab, array of team.
*/
void updateTeamLife(Equipe** equipeTab)
{
	int i;
	for (i = 0; i < globalVar.nbEquipe; i++)
	{
		equipeTab[i]->vie = getLifeTeam(equipeTab[i]);
	}
}

/**
* \fn int mainInit()
* \brief
*
* \returns 0 si OK, -1 si erreur
*/
int mainInit()
{
	int i;

	if(setFonts())
		return -1;

	timerTeamTexture = NULL;
	timerGeneralTexture = NULL;
	rectTimerTeam = initRect(0, 0, 10, 10);
	rectTimerGeneral = initRect(0, 0, 10, 10);

	globalVar.timeLastWormsChange = SDL_GetTicks();
	globalVar.timePause = 0;
	globalVar.timeStartGame = SDL_GetTicks();

	globalVar.indexWormsTab = 0;
	globalVar.gameEnd = 0;
	globalVar.teamPlaying = 0;
	for (i = 0; i < 4; i++)
	{
		globalVar.wormsPlaying[i] = 0;
	}

	fprintf(logFile, "mainInit : DONE.\n\tnombre d'equipes : %d.\n\tnombre de worms par equipe : %d %d %d %d.\n\n", globalVar.nbEquipe, 
		globalVar.nbWormsEquipe[0], globalVar.nbWormsEquipe[1], globalVar.nbWormsEquipe[2], globalVar.nbWormsEquipe[3]);

	return 0;
}

/**
* \fn int saveGame(Jeu* jeu)
* \brief Save the results of the game.
*
* \param[in] jeu, pointer to the game structure.
* \returns  1 = succes, 0 = fail
*/
int saveGame(Jeu* jeu)
{
	FILE* file = fopen(globalVar.savePath, "w+");
	int indexEquipe = 0, indexWorms = 0;
	if (file == NULL)
	{
		fprintf(logFile, "saveGame : FAILURE, ouverture du fichier d'enregistrement.");
		return 0;
	}
	fprintf(file, "Resultat de la partie : \n\n");
	fprintf(file, "\tTemps de jeu : %d secondes.\n", TEMPSPARTIE - jeu->temps);
	fprintf(file, "\tMap jouée : %s.\n", jeu->nomMap);
	fprintf(file, "\tNombre d'equipes : %d.\n", jeu->nbEquipe);
	fprintf(file, "\tNombre de worms par equipe : %d %d %d %d.\n\n", globalVar.nbWormsEquipe[0], globalVar.nbWormsEquipe[1], globalVar.nbWormsEquipe[2], globalVar.nbWormsEquipe[3]);
	for (indexEquipe = 0; indexEquipe < jeu->nbEquipe; indexEquipe++)
	{
		fprintf(file, "Bravo à l'équipe %d surnommée les \"%s\" composée des braves : \n", indexEquipe + 1, globalVar.teamNames[indexEquipe]);
		for (indexWorms = 0; indexWorms < globalVar.nbWormsEquipe[indexEquipe]; indexWorms++)
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

/**
* \fn int isGameEnd(Jeu* jeu)
* \brief Determines if a game is over by checking the global life of every team.
*
* \param[in] jeu, jeu en cours
* \returns  1 = the game is over, 0 = the game is not over yet.
*/
int isGameEnd(Jeu* jeu) {
	int i, nbTeamAlive = 0;
	if (jeu->temps < 0)
	{
		globalVar.gameEnd = 1;
		return 1;
	}

	for (i = 0; i < globalVar.nbEquipe; i++)
	{
		if (jeu->equipes[i]->vie > 0) nbTeamAlive++;
	}
	if (nbTeamAlive <= 1) {
		globalVar.gameEnd = 1;
		return 1;
	}
	return 0;
}