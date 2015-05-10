#ifndef SOUND_H
#define SOUND_H

#include "Libraries.h"

/*Chemins vers les sons des bombes*/
#define ExploMed "../assets/sounds/Explosion_medium.wav"
#define BipExplo "../assets/sounds/Bip_explosion.wav"
#define ExploSourde "../assets/sounds/Explosion_sourde.wav"
#define MusiqueMenu "../assets/sounds/MusiqueMenu.wav"
#define MusiqueMenu2 "../assets/sounds/MusiqueMenu2.wav"
#define MusiqueVictoire "../assets/sounds/ffvii.wav"
#define MusiqueInGame "../assets/sounds/musicInGame.wav"

#define NBCHUNK 20
#define NBMUSIC 5

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
Chunk chunkTab[NBCHUNK];
Music musicTab[NBMUSIC];
Mix_Music * ptrMusicMenu;


int initSDLMixer();
int loadSounds(char* file, int type);
int playMusique(int validation, char * file);
int findChunkInTab(char * file);
int findMusicInTab(char * file);
int playChunk(int validation, char * file);
void cleanSounds();
#endif // !SOUND_H