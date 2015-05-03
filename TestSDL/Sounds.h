#ifndef SOUND_H
#define SOUND_H

#include "Libraries.h"

/*Chemins vers les sons des bombes*/
#define ExploMed "../assets/sounds/Explosion_medium.wav"
#define BipExplo "../assets/sounds/Bip_explosion.wav"
#define ExploSourde "../assets/sounds/Explosion_sourde.mp3"
//#define MusiqueMenu "../assets/sounds/MusiqueMenu.wav"
#define MusiqueMenu "../assets/sounds/test.mp3"

Mix_Music* ptrMusic;
Mix_Chunk* sndFx;

int initSDLMixer();
int loadSounds(const char* file, int type);
void cleanSounds();
#endif // !SOUND_H
