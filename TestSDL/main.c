
#include "Libraries.h" //Inclus toutes les librairies
#include "AffichageGeneral.h"
#include "partie.h"


int main(int argc, char** argv)
{
	int nbEquipe = 1;
	int nbWormsEquipe = 1;

	

	Jeu * game = nouveauJeu(nbEquipe, nbWormsEquipe, "../assets/pictures/maptest3.png");
	
	//sandboxRenderer();
	if (mainFenetre(game) < 0)
	{
		printf("Une erreur s'est produite");
	}
	destroyJeu(&game);
	return 0;
}