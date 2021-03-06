#pragma once
#include "Entity.h"


class CScene {
public:
	static bool& ms_bFreeCam;
	// also known as freeze world
	static int&  ms_stepMode;
	static CEntity*& ms_pPlayer;
	static CEntity*& ms_pCamera;
	static void OpenLevel(const char* level);
	static CEntity* FindPlayer();
	static void PlayerFullBodyAnimDone();
};