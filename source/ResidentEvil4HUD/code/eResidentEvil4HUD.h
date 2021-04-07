#pragma once
#include "..\manhunt\Frontend.h"

enum eBulletTextureType {
	BULLET_NORMAL,
	BULLET_SHOTGUN,
	BULLET_RIFLE,
	BULLET_GRENADE,
	TOTAL_BULLET_TEXTURES
};

class eResidentEvil4HUD {
private:
	static int pTXD;
	static int pHealthTexture[21];
	static int pNumberTexture[10];
	static int pBulletTexture[TOTAL_BULLET_TEXTURES];
	static int pEmptyTexture;
	static int pInsideTexture;
	static int pNameTexture;
	static int pLifebarTexture;
	
	static CRGBA lifebarColor;
	static CRGBA infoColor;
	static CRGBA ballColor;
	static int iLifebarColorTimer;
	static bool bInvertUpdate;
	static int  iLoadedAmmo;
	static int  iTotalAmmo;
	static bool bNoAmmoLeft;
public:
	static void LoadResources();
	static void	DrawLifeBar(float posx, float posY, float scalex, float scaley, int r, int g, int b, int a, int pTexture);
	static int  GetHealthTexture();
	static void UpdateLifeColor();
	static void ResetTimer();
	static void DrawNumber(float x, float y, int number);
	static void StoreLoadedAmmoCount(char*, char*, int count);
	static void StoreTotalAmmoCount(char*, char*, int count);
	static bool IsPlayerHoldingAFirearm(); 
	static bool IsPlayerHoldingShotgun();
	static bool IsPlayerHoldingNormalFirearm();
};