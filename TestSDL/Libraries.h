#ifndef LIBRARIES_H
#define LIBRARIES_H

/*
ATTENTION A LIRE : Si vous ajoutez de nouveaux headers ou de nouvelles librairies et 
qu'ils/elles incluent d'autres headers/lib déjà présent, mettez l'include à la fin de la derniere lib dépendante.
EXEMPLE : partie.h dépend de worms.h donc je le mets après sinon ça plante à la compilation.
*/


/*Bibliothèques de base*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

/*Bibliothèques supplémentaires*/
#include <SDL/SDL_image.h>

/*Headers du projet*/


#endif