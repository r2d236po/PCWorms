
#include "Libraries.h" //Inclus toutes les librairies
#include "AffichageGeneral.h"

int main(int argc, char** argv)
{
	//sandboxRenderer();
	if (mainFenetre() < 0)
	{
		printf("Une erreur s'est produite");
	}
	return 0;
}