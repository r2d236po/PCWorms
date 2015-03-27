
#include "Libraries.h" //Inclus toutes les librairies
#include "AffichageGeneral.h"
#include "partie.h"


int main(int argc, char** argv)
{
	logFile = fopen("log.txt", "w+");
	if (logFile != NULL)
		fprintf(logFile, "Start of session : \n\n");

	mainInit(1, 2);
	Jeu * game = nouveauJeu(globalVar.nbEquipe, globalVar.nbWormsEquipe, cMAP_TEST3);
	if (mainFenetre(game) < 0)
	{
		if (logFile != NULL)
			fprintf(logFile, "mainFenetre : FAILURE.\n");
	}
	destroyJeu(&game);
	if (logFile != NULL)
	{
		fprintf(logFile, "\n\nEnd of Session.");
		fclose(logFile);
	}
	return 0;
}