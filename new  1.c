#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "F1_coeff_16bits.h"


short int TabC[3];

short int evalFunction(unsigned short int x, short int TabC[3], unsigned char w_l, unsigned short int mask_l){

    short int i;
    short int y;
	i = (x>>3)&mask_l; //calcul de l'index

    /*initialisation TabC*/
    TabC[0] = C1_8b[i][0];
    TabC[1] = C1_8b[i][1]; //Récupération des coefficients
    TabC[2] = C1_8b[i][2];

    y=x*TabC[2];
    y=y<<3;
    y+=TabC[1];
    y=(char)(y>>8);
    y*=x;
    y=y<<4;
    y+=TabC[0];

    return y;

}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer


    /*Appel Fonction*/
evalFunction(19,TabC, 3, 15);

	return 0;
}


{
				if (pInput->arme && indexWorms == globalVar.indexWormsTab && !armePrec) // On affiche l'arme la première fois
				{
					arme1->clip_rect.x = wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.x - 10;
					arme1->clip_rect.y = wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface->clip_rect.y + 5;
					display(arme1, 1);
					display(wormsTab[globalVar.indexWormsTab]->wormsObject->objectSurface, 0);
				}
				if (pInput->arme && armePrec) // On fait tourner l'arme en fonction de la souris
				{
					SDL_GetMouseState(&x, &y);
					xx = (wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.x) - x;
					yy = (wormsTab[indexWorms]->wormsObject->objectSurface->clip_rect.y) - y;
					z = -atan(yy/xx);
					z = (z / pi * 180.0);
					rotoSurface = rotozoomSurface(arme1, z, 1.0, 1);
					centerRectToPoint(&rotoSurface->clip_rect, arme1->clip_rect.x + arme1->w / 2, arme1->clip_rect.y + arme1->h / 2);
					display(rotoSurface, 1);
					display(wormsTab[indexWorms]->wormsObject->objectSurface, 0);
					
					/*SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
					SDL_RenderDrawRect(pRenderer, &rotoSurface->clip_rect);
					SDL_RenderPresent(pRenderer);*/
					SDL_FreeSurface(rotoSurface);
					rotoSurface = NULL;
					display(wormsTab[indexWorms]->wormsObject->objectSurface, 0);
				}
				else display(wormsTab[indexWorms]->wormsObject->objectSurface, 1);
				if (pInput->arme == 0 && armePrec == 1) // On efface l'arme
				{
					updateSurfaceFromSurface(pMapTerrain->globalMapSurface, pMapTerrain->collisionMapSurface, &rotoSurface->clip_rect, 1);
					updateTextureFromSurface(pTextureDisplay, pMapTerrain->globalMapSurface, &rotoSurface->clip_rect);
					display(wormsTab[indexWorms]->wormsObject->objectSurface, 1);
					SDL_FreeSurface(rotoSurface);
				}
				
				
				
				
				
				
				gestionArme(...)
				{
					SDL_Surface* rotoSurface = NULL;
					static int armePrec = 0;
					if (pInput->arme == 1 && armePrec == 0)
					{
						...
					}
					if (pInput->arme == 1 && armePrec == 1)
					{
						rotoSurface = rotozoom(...);
						...
					}
					else if (pInput->arme == 0 && armePrec == 1)
					{
						...
					}
					if (rotoSurface != NULL)
						SDL_FreeSurface(rotoSurface);
					display(wormsSurface);
					armePrec = pInput->arme;
				}
