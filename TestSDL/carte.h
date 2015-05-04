#ifndef CARTE_H
#define CARTE_H

#include "Libraries.h"

/*Fonction de gestion du terrain*/
int initialisionTerrain(Terrain** p_pMapTerrain, const char * nomImageFond, const char * nomImageMap);
void destroyMap(Terrain** p_pMapTerrain);	//détruit un terrain


#endif