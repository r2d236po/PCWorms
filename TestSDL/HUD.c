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
* \fn void updateTextSurfaceWorms(Worms** wormsTab)
* \brief Updates the life display of the worms in game.
*
* \param[in] wormsTab, array of worms.
* \returns void
*/
void updateTextSurfaceWorms(Worms** wormsTab)
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

	updateTextSurfacePosition(wormsTab);
}

/**
* \fn void updateTextSurfacePosition(Worms** wormsTab)
* \brief Updates the life display of the worms in game.
*
* \param[in] wormsTab, array of worms.
*/
void updateTextSurfacePosition(Worms** wormsTab)
{
	int i, t_x, t_y;

	for (i = 0; i < globalVar.nbEquipe * globalVar.nbWormsEquipe; i++)
	{
		t_x = wormsTab[i]->wormsObject->absoluteCoordinate.x + 15 - (wormsTab[i]->texteSurface->clip_rect.w / 2);
		t_y = wormsTab[i]->wormsObject->absoluteCoordinate.y - 20;

		if (t_x >= 0){ wormsTab[i]->texteSurface->clip_rect.x = t_x; }
		else { wormsTab[i]->texteSurface->clip_rect.x = wormsTab[i]->wormsObject->absoluteCoordinate.x; }

		if (t_y >= 0) { wormsTab[i]->texteSurface->clip_rect.y = t_y; }
		else { wormsTab[i]->texteSurface->clip_rect.y = wormsTab[i]->wormsObject->absoluteCoordinate.y; }
	}
}