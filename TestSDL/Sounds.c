#include "Sounds.h"

/**
* \fn void initSDLMixer()
* \brief Détruit et libere les differents sons du jeu.
*
*/
int initSDLMixer()
{
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
		printf("%s", Mix_GetError());
		return 0;
	}
	else return 1;
}

/**
* \fn int loadSounds(const char* file, int type)
* \brief Charge un son ou une musique.
*
* \param[in] file, nom du fichier
* \param[in] type, type de son, 0 = chunk, 1 = musique
* \return -1 = FAIL, 1 = success
*/
int loadSounds(const char* file, int type)
{
	switch (type)
	{
	case 0:
		sndFx = Mix_LoadWAV(file);
		if (logFile != NULL && sndFx == NULL)
		{
			fprintf(logFile, "loadSounds : FAILURE : %s.\n\n", Mix_GetError());
			return -1;
		}
		break;
	case 1:
		ptrMusic = Mix_LoadMUS(MusiqueMenu);
		if (logFile != NULL && ptrMusic == NULL)
		{
			fprintf(logFile, "loadSounds : FAILURE : %s.\n\n", Mix_GetError());
			return -1;
		}
		break;
	}
	fprintf(logFile, "loadSounds : SUCCESS.\n\tname of the sound : %s.\n\n", file);
	return 1;
}

/**
* \fn void cleanSounds()
* \brief Détruit et libere les differents sons du jeu.
*
*/
void cleanSounds()
{
	if (ptrMusic != NULL){
		Mix_FreeMusic(ptrMusic);
		ptrMusic = NULL;
	}
	if (sndFx != NULL){
		Mix_HaltMusic();
		Mix_FreeChunk(sndFx);
		sndFx = NULL;
	}
	fprintf(logFile, "cleanSounds : DONE.\n");
}