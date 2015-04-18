#include "HUD.h"

void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b)
{
	color->r = r;
	color->g = g;
	color->b = b;
}

void updateLifeSurfaceWorms(Worms** wormsTab)
{
	static char str[10];
	int i;
	SDL_Surface * txtSurface = NULL;

	for (i = 0; i < globalVar.nbEquipe * globalVar.nbWormsEquipe; i++)
	{
		sprintf(str, "%d", wormsTab[i]->vie);

		txtSurface = TTF_RenderText_Blended(globalVar.FontName, str, globalVar.couleurBleu);

		SDL_FreeSurface(wormsTab[i]->texteSurface);
		wormsTab[i]->texteSurface = txtSurface;
	}
	txtSurface = NULL;
	}