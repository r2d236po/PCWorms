#include "HUD.h"

void initialisationPolice(TTF_Font ** NameFont)
{
	*NameFont = TTF_OpenFont("../assets/fonts/Worms_3D_Font.ttf", 65);
}

void destroyPolice(TTF_Font ** NameFont)
{
	TTF_CloseFont(*NameFont);
}