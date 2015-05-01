
#include "Libraries.h" //Inclus toutes les librairies
#include "AffichageGeneral.h"
#include "partie.h"


int main(int argc, char** argv)
{
	{
		time_t t1;
		t1 = time(NULL);
		logFile = fopen("../assets/logAndResult/log.txt", "w+");
		if (logFile == NULL)
			exit(EXIT_FAILURE);
		fprintf(logFile, "Start of session : %s\n\n", ctime(&t1));
	};
	if (initSprites() < 0) // Ouverture des fichiers sprites
	{
		fclose(logFile);
		exit(EXIT_FAILURE);
	}
	strcpy(globalVar.savePath, DEFAULTPATH);
	if (mainFenetre() < 0)
	{
		fprintf(logFile, "mainFenetre : FAILURE.\n");
	}
	return 0;
}