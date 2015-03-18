
#include "Libraries.h" //Inclus toutes les librairies
#include "AffichageGeneral.h"
#include "partie.h"


int main(int argc, char** argv)
{
	
	mainInit(2, 5);

	Jeu * game = nouveauJeu(globalVar.nbEquipe, globalVar.nbWormsEquipe, cMAP);
	
	if (mainFenetre(game) < 0)
	{
		printf("Une erreur s'est produite");
	}

	destroyJeu(&game);
	return 0;
}