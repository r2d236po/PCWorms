#include "HUD.h"

/**
* \fn void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b)
* \brief Initializes an SDL_Color structure.
*
* \param[in] color, pointer to the SDL_Color structure.
* \param[in] r, value of the red.
* \param[in] g, value of the green.
* \param[in] b, value of the blue.
* \returns void
*/
void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b)
{
	color->r = r;
	color->g = g;
	color->b = b;
}

/**
* \fn void updateLifeSurfaceWorms(Worms** wormsTab)
* \brief Updates the life display of the worms in game.
*
* \param[in] wormsTab, array of worms.
* \returns void
*/
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