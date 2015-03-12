#include "HUD.h"

void initialisationPolice()
{
	setSDLColor(&font.couleurNameBleu, 127, 131, 238);
	setSDLColor(&font.couleurNameRouge, 255, 125, 130);
	setSDLColor(&font.couleurNameVert, 123, 250, 119);
	setSDLColor(&font.couleurNameJaune, 255, 255, 120);

	font.FontName = TTF_OpenFont("../assets/fonts/Worms_3D_Font.ttf", 20);
}

void destroyPolice()
{
	TTF_CloseFont(font.FontName);
}

void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b)
{
	color->r = r;
	color->g = g;
	color->b = b;
}