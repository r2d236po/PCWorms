#include "Libraries.h"
#include "Sounds.h"

/**
* \fn void cleanSounds()
* \brief Détruit et libere les differents sons du jeu.
*
*/
void cleanSounds()
{
	Mix_FreeMusic(music);
	music = NULL;
	Mix_FreeChunk(sndFx);
	sndFx = NULL;
	if (logFile != NULL)
		fprintf(logFile, "cleanSounds : DONE.\n");
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
		music = Mix_LoadMUS(file);
		if (logFile != NULL && sndFx == NULL)
		{
			fprintf(logFile, "loadSounds : FAILURE : %s.\n\n", Mix_GetError());
			return -1;
		}
		break;
	}
	if (logFile != NULL)
		fprintf(logFile, "loadSounds : SUCCESS.\n\n");
	return 1;
}