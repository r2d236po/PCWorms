#ifndef SOUND_H
#define SOUND_H

#include "Libraries.h"

/*Chemins vers les sons des bombes*/
#define ExploMed "../assets/sounds/Explosion_medium.wav"
#define BipExplo "../assets/sounds/Bip_explosion.wav"
#define ExploSourde "../assets/sounds/Explosion_sourde.mp3"
#define MusiqueMenu "../assets/sounds/MusiqueMenu.wav"
#define MusiqueMenu2 "../assets/sounds/MusiqueMenu2.wav"
#define MusiqueMenu3 "../assets/sounds/ffvii.wav"
typedef struct // structure pour les chunk
{
	char *adresse;
	Mix_Chunk * ptrChunk;
}Chunk;

typedef struct // structure pour les chunk
{
	char *adresse;
	Mix_Music * ptrMusic;
}Music;

int indexTabChunk; // index de la dernière case non vide du tableau des chunks
int indexTabMusic; // index de la dernière case non vide du tableau des music
Mix_Chunk * sndFx;
Chunk chunkTab[30];
Music musicTab[5];
Mix_Music * ptrMusicMenu;


int initSDLMixer();
int loadSounds(char* file, int type);
int playMusique(int validation, char * file);
int findChunkInTab(char * file);
int findMusicInTab(char * file);
void cleanSounds();
#endif // !SOUND_H