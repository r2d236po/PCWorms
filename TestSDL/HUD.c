#include "HUD.h"

void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b)
{
	color->r = r;
	color->g = g;
	color->b = b;
}

void UpdateNameWorms(Worms* worms)
{
	static char str[100];
	sprintf(str, "%s\n%d", worms->nom, worms->vie);
	SDL_FreeSurface(worms->texteSurface);
	worms->texteSurface = TTF_RenderText_Blended(globalVar.FontName, str, globalVar.couleurNameBleu);
}