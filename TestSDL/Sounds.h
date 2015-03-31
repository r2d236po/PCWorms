#ifndef SOUND_H
#define SOUND_H


/*Chemins vers les sons des bombes*/
#define ExploMed "../assets/sounds/Explosion_medium.wav"
#define BipExplo "../assets/sounds/Bip_explosion.wav"
#define ExploSourde "../assets/sounds/Explosion_sourde.wav"
 
Mix_Music* music;
Mix_Chunk* sndFx;


int loadSounds(const char* file, int type);
void cleanSounds();
#endif // !SOUND_H
