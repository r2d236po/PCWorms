#ifndef HUD_H
#define HUD_H

#include "Libraries.h" //Inclus toutes les librairies


void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b);
void updateTextSurfaceWormsTab(Worms** wormsTab);
int updateTextSurfaceWorms(Worms* pWorms);
void updateTextSurfacePosition(Worms* pWorms);

#endif