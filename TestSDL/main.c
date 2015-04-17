
#include "Libraries.h" //Inclus toutes les librairies
#include "AffichageGeneral.h"
#include "partie.h"


int main(int argc, char** argv)
{
	{
		time_t t1;
		t1 = time(NULL);
		logFile = fopen("log.txt", "w+");
		if (logFile == NULL)
			exit(EXIT_FAILURE);
		fprintf(logFile, "Start of session : %s\n\n", ctime(&t1));
	};

	if (mainInit(1, 1) < 0)	//set le nombre d'équipe et le nombre de worms par équipe
	{
		fprintf(logFile, "mainInit : FAILURE.\n");
		return 0;
	}
	
	if (initSprites() < 0)
	{
		fclose(logFile);
		exit(EXIT_FAILURE);
	}
	Jeu * game = nouveauJeu(globalVar.nbEquipe, globalVar.nbWormsEquipe, cMAP_TEST3);
	if (mainFenetre(game) < 0)
	{
		fprintf(logFile, "mainFenetre : FAILURE.\n");
	}
	saveGame(game);
	destroyJeu(&game);
	{
		time_t t1 = time(NULL);
		fprintf(logFile, "\n\nEnd of Session : %s", ctime(&t1));
		fclose(logFile);
	};
	return 0;
}