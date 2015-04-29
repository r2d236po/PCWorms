#ifndef KAAMENGINE_H
#define KAAMENGINE_H

/*Dependencies*/
#include "KaamEngineDependencies.h"

/*KaamEngine V1.0*/
void KaamEngine();

/*Inits*/
int KaamInitGame(Worms** wormsTab, SDL_Surface* pSurfaceMap);
KaamObject* KaamInitObject(SDL_Rect rectSurface, float initSpeedX, float initSpeedY, enum DIRECTION initDirection, int weapon);
void KaamInitSurfaceObject(KaamObject* pObject, Uint32* pixels, Uint32 nbPixels);
void KaamDestroyObject(KaamObject** p_pObject);

/*Physic Management*/
void KaamPhysicManagement(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap);
void KaamGravityManagement(SDL_Surface* pSurfaceMap, KaamObject* pObject);
void KaamMotionManagement(Input* pInput, KaamObject* object);
void KaamWormsMotionManagement(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap);

/*Motion Management*/
void KaamGroundMotionReset(Input* pInput, KaamObject* pObject);
void KaamGroundCollisionProcess(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap, int deplacement);
void KaamGroundMotion(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap);
void KaamNonLinearMotion(Input* pInput, SDL_Surface* pSurfaceMap, KaamObject* pObject, int allowRebound);

/*Collision Management*/
int globalMotionPossibility(KaamObject* pObject, SDL_Surface* pSurfaceMap, enum DIRECTION direction);
void KaamCollisionManagement(Input* pInput, SDL_Surface* pSurfaceMap, KaamObject* pObject, enum DIRECTION directionBeforeCollision, int allowRebound);
int KaamCollisionReaction(KaamObject* pObject, enum DIRECTION directionBeforeCollision, int allowRebound);

#endif // !KAAMENGINE_H
