#ifndef SOUND_H
#define SOUND_H

#include "Libraries.h"

/*Chemins vers les sons des bombes*/
#define ExploMed "../assets/sounds/Explosion_medium.wav"
#define BipExplo "../assets/sounds/Bip_explosion.wav"
#define ExploSourde "../assets/sounds/Explosion_sourde.mp3"
#define MusiqueMenu "../assets/sounds/MusiqueMenu.wav"

typedef struct // structure pour les chunk
{
	char *adresse;
	Mix_Chunk * ptrChunk;
}Chunk;

int indextab; // index de la dernière case non vide du tableau
Mix_Chunk * sndFx;
Chunk chunkTab[30];
Mix_Music * ptrMusicMenu;


int initSDLMixer();
int loadSounds(char* file, int type);
int playMusiqueMenu(int validation);
int findChunkInTab(char * file);
void cleanSounds();
#endif // !SOUND_H