#include "HUD.h"

void initialisationPolice()
{
	setSDLColor(&font.couleurNameBleu, 238, 131, 127);
	setSDLColor(&font.couleurNameRouge, 130, 125, 255);
	setSDLColor(&font.couleurNameVert, 119, 250, 123);
	setSDLColor(&font.couleurNameJaune, 120, 255, 255);

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