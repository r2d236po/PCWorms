#include "Sounds.h"
#include "memory.h"

/**
* \fn void initSDLMixer()
* \brief Détruit et libere les differents sons du jeu.
*
*/
int initSDLMixer()
{
	indexTabChunk = 0;
	indexTabMusic = 0;
	Mix_AllocateChannels(NBCHUNK/3);
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
		fprintf(logFile,"%s", Mix_GetError());
		return 0;
	}	else return 1;


}

/**
* \fn int loadSounds(const char* file, int type)
* \brief Charge un son ou une musique.
*
* \param[in] file, nom du fichier
* \param[in] type, type de son, 0 = chunk, 1 = musique
* \return -1 = FAIL, index dans la table des chunk ou des musique 
*/
int loadSounds(char* file, int type)
{
	int resultSearch = 0;
	switch (type)
	{
	case 0:
		resultSearch = findChunkInTab(file);
		if ( resultSearch == -1)
		{
			if (chunkTab[indexTabChunk].ptrChunk != NULL) indexTabChunk++;
			chunkTab[indexTabChunk].adresse = file;
			chunkTab[indexTabChunk].ptrChunk = Mix_LoadWAV(file);
			if (logFile != NULL && chunkTab[indexTabChunk].ptrChunk == NULL)
			{
				fprintf(logFile, "loadSounds FAILURE : %s.\n\n", Mix_GetError());
				return -1;
			}
		}
		else
		{
			if (chunkTab[resultSearch].ptrChunk == NULL)
			{
				chunkTab[resultSearch].ptrChunk = Mix_LoadWAV(file);
				if (logFile != NULL && chunkTab[resultSearch].ptrChunk == NULL)
				{
					fprintf(logFile, "loadSounds : FAILURE : %s.\n\n", Mix_GetError());
					return -1;
				}
			}
		}
		fprintf(logFile, "loadSounds : SUCCESS.\n\tname of the sound : %s.\n\n", file);
		return indexTabChunk;
		break;
	case 1:
		resultSearch = findMusicInTab(file);
		if (resultSearch == -1)
		{
			if (musicTab[indexTabMusic].ptrMusic != NULL) indexTabMusic++;
			musicTab[indexTabMusic].adresse = file;
			musicTab[indexTabMusic].ptrMusic = Mix_LoadMUS(file);
			if (logFile != NULL && musicTab[indexTabMusic].ptrMusic == NULL)
			{
				fprintf(logFile, "loadSounds : FAILURE : %s.\n\n", Mix_GetError());
				return -1;
			}
		}
		else
		{
			if (musicTab[resultSearch].ptrMusic == NULL)
			{
				musicTab[resultSearch].ptrMusic = Mix_LoadMUS(file);
				if (logFile != NULL && musicTab[resultSearch].ptrMusic == NULL)
				{
					fprintf(logFile, "loadSounds : FAILURE : %s.\n\n", Mix_GetError());
					return -1;
				}
			}
		}
		fprintf(logFile, "loadSounds : SUCCESS.\n\tname of the sound : %s.\n\n", file);
		return indexTabMusic;
		break;
	}
	return -1;
}

/**
* \fn void cleanSounds()
* \brief Détruit et libere les differents sons du jeu.
*
*/
void cleanSounds()
{
	for (int i = 0; i <= indexTabMusic; i++)
	{
		if (musicTab[i].ptrMusic != NULL){
			Mix_FreeMusic(musicTab[i].ptrMusic);
			musicTab[i].adresse = NULL;
			musicTab[i].ptrMusic = NULL;
		}
	}
	for (int i = 0; i <= indexTabChunk; i++)
	{
		if (chunkTab[i].ptrChunk != NULL){
			Mix_FreeChunk(chunkTab[i].ptrChunk);
			chunkTab[i].adresse = NULL;
			chunkTab[i].ptrChunk = NULL;
		}
	}
	fprintf(logFile, "cleanSounds : DONE.\n");
}


/**
* \fn int playMusique(int validation,char * file)
* \brief lit la musique du menu 
* \param[in] validation, 1 pour lancer 0 pour arreter
* \param[in] file, chemin du fichier à jouer
* \return -1 = FAIL, 1 = success
*/
int playMusique(int validation,char * file){
	if (validation){
		int index = findMusicInTab(file);
		if (index == -1){
			int index = loadSounds(file, 1);
			if (index != -1)
			{
				if (Mix_PlayMusic(musicTab[index].ptrMusic, -1) == -1)
				{
					fprintf(logFile, "Bug playsound: %s\n", Mix_GetError());
					return -1;
				}
			}
		}
		else{
			if (!Mix_PlayingMusic()){
				Mix_RewindMusic();
				if (Mix_PlayMusic(musicTab[index].ptrMusic, -1) == -1)
				{
					fprintf(logFile, "Bug playsound: %s\n", Mix_GetError());
					return -1;
				}
			}
		}
	}
	else{
		if (!validation && Mix_PlayingMusic())
		{
			while (!Mix_FadeOutMusic(1000) && Mix_PlayingMusic()) {
			}
		}
	}
	return 1;
}

/**
* \fn int findChunkInTab(char * file)
* \brief retourne l'index du chunk dans la table des chunks
* \param[in] file, pointeur sur le chemin du fichier 
* \return -1 = FAIL, i = l'index 
*/
int findChunkInTab(char * file){
	int i = 0;
	for (i = 0; i <= indexTabChunk; i++)
	{
		if (chunkTab[i].adresse != NULL){
			if (strcmp(chunkTab[i].adresse, file) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}

/**
* \fn int findMusicInTab(char * file)
* \brief retourne l'index de la musique dans la table des musique
* \param[in] file, pointeur sur le chemin du fichier
* \return -1 = FAIL, i = l'index
*/
int findMusicInTab(char * file){
	int i = 0;
	for (i = 0; i <= indexTabMusic; i++)
	{
		if (musicTab[i].adresse != NULL){
			if (!strcmp(musicTab[i].adresse, file))
			{
				return i;
			}
		}
	}
	return -1;
}

/**
* \fn int playChunk(int validation)
* \brief lit la musique du menu
* \param[in] validation, 1 pour lancer 0 pour arreter
* \return -1 = FAIL, 1 = success
*/
int playChunk(int validation, char * file){
	if (validation){
		int index = findChunkInTab(file);
		if (index == -1){
			int index = loadSounds(file, 0);
			if (index != -1)
			{
				if (Mix_PlayChannel(-1, chunkTab[indexTabChunk].ptrChunk,0) < 0)
				{
					fprintf(logFile, "Bug playchunk: %s\n", Mix_GetError());
					return -1;
				}
			}
		}
		else{
			if (Mix_PlayChannel(-1, chunkTab[index].ptrChunk, 0) < 0)
			{
				fprintf(logFile, "Bug playchunk: %s\n", Mix_GetError());
				return -1;
			}
		}
	}
	return 1;
}