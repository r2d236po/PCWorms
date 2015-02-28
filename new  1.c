int mainFenetre2()
{
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	SDL_Renderer* pRenderer = NULL; //déclaration du renderer
	SDL_Window* pWindow = NULL;	//déclaration de la window
	Input * pInput = NULL; //structure contenant les informations relatives aux inputs clavier
	Terrain * mainMap = NULL;
	SDL_Texture* display = NULL;	//Texture globale
	SDL_Surface** surfaceTab = NULL;
	Worms * worms1 = NULL; //worms
	SDL_Rect camera = { 0, 0, 0, 0 }; // rect(x,y,w,h)
	SDL_Rect camera2 = { 0, 0, 0, 0 }; //Camera 2 NE DOIS JAMAIS ZOOMER

	//init SDL + fenetre + renderer
	if (initSWR(&pWindow, &pRenderer))
	{
		//Initialisation des inputs
		pInput = initInput();
		if (pInput == NULL)
		{
			printf("Erreur lors de l'allocation de pInput");
			cleanUp(&pWindow, &pRenderer, &pInput);
			return -1;
		}

		//Initialisation du terrain
		if (initialisionTerrain(&mainMap, pRenderer, "../assets/pictures/fond2.png", "../assets/pictures/map.png") < 0)
		{
			printf("Probleme lors de la creation du terrain");
			cleanUp(&pWindow, &pRenderer, &pInput);
			return -1;
		}

		//Initialisation worms
		worms1 = createWorms(pRenderer, "../assets/pictures/worms.png");
		if (worms1 == NULL)
		{
			printf("Erreur creation worms");
			destroyMap(&mainMap);
			cleanUp(&pWindow, &pRenderer, &pInput);
			return -1;
		}
		surfaceTab = malloc(2 * sizeof(SDL_Surface*));

		surfaceTab[0] = mainMap->mapCollision;
		surfaceTab[1] = worms1->wormsSurface;

		//Initialisation des caméras
		initCameras(pRenderer, mainMap, &camera);
		camera2.w = 1080;
		camera2.h = 600;

		//Initialisation de l'affichage
		if (createGlobalTexture(surfaceTab, 2, &display, pRenderer, pWindow, &camera) < 0)
		{
			printf("Erreur creation de la texture globale");
			destroyWorms(&worms1);
			destroyMap(&mainMap);
			cleanUp(&pWindow, &pRenderer, &pInput);
			return -1;

		}

		while (!(pInput->quit))
		{
			//Récupération des inputs
			getInput(pInput, pWindow);

			//Gestion des inputs
			if (!gestInput(pInput, pRenderer, mainMap, &camera2, display, &camera, worms1))
			{
				printf("Erreur lors du traitement de l'entree");
			}

			//Update de l'écran
			if (pInput->raffraichissement)
			{
				updateGlobaleTexture(surfaceTab, display, 1, &worms1->wormsRect);
				updateScreen(pRenderer, &camera2, &mainMap->mapCollision,2, 0, mainMap, 1, display, &camera, NULL);
			}

			//Gestion du frame Rate
			frameRate(frame_max);
			frame_max = SDL_GetTicks() + FRAME_RATE;
		}
		free(surfaceTab);
		surfaceTab = NULL;
		destroyMap(&mainMap);
		destroyWorms(&worms1);
		SDL_DestroyTexture(display);
		display = NULL;
		cleanUp(&pWindow, &pRenderer, &pInput);
	}
	IMG_Quit();
	SDL_Quit();
	return 0;
}



//met à jour le zoom de la texture globale
int updateCamera(SDL_Renderer* pRenderer, SDL_Rect* camera, SDL_Window* pWindow, SDL_Texture** pTexture)
{
	SDL_Texture* textureTemp = NULL;
	SDL_Surface* surfaceTemp = SDL_GetWindowSurface(pWindow);
	SDL_Rect rect = { 0, 0, 0, 0 };
	int h = 0, w = 0;
	unsigned char* pixels = NULL;
	SDL_GetRendererOutputSize(pRenderer, &w, &h);
	rect.w = w;
	rect.h = h;
	if (surfaceTemp == NULL)
	{
		printf("Failed to create info surface from window in saveScreenshotBMP(string) %s", SDL_GetError());
		SDL_FreeSurface(surfaceTemp);
		return -1;
	}
	pixels = malloc(w * h * surfaceTemp->format->BytesPerPixel * sizeof(int));
	if (pixels == 0)
	{
		printf("Unable to allocate memory for screenshot pointeur data buffer!\n ");
		pixels = NULL;
		return -1;
	}
	if (SDL_RenderReadPixels(pRenderer, NULL, surfaceTemp->format->format, pixels, w * surfaceTemp->format->BytesPerPixel) != 0)
	{
		printf("Failed to read pointeur data from SDL_Renderer object");
		free(pixels);
		pixels = NULL;
		return -1;
	}
	if (*pTexture == NULL)
	{
		textureTemp = SDL_CreateTexture(pRenderer, surfaceTemp->format->format, SDL_TEXTUREACCESS_STREAMING, w, h);
		if (textureTemp == NULL)
		{
			printf("Erreur lors de la creation de la texture");
			SDL_FreeSurface(surfaceTemp);
			surfaceTemp = NULL;
			free(pixels);
			pixels = NULL;
			return -1;
		}
		SDL_SetTextureBlendMode(textureTemp, SDL_BLENDMODE_BLEND);
		SDL_UpdateTexture(textureTemp, NULL, pixels, surfaceTemp->pitch);
		*pTexture = textureTemp;
		textureTemp = NULL;
	}
	else
	{
		SDL_UpdateTexture(*pTexture, NULL, pixels, surfaceTemp->pitch);
	}
	SDL_RenderCopy(pRenderer, *pTexture, camera, &rect);
	SDL_RenderPresent(pRenderer);
	SDL_FreeSurface(surfaceTemp);
	surfaceTemp = NULL;
	free(pixels);
	pixels = NULL;
	SDL_RenderPresent(pRenderer);
	return 0;
}



//Rescale de la surface
SDL_Surface* crop_surface(SDL_Surface* sprite_sheet, int x, int y, int width, int height)
{
	SDL_Surface* surfaceTemp = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect rect = { 0, 0, 0, 0 };
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	surfaceTemp = SDL_CreateRGBSurface(sprite_sheet->flags, width, height,
		sprite_sheet->format->BitsPerPixel,
		sprite_sheet->format->Rmask,
		sprite_sheet->format->Gmask,
		sprite_sheet->format->Bmask,
		sprite_sheet->format->Amask);

	if (surfaceTemp == NULL)
	{
		printf("Erreur lors de la creation de la surface");
		return NULL;
	}
	SDL_BlitSurface(sprite_sheet, &rect, surfaceTemp, 0);
	surface = surfaceTemp;
	SDL_FreeSurface(surfaceTemp);
	surfaceTemp = NULL;
	return surface;
}

SDL_Surface* ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
	Sint32 y = 0, x = 0, o_y = 0, o_x = 0;
	if (!Surface || !Width || !Height)
		return 0;
	SDL_Surface * ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
		Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);
	SDL_Surface* surface = NULL;

	double  stretch_factor_x = ((double)(Width) / (double)(Surface->w));
	double	stretch_factor_y = ((double)(Height) / (double)(Surface->h));

	for (y = 0; y < Surface->h; y++)
	{
		for (x = 0; x < Surface->w; x++)
		{
			for (o_y = 0; o_y < stretch_factor_y; ++o_y)
			{
				for (o_x = 0; o_x < stretch_factor_x; ++o_x)
				{
					DrawPixel(ret, (Sint32)(stretch_factor_x * x) + o_x, (Sint32)(stretch_factor_y * y) + o_y, ReadPixel(Surface, x, y));
				}
			}
		}
	}
	surface = ret;
	SDL_FreeSurface(ret);
	return surface;
}