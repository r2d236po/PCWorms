#include "Sounds.h"

/**
* \fn void initSDLMixer()
* \brief Détruit et libere les differents sons du jeu.
*
*/
int initSDLMixer()
{
	ptrMusicMenu = NULL;
	indextab = 0;
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
* \return -1 = FAIL, 1 = success
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
			chunkTab[indextab+1].adresse = file;
			chunkTab[indextab+1].ptrChunk = Mix_LoadWAV(file);
			if (logFile != NULL && chunkTab[indextab+1].ptrChunk == NULL)
			{
				fprintf(logFile, "loadSounds : FAILURE : %s.\n\n", Mix_GetError());
				return -1;
			}
			else indextab++;
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
		break;
	case 1:
		ptrMusicMenu = Mix_LoadMUS(file);
		if (logFile != NULL && ptrMusicMenu == NULL)
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
	if (ptrMusicMenu != NULL){
		Mix_FreeMusic(ptrMusicMenu);
		ptrMusicMenu = NULL;

	}
	for (int i = 0; i <= indextab; i++)
	{
		if (chunkTab[i].ptrChunk != NULL){

			Mix_FreeChunk(chunkTab[i].ptrChunk);
			chunkTab[i].ptrChunk = NULL;
		}




	}
	fprintf(logFile, "cleanSounds : DONE.\n");
}


/**
* \fn int playMusiqueMenu(int validation)
* \brief lit la musique du menu 
* \param[in] validation, 1 pour lancer 0 pour arreter
* \return -1 = FAIL, 1 = success
*/
int playMusiqueMenu(int validation){
	if (validation){
		if (ptrMusicMenu == NULL){
			if (loadSounds(MusiqueMenu, 1))
			{
				if (Mix_PlayMusic(ptrMusicMenu, -1) == -1)
				{
					fprintf(logFile, "Bug playsound: %s\n", Mix_GetError());
					return -1;
				}
			}
		}
		else{
			if (!Mix_PlayingMusic()){
				Mix_RewindMusic();
				if (Mix_PlayMusic(ptrMusicMenu, -1) == -1)
				{
					fprintf(logFile, "Bug playsound: %s\n", Mix_GetError());
					return -1;
				}
			}
		}
	}
	else if (!validation && (ptrMusicMenu != NULL) && Mix_PlayingMusic())
	{
		while (!Mix_FadeOutMusic(1000) && Mix_PlayingMusic()) {
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
	for ( i = 0; i < indextab; i++)
	{
		if (strcmp(chunkTab[i].adresse, file))
		{
			return i;
		}
	}
	return -1;
}