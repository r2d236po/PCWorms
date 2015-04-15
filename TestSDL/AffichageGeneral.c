#include "AffichageGeneral.h"
#include "input.h"
#include "physique.h"
#include "Sounds.h"
#include "my_stdrFct.h"



int mainFenetre(Jeu * jeu)
{
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE, temps = 0;
	SDL_Renderer * pRenderer = NULL; //déclaration du renderer
	SDL_Window * pWindow = NULL;	//déclaration de la window
	Input * pInput = NULL; //structure contenant les informations relatives aux inputs clavier
	SDL_Texture * display = NULL;	//Texture globale
	SDL_Rect camera = initRect(0, 0, 0, 0); // rect(x,y,w,h)
	int indexTeam = 0;


	//init SDL + fenetre + renderer
	if (initSWR(&pWindow, &pRenderer))
	{
		//Initialisation des inputs
		pInput = initInput();
		if (pInput == NULL)
		{
			fprintf(logFile, "mainFenetre : FAILURE, initInput.\n");
			cleanUp(&pWindow, &pRenderer, &pInput, &display);
			return -1;
		}
		//Initialisation du terrain
		if (initialisionTerrain(&jeu->pMapTerrain, pRenderer, "../assets/pictures/FondMap1.png", jeu->nomMap) < 0)
		{
			fprintf(logFile, "mainFenetre : FAILURE, initialisationTerrain.\n");
			cleanUp(&pWindow, &pRenderer, &pInput, &display);
			return -1;
		}

		//Initialisation de l'affichage
		display = my_createTextureFromSurface(jeu->pMapTerrain->imageMapSurface, pRenderer);
		if (display == NULL)
		{
			fprintf(logFile, "mainFenetre : FAILURE, createGlobalTexture.\n");
			destroyMap(&jeu->pMapTerrain);
			cleanUp(&pWindow, &pRenderer, &pInput, &display);
			return -1;
		}

		//Initialisation de la caméra
		initCameras(pRenderer, jeu->pMapTerrain, &camera, NULL);

		//Initialisation des worms
		//initGameWorms(jeu->equipes[0]->worms, pInput, jeu->pMapTerrain, display, pRenderer, &camera);
		if (loadSounds(BipExplo, 0) < 0)
		{
			fprintf(logFile, "mainFenetre : FAILURE, loadSounds.\n");
			cleanUp(&pWindow, &pRenderer, &pInput, &display);
			return -1;
		}
		while (!(pInput->quit))
		{
			//Récupération des inputs
			getInput(pInput, pWindow);

			//Gestion des inputs
			if (!gestInput(pInput, pRenderer, jeu->pMapTerrain, display, &camera, jeu->equipes[indexTeam]->worms))
			{
				fprintf(logFile, "mainFenetre : FAILURE, gestInput.\n");
			}

			//Update de l'écran
			if (pInput->raffraichissement)
			{
				updateScreen(pRenderer, 2, 0, jeu->pMapTerrain, 1, display, &camera, NULL);
				pInput->raffraichissement = 0;
			}

			//Gestion du frame Rate
			frameRate(frame_max);
			frame_max = SDL_GetTicks() + FRAME_RATE;
			if ((SDL_GetTicks() - temps) >= 1000)
			{
				temps = SDL_GetTicks();
				jeu->temps -= 1;
			}
			if (globalVar.nbEquipe > 1)
			{
				if (indexTeam != globalVar.nbEquipe - 1)
					indexTeam += 1;
				else indexTeam = 0;
			}
		}
		destroyMap(&jeu->pMapTerrain);
		destroyPolice();
	}
	cleanUp(&pWindow, &pRenderer, &pInput, &display);
	fprintf(logFile, "mainFenetre : SUCCESS.\n");
	return 0;
}
int sandboxRenderer()
{
	int closeWindow = 0;
	Point p1, p2;
	int click = 0;
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	SDL_Event event;
	SDL_Renderer* renderer2 = NULL; //déclaration du renderer
	SDL_Window* pWindow2 = NULL;

	/* Initialisation simple */
	if (SDL_VideoInit(NULL) < 0)
	{
		fprintf(logFile, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}

	/* Création de la fenêtre */
	pWindow2 = creerFenetre(1080, 600, "KaamWorms");
	if (pWindow2 == NULL)
	{
		SDL_Quit();
		return -1;
	}

	/* Création du renderer */
	renderer2 = SDL_CreateRenderer(pWindow2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer2 == NULL)//gestion des erreurs
	{
		fprintf(logFile, "Erreur lors de la creation d'un renderer : %s", SDL_GetError());
		return -1;
	}

	SDL_SetRenderDrawColor(renderer2, 210, 50, 60, 255);
	SDL_RenderPresent(renderer2);

	while (!closeWindow)
	{

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				closeWindow = 1;
				break;

			case SDL_MOUSEBUTTONDOWN:
				click = 1;/*Booleen de mémorisation du click*/
				if (event.button.button == SDL_BUTTON_LEFT)/*Test du bouton de la souris*/
					SDL_SetRenderDrawColor(renderer2, 210, 50, 60, 255);
				else if (event.button.button == SDL_BUTTON_RIGHT)
					SDL_SetRenderDrawColor(renderer2, 50, 210, 60, 255);
				else if (event.button.button == SDL_BUTTON_MIDDLE)
					SDL_SetRenderDrawColor(renderer2, 50, 60, 210, 255);
				SDL_GetMouseState(&p2.x, &p2.y); //Initialisationn pour affichage ligne
				afficherPoint(renderer2);
				break;

			case SDL_MOUSEBUTTONUP:
				click = 0;/*Booleen de démémorisation du click*/
				break;

			case SDL_MOUSEMOTION:
				if (click)/*Trace les points en suivant la souris, ne pas aller trop vite*/
				{
					afficherLigne(renderer2, &p1, &p2);
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_c) /*Clear de la fenêtre*/
				{
					clearRenderer(renderer2);
				}
				break;

			default:
				break;
			}
		}
		frameRate(frame_max);
		frame_max = SDL_GetTicks() + FRAME_RATE;
	} //comment

	SDL_DestroyRenderer(renderer2);
	SDL_DestroyWindow(pWindow2);
	//SDL_Quit();
	return 0;
}

/**
* \fn int initSWR(SDL_Window** p_pWindow, SDL_Renderer** p_pRenderer)
* \brief Initialisation de la fenêtre.
*
*La fonction initSWR initialise la SDL, la SDL_Image, la SDL_TTF et les pointeurs
*pWindow ainsi que pRenderer. Elle créé la fenêtre et le renderer associé.
*Elle retourne 1 si tout c'est bien passé, -1 sinon.
*
* \param[in] p_pWindow, adresse du pointeur pWindow pour l'initialiser
* \param[in] p_pRenderer, adresse du pointeur pRenderer pour l'initialiser
* \returns int, indicateur si la fonction a bien fonctionnée.
* \returns les pointeurs pWindow et pRenderer
*/
int initSWR(SDL_Window** p_pWindow, SDL_Renderer** p_pRenderer)
{
	/* Initialisation simple */
	if (SDL_VideoInit(NULL) < 0)
	{
		fprintf(logFile, "initSWR : FAILURE, initialisation de la SDL (%s)\n\n", SDL_GetError());
		return -1;
	}
	/* Création de la fenêtre */
	*p_pWindow = creerFenetre(1080, 600, "KaamWorms");
	if (*p_pWindow == NULL)
	{
		fprintf(logFile, "initSWR : FAILURE, creerFenetre.\n\n");
		return -1;
	}
	{
		SDL_Surface* surfaceIcone = loadImage(ICONE);
		if (surfaceIcone != NULL)
			SDL_SetWindowIcon(*p_pWindow, surfaceIcone);
		SDL_FreeSurface(surfaceIcone);
	};
	/* Création du renderer */
	*p_pRenderer = SDL_CreateRenderer(*p_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (*p_pRenderer == NULL)//gestion des erreurs
	{
		fprintf(logFile, "initSWR : FAILURE, erreur lors de la creation du renderer : %s\n\n", SDL_GetError());
		cleanUp(p_pWindow, NULL, NULL, NULL);
		return -1;
	}
	/*Initialisation SDL_Image*/
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		fprintf(logFile, "initSWR : FAILURE, initialisation de IMG : %s.\n\n", IMG_GetError());
		cleanUp(p_pWindow, p_pRenderer, NULL, NULL);
		return -1;
	}
	/*Initialisation SDL_TTF*/
	if (TTF_Init() == -1)
	{
		fprintf(logFile, "initSWR : FAILURE, initialisation de TTF_Init : %s.\n\n", TTF_GetError());
		return -1;
	}
	if (Mix_Init(MIX_INIT_MP3) && Mix_Init(MIX_INIT_FLAC))
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			fprintf(logFile, "initSWR : FAILURE, initialisation de Mix_Init : %s.\n\n", Mix_GetError());
		}
	}
	fprintf(logFile, "initSWR : SUCCESS.\n\n");
	return 1;
}
/**
* \fn int initSprites(void)
* \brief Init sprites surfaces of the game.
*
* \returns 0 = SUCCESS, -1 = FAILURE to load a sprite
*/
int initSprites(void)
{
	spriteGrenadeExplosion = NULL;
	spriteDeplacement = NULL;
	spriteNukeExplosion = NULL;
	spriteDeplacement = loadImage("../assets/sprites/spriteMov.png");
	if (spriteDeplacement == NULL)
	{
		fprintf(logFile, "initSprites : FAILURE, loadImage.\n\n");
		return -1;
	}
	spriteGrenadeExplosion = loadImage("../assets/sprites/Grenade_Sprite.png");
	if (spriteGrenadeExplosion == NULL)
	{
		fprintf(logFile, "initSprites : FAILURE, loadImage.\n\n");
		cleanSprites();
		return -1;
	}
	spriteNukeExplosion = loadImage("../assets/sprites/Nuke_Sprite.png");
	if (spriteNukeExplosion == NULL)
	{
		fprintf(logFile, "initSprites : FAILURE, loadImage.\n\n");
		cleanSprites();
		return -1;
	}
	fprintf(logFile, "initSprites : SUCCESS.\n\n");
	return 0;
}
/**
* \fn void cleanSprites(void)
* \brief clean sprites surfaces of the game.
*
* \returns void
*/
void cleanSprites(void)
{
	if (spriteDeplacement != NULL)
	{
		SDL_FreeSurface(spriteDeplacement);
		spriteDeplacement = NULL;
	}
	if (spriteGrenadeExplosion != NULL)
	{
		SDL_FreeSurface(spriteGrenadeExplosion);
		spriteGrenadeExplosion = NULL;
	}
	if (spriteNukeExplosion != NULL)
	{
		SDL_FreeSurface(spriteNukeExplosion);
		spriteNukeExplosion = NULL;
	}
	fprintf(logFile, "cleanSprites : DONE.\n");
}
/**
* \fn void cleanUp(SDL_Window** p_pWindow, SDL_Renderer** p_pRenderer, Input** p_pInput, SDL_Texture** p_pTextureDisplay)
* \brief Détruit la fenêtre et le renderer. Libère la mémoire de pInput et quitte la SDL.
*
* \param[in] p_pWindow, adresse du pointeur de la fenêtre.
* \param[in] p_pRenderer, adresse du pointeur du renderer.
* \param[in] p_pInput, adresse du pointeur de la structure Input.
* \param[in] p_pTextureDisplay, adresse de la texture display.
*/
void cleanUp(SDL_Window** p_pWindow, SDL_Renderer** p_pRenderer, Input** p_pInput, SDL_Texture** p_pTextureDisplay)
{
	if ((*p_pInput) != NULL)
	{
		free(*p_pInput);
		(*p_pInput) = NULL;
	}
	if ((*p_pRenderer) != NULL)
	{
		SDL_DestroyRenderer(*p_pRenderer);
		(*p_pRenderer) = NULL;
	}
	if ((*p_pWindow) != NULL)
	{
		SDL_DestroyWindow(*p_pWindow);
		(*p_pWindow) = NULL;
	}
	if ((*p_pTextureDisplay) != NULL)
	{

		SDL_DestroyTexture(*p_pTextureDisplay);
		(*p_pTextureDisplay) = NULL;
	}
	cleanSprites();
	TTF_Quit();
	IMG_Quit();
	cleanSounds();
	Mix_Quit();
	SDL_Quit();
	fprintf(logFile, "cleanUp : DONE.\n");
}
/*
Déplace un rectangle entre les coordonnées précédentes (x2 et y2) de la souris et celles actuelles
*/
void deplacementRectangle(SDL_Rect * rect, int x2, int y2, int dir)
{
	int x1 = 0, y1 = 0;
	SDL_GetMouseState(&x1, &y1);
	if (dir == 1)
	{
		rect->x = rect->x + (x1 - x2);
		rect->y = rect->y + (y1 - y2);
	}
	else if (dir == -1)
	{
		rect->x = rect->x - (x1 - x2);
		rect->y = rect->y - (y1 - y2);
	}
}
//Affiche un point aux coordonnées de la souris
void afficherPoint(SDL_Renderer * r)
{
	static int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_RenderDrawPoint(r, x, y);
	SDL_RenderPresent(r);
}
void afficherLigne(SDL_Renderer * r, Point * p1, Point * p2)
{
	p1->x = p2->x;
	p1->y = p2->y;
	SDL_GetMouseState(&p2->x, &p2->y);
	SDL_RenderDrawLine(r, p1->x, p1->y, p2->x, p2->y);
	SDL_RenderPresent(r);
}
//Clear noir du renderer
void clearRenderer(SDL_Renderer * pRenderer)
{
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(pRenderer);
	SDL_RenderPresent(pRenderer);
}
/**
* \fn void updateScreen(SDL_Renderer * pRenderer, int nb, ...)
* \brief Actualise l'affichage.
*
* Cette fonction est appelée à chaque changement dans l'image pour
* réactuliser l'affichage.
* Elle peut prendre plusieurs types de paramètre pour réaliser des actions
* diverses (affichage de la map, d'un (de) rectangle(s) ou d'une (de) textures).
*
* \param[in] pRenderer, pointeur vers le renderer de la fenêtre
* \param[in] nb, nombre de paramètres dans l'appel
* \remarks Le formatage des paramètres supplémentaire est le suivant :
*	pour le background de la map : 0,Terrain* map
*	pour une texture : 1,Texture* pTexture,SDL_Rect* rectSource, SDL_Rect* rectDest
*	pour remplir un rectangle : 2,Uint32 couleurRemplissage,SDL_Rect* rect à remplir
*
* ATTENTION : Penser a incrementer nb en fonction du nombre de paramètre en plus,
* par exemple : une texture et un rectangle => nb = 2
*/
void updateScreen(SDL_Renderer * pRenderer, int nb, ...)
{
	SDL_Rect* rect = NULL;
	SDL_Rect* rect2 = NULL;
	SDL_Rect temp = initRect(0, 0, 0, 0);
	Terrain * map = NULL;
	SDL_Texture* text = NULL;
	va_list list;
	Uint32 rgb = 0;
	int i = 0, w = 0, h = 0;
	va_start(list, nb);
	for (i = 0; i < nb; i++)
	{
		switch (va_arg(list, int))
		{
		case 0:
			map = va_arg(list, Terrain*);
			SDL_GetRendererOutputSize(pRenderer, &w, &h);
			temp.w = w;
			temp.h = h;
			SDL_RenderCopy(pRenderer, map->imageBackground, NULL, NULL);
			break;
		case 1:
			text = va_arg(list, SDL_Texture*);
			rect = va_arg(list, SDL_Rect*);
			rect2 = va_arg(list, SDL_Rect*);
			SDL_RenderCopy(pRenderer, text, rect, rect2);
			break;
		case 2:
			rgb = va_arg(list, Uint32);
			SDL_SetRenderDrawColor(pRenderer, rgb >> 24, (rgb >> 16) & 0x00FF, (rgb >> 8) & 0x0000FF, (rgb & 0x000000FF));
			SDL_RenderFillRect(pRenderer, va_arg(list, SDL_Rect*));
			break;
		default:
			break;
		}
	}
	map = NULL;
	text = NULL;
	rect = NULL;
	SDL_RenderPresent(pRenderer);
	va_end(list);
}
/**
* \fn void frameRate(unsigned int fM)
* \brief Actualise l'affichage.
*
* Cette fonction est appelée à chaque boucle principale du programme pour
* réguler le frame rate et limiter l'usage du CPU par la fonction getInput.
* \param[in] fM, valeur du temps ecoule lors de la derniere boucle
*/
void frameRate(unsigned int fM)
{
	unsigned int tick = SDL_GetTicks();
	if (fM < tick)
	{
		return;
	}
	else if (fM > tick + FRAME_RATE)
	{
		SDL_Delay(FRAME_RATE);
	}
	else // (fM > tick)
	{
		SDL_Delay(fM - tick);
	}
}
/**
* \fn void initCameras(SDL_Renderer * pRenderer, Terrain * pMapTerrain, SDL_Rect * pCamera, Worms  * pWorms)
* \brief Initialise une camera.
*
* \param[in] pRenderer, le renderer de la fenêtre.
*
* \param[in] pMapTerrain, la texture de la map.
*
* \param[in] pCamera, le rect de la camera à initialiser.
*
* \param[in] pWorms, pointeur du worms sur lequel on se centre
* \returns void
*/
void initCameras(SDL_Renderer * pRenderer, Terrain * pMapTerrain, SDL_Rect * pCamera, Worms  * pWorms){
	int w = 0, h = 0, wW = 0, hW = 0;
	SDL_GetRendererOutputSize(pRenderer, &wW, &hW);
	if (pWorms == NULL){
		w = pMapTerrain->imageMapSurface->w;
		h = pMapTerrain->imageMapSurface->h;
		pCamera->x = 0;
		pCamera->y = 0;
		if (h > hW || w > wW){
			pCamera->h = h;
			pCamera->w = (int)(pCamera->h * ((float)wW / (float)hW));
		}
	}
	else{
		pCamera->h = pWorms->wormsObject->objectSurface->h * 10;
		pCamera->w = pCamera->h * (int)((float)wW / (float)hW);
		pCamera->x = pWorms->wormsObject->absoluteCoordinate.x - (pCamera->w / 2);
		pCamera->y = pWorms->wormsObject->absoluteCoordinate.y - (pCamera->h / 2);
		if (pCamera->x < 0)pCamera->x = 0;
		if (pCamera->y < 0)pCamera->y = 0;
	}
	fprintf(logFile, "initCameras : DONE.\n\n");
}
/**
* \fn void moveCam(SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput,...)
* \brief Déplace la camera suivant la souris.
*
* \param[in] pTexture, la texture de la fenêtre courante.
*
* \param[in] camera, le rect de la camera courante.
*
* \param[in] pInput, les inputs utilisateur.
*
* \returns void
*/
void moveCam(SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput)
{
	int w = 0, h = 0;
	SDL_QueryTexture(pTexture, NULL, NULL, &w, &h);
	camera->x = camera->x - (pInput->cursor.now.x - pInput->cursor.before.x);
	camera->y = camera->y - (pInput->cursor.now.y - pInput->cursor.before.y);
	if (camera->x + camera->w > w){
		camera->x = w - camera->w;
	}
	if (camera->y + camera->h > h){
		camera->y = h - camera->h;
	}
	if (camera->x < 0){
		camera->x = 0;
	}
	if (camera->y < 0){
		camera->y = 0;
	}
}
/**
* \fn void zoomIn(SDL_Renderer * pRenderer, SDL_Rect * camera)
* \brief Réajuste la taille de la caméra pour un grossissement de l'image.
*
* \param[in] pRenderer, le renderer de la fenêtre courante.
*
* \param[in] camera, le rect de la camera courante.
*
* \returns void
*/
void zoomIn(SDL_Renderer * pRenderer, SDL_Texture * pTexture, SDL_Rect * camera, Input * pInput)
{
	int wW = 0, hW = 0, w = 0, h = 0;
	float x = 0, y = 0, offsetx = 0, offsety;
	SDL_GetRendererOutputSize(pRenderer, &wW, &hW);
	SDL_QueryTexture(pTexture, NULL, NULL, &w, &h);

	offsety = (float)camera->h;
	camera->h = camera->h - 10;
	offsety -= camera->h;
	offsetx = (float)camera->w;
	camera->w = (int)(camera->h * ((float)wW / (float)hW));// keep the ratio depending of the size of the window!!!!!
	offsetx -= camera->w;

	x = 2 * (float)((float)(pInput->cursor.now.x / (float)wW) - 0.5);
	y = 2 * (float)((float)(pInput->cursor.now.y / (float)hW) - 0.5);

	if (x > 0.2 || x < -0.2){
		float coefx = (float)(0.03 * camera->w);
		if (coefx < offsetx / 2)coefx = offsetx / 2;
		camera->x += (int)(x * coefx) + (int)(offsetx / 2);
	}
	else camera->x += (int)(offsetx / 2);

	if (y > 0.2 || y < -0.2){
		float coefy = (float)(0.05 * camera->h);
		if (coefy < offsety / 2)coefy = offsety / 2;
		camera->y += (int)(y * coefy) + (int)(offsety / 2);
	}
	else camera->y += (int)(offsety / 2);

	if (camera->x < 0)camera->x = 0;
	if (camera->y < 0)camera->y = 0;
	if (camera->x + camera->w > w) camera->x = w - camera->w;
	if (camera->y + camera->h > h) camera->y = h - camera->h;

}
/**
* \fn void zoomOut(SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect * camera)
* \brief Réajuste la taille de la caméra pour un rétrécissement de l'image.
*
* \param[in] pRenderer, le renderer de la fenêtre courante.
*
* \param[in] pTexture, la texture de la fenêtre courante.
*
* \param[in] camera, le rect de la camera courante.
*
* \returns void
*/
void zoomOut(SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect * camera)
{
	int w = 0, h = 0, wM = 0, hM = 0;
	SDL_GetRendererOutputSize(pRenderer, &w, &h);
	SDL_QueryTexture(pTexture, NULL, NULL, &wM, &hM);
	if (camera->h < hM){
		camera->x = camera->x + (camera->w) / 2;
		camera->y = camera->y + (camera->h) / 2;
		camera->h = camera->h + 20;
		camera->w = (int)(camera->h * ((float)w / (float)h));// keep the ratio depending of the size of the window!!!!!
		if (camera->w > w){
			camera->w = w;
		}
		if (camera->h > h){
			camera->h = h;
		}
		camera->x = camera->x - ((camera->w) / 2);
		camera->y = camera->y - ((camera->h) / 2);
	}

	if (camera->x + camera->w > wM){
		camera->x = wM - camera->w;
	}
	if (camera->y + camera->h > hM){
		camera->y = hM - camera->h;
	}
	if (camera->x < 0){
		camera->x = 0;
	}
	if (camera->y < 0){
		camera->y = 0;
	}
}
/**
* \fn int updateGlobaleTexture(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceModif, SDL_Texture* pTexture, SDL_Rect* pRectSurfaceModif)
* \brief Met a jour la texture globale sur une zone bien determinee.
*
* \param[in] pSurfaceMap, pointeur vers la surface de la map.
* \param[in] pSurfaceModif, pointeur vers la surface a modifier
* \param[in] pTextureDisplay, pointeur vers la texture globale cree avec createGlobalTexture.
* \param[in] pRectSurfaceModif, pointeur vers la structure SDL_Rect associe a la surface a update, contient les coordonnees d'avant update.
* \returns int, indicateur de reussite de la fonction : 0 = succes, -1 = echec
* \remarks le SDL_Rect est mis a jour dans la fonction vers les nouvelles coordonnees de la surface
*/
int updateGlobaleTexture(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceModif, SDL_Texture* pTextureDisplay, SDL_Rect* pRectSurfaceModif)
{
	Uint32* pixelWrite = NULL;
	Uint32 pixelRead;
	int nombrePixel = 0;
	Uint8 r = 0, g = 0, b = 0, a = 0;
	int x = 0, y = 0, i = 0;
	if (limitMap(pSurfaceMap->h, pSurfaceMap->w, pSurfaceModif, NULL))
		return -1;
	nombrePixel = pRectSurfaceModif->w * pRectSurfaceModif->h;
	reajustRect(pRectSurfaceModif, pSurfaceMap);
	pixelWrite = malloc(nombrePixel*sizeof(Uint32));
	if (pixelWrite == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "updateGlobalTexture : FAILURE, allocation memoire pixelWrite.\n\n");
		return -1;
	}
	for (i = 0; i < 2; i++)
	{
		for (y = pRectSurfaceModif->y; y < pRectSurfaceModif->y + pRectSurfaceModif->h; y++)
		{
			for (x = pRectSurfaceModif->x; x < pRectSurfaceModif->x + pRectSurfaceModif->w; x++)
			{
				if (i != 0)
				{
					pixelRead = ReadPixel(pSurfaceModif, x - pRectSurfaceModif->x, y - pRectSurfaceModif->y);
					SDL_GetRGBA(pixelRead, pSurfaceModif->format, &r, &g, &b, &a);
					if (a < 200)
					{
						pixelRead = ReadPixel(pSurfaceMap, x, y);
					}
				}
				else pixelRead = ReadPixel(pSurfaceMap, x, y);
				pixelWrite[x - pRectSurfaceModif->x + (y - pRectSurfaceModif->y)* pRectSurfaceModif->w] = pixelRead;
			}
		}
		SDL_UpdateTexture(pTextureDisplay, pRectSurfaceModif, pixelWrite, 4 * pRectSurfaceModif->w);
		pRectSurfaceModif->y = pSurfaceModif->clip_rect.y;
		pRectSurfaceModif->x = pSurfaceModif->clip_rect.x;
	}
	free(pixelWrite);
	pixelWrite = NULL;
	return 0;
}
/**
* \fn int updateWormsOverlay(Worms** wormsTab, SDL_Texture* pTextureDisplay, SDL_Surface* pSurfaceMap, int indexWorms1, int indexWorms2)
* \brief Update l'affichage de deux worms supperpose .
*
* \param[in] wormsTab, tableau de worms.
* \param[in] pTextureDisplay, pointeur vers la texture globale cree avec createGlobalTexture.
* \param[in] pSurfaceMap, pointeur vers la surface de la map.
* \param[in] indexWorms1, indice du worms numero 1.
* \param[in] indexWorms2, indice du worms numero 2.
* \return error, -1 = FAIL, 0 = SUCCESS
*/
int updateWormsOverlay(Worms** wormsTab, SDL_Texture* pTextureDisplay, SDL_Surface* pSurfaceMap, int indexWorms1, int indexWorms2)
{
	Uint32* pixelWrite = NULL;
	Uint32 pixelRead;
	int nombrePixel = 0;
	SDL_Surface* pSurfaceWorms1 = wormsTab[indexWorms1]->wormsObject->objectSurface;
	SDL_Surface* pSurfaceWorms2 = wormsTab[indexWorms2]->wormsObject->objectSurface;
	SDL_Rect rect;
	int x = 0, y = 0, h = wormsTab[indexWorms1]->wormsObject->objectSurface->h, w = wormsTab[indexWorms1]->wormsObject->objectSurface->w;
	int y0 = wormsTab[indexWorms1]->wormsObject->objectSurface->clip_rect.y, x0 = wormsTab[indexWorms1]->wormsObject->objectSurface->clip_rect.x;
	int y1 = wormsTab[indexWorms2]->wormsObject->objectSurface->clip_rect.y, x1 = wormsTab[indexWorms2]->wormsObject->objectSurface->clip_rect.x;
	int yStart = y1, xEnd = x0 + w, yEnd = y0 + h, xStart = x1;
	if (y1 < y0)
	{
		yStart = y0;
		yEnd = y1 + h;
	}
	if (x1 < x0)
	{
		xStart = x0;
		xEnd = x1 + w;
	}
	rect.x = xStart;
	rect.y = yStart;
	rect.h = (yEnd - yStart);
	rect.w = (xEnd - xStart);
	nombrePixel = rect.w * rect.h;
	pixelWrite = (Uint32*)malloc(nombrePixel * sizeof(Uint32));
	if (pixelWrite == NULL)
	{
		fprintf(logFile, "createWorms : FAILURE, allocating memory to pixelWrite.\n\n");
		return -1;
	}
	for (y = yStart; y < yEnd; y++)
	{
		for (x = xStart; x < xEnd; x++)
		{
			pixelRead = ReadPixel(pSurfaceWorms2, MY_ABS((x - x1)), y - y1);
			if (pixelTransparent(pixelRead, pSurfaceWorms2->format))
			{
				pixelRead = ReadPixel(pSurfaceWorms1, MY_ABS(x - x0), MY_ABS(y - y0));
				if (pixelTransparent(pixelRead, pSurfaceWorms1->format))
				{
					pixelRead = ReadPixel(pSurfaceMap, x, y);
				}
			}
			pixelWrite[(x - xStart) + (y - yStart)* rect.w] = pixelRead;
		}
	}
	SDL_UpdateTexture(pTextureDisplay, &rect, pixelWrite, 4 * rect.w);

	free(pixelWrite);
	pixelWrite = NULL;
	return 0;
}
/**
* \fn int reajustRect(SDL_Rect* pRect, SDL_Surface* pSurfaceMap)
* \brief Réajuste les dimensions d'un rectangle qui depasserai de la map
*
* \param[in] pRect, rectangle a evaluer, peut etre modifie par la fonction
* \param[in] pSurfaceMap, surface de la map
* \return 1 si il y a eu modification, 0 sinon
*/
int reajustRect(SDL_Rect* pRect, SDL_Surface* pSurfaceMap)
{
	int modif = 0;
	if (pRect->x < 0) {
		pRect->w -= pRect->x;
		pRect->x = 0;
		modif = 1;
	}
	if (pRect->y < 0) {
		pRect->h -= pRect->y;
		pRect->y = 0;
		modif = 1;
	}
	if (pRect->x + pRect->w >= pSurfaceMap->clip_rect.w){
		pRect->w -= pRect->x + pRect->w - pSurfaceMap->clip_rect.w;
		modif = 1;
	}
	if (pRect->y + pRect->h >= pSurfaceMap->clip_rect.h){
		pRect->h -= pRect->y + pRect->h - pSurfaceMap->clip_rect.h;
		modif = 1;
	}
	return modif;
}
/**
* \fn void screenshot(SDL_Renderer* pRenderer)
* \brief Make a screenshot of the screen
*
* \param[in] pRenderer, pointer to the renderer of the current window
* \return void
*/
void screenshot(SDL_Renderer* pRenderer)
{
	int w, h;
	int i = 0;
	const char* mainPath = "../assets/screenshot/";
	char path[50];
	char screenshotName[29];
	time_t t1;
	t1 = time(NULL);
	SDL_Surface* surfaceScreenshot = NULL;
	SDL_GetRendererOutputSize(pRenderer, &w, &h);
	surfaceScreenshot = SDL_CreateRGBSurface(0, w, h, 32, RMASK, GMASK, BMASK, AMASK);
	SDL_RenderReadPixels(pRenderer, NULL, SDL_PIXELFORMAT_ABGR8888, surfaceScreenshot->pixels, surfaceScreenshot->pitch);
	strcpy(path, mainPath);
	sprintf(screenshotName, "%s\0", ctime(&t1));
	strcat(path, screenshotName);
	memcpy(path + strlen(mainPath) + 3, "_", 1);
	memcpy(path + strlen(mainPath) + 7, "_", 1);
	for (i = 10; i < 22; i += 3)
		memcpy(path + strlen(mainPath) + i, ".", 1);
	memcpy(path + strlen(mainPath) + strlen(screenshotName) - 1, ".bmp\0", 5);
	SDL_SaveBMP(surfaceScreenshot, path);
	SDL_FreeSurface(surfaceScreenshot);
}
/**
* \fn void centerCam(SDL_Rect * camera,SDL_Surface * surfaceWhereCenter, SDL_Texture* pTexture)
* \brief Center the camera on the surface with actual Zoom
* \param[in] camera, pointer to the camera to be centered
* \param[in] surfaceWhereCenter, pointer to the surface where the camera will be centered
* \param[in] pTexture, pointer texture of the main window
* \return void
*/
void centerCam(SDL_Rect * camera, SDL_Surface * surfaceWhereCenter, SDL_Texture* pTexture){
	int x = 0, y = 0, wM = 0, hM = 0;
	SDL_QueryTexture(pTexture, NULL, NULL, &wM, &hM);

	x = surfaceWhereCenter->clip_rect.x + surfaceWhereCenter->w / 2 - 1;
	y = surfaceWhereCenter->clip_rect.y + surfaceWhereCenter->h / 2 - 1;

	camera->x = x - camera->w / 2;
	camera->y = y - camera->h / 2;

	if (camera->x + camera->w > wM){
		camera->x = wM - camera->w;
	}
	if (camera->y + camera->h > hM){
		camera->y = hM - camera->h;
	}
	if (camera->x < 0){
		camera->x = 0;
	}
	if (camera->y < 0){
		camera->y = 0;
	}
}