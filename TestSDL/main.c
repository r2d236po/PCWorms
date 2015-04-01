
#include "Libraries.h" //Inclus toutes les librairies
#include "AffichageGeneral.h"
#include "partie.h"


int main(int argc, char** argv)
{
	{
		time_t t1;
		t1 = time(NULL);
		logFile = fopen("log.txt", "w+");
		if (logFile != NULL)
			fprintf(logFile, "Start of session : %s\n\n", ctime(&t1));
	};

	mainInit(1, 1);
	Jeu * game = nouveauJeu(globalVar.nbEquipe, globalVar.nbWormsEquipe, cMAP_TEST3);
	if (mainFenetre(game) < 0)
	{
		if (logFile != NULL)
			fprintf(logFile, "mainFenetre : FAILURE.\n");
	}
	saveGame(game);
	destroyJeu(&game);
	{
		time_t t1 = time(NULL);
		if (logFile != NULL)
		{
			fprintf(logFile, "\n\nEnd of Session : %s", ctime(&t1));
			fclose(logFile);
		}
	};
	return 0;
}