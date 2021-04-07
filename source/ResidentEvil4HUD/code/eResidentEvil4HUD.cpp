#include "eResidentEvil4HUD.h"
#include "..\manhunt\core.h"
#include "..\manhunt\Frontend.h"
#include "..\manhunt\Renderer.h"
#include "..\manhunt\Entity.h"
#include "..\manhunt\Scene.h"
#include "..\manhunt\Time.h"
#include "..\manhunt\Inventory.h"
#include "..\manhunt\Collectable.h"
int eResidentEvil4HUD::pTXD;
int eResidentEvil4HUD::pHealthTexture[21];
int eResidentEvil4HUD::pNumberTexture[10];
int eResidentEvil4HUD::pBulletTexture[TOTAL_BULLET_TEXTURES];
int eResidentEvil4HUD::pEmptyTexture;
int eResidentEvil4HUD::pInsideTexture;
int eResidentEvil4HUD::pNameTexture;
int eResidentEvil4HUD::pLifebarTexture;
CRGBA eResidentEvil4HUD::lifebarColor;
CRGBA eResidentEvil4HUD::infoColor;
CRGBA eResidentEvil4HUD::ballColor;
int  eResidentEvil4HUD::iLifebarColorTimer;
bool eResidentEvil4HUD::bInvertUpdate;
int  eResidentEvil4HUD::iLoadedAmmo;
int  eResidentEvil4HUD::iTotalAmmo;
bool eResidentEvil4HUD::bNoAmmoLeft;

void eResidentEvil4HUD::LoadResources()
{
	iTotalAmmo = -1;
	iLoadedAmmo = -1;
	lifebarColor = { 10,48,10,255};
	infoColor = { 40, 255, 255,255 };
	ballColor = { 210, 255, 255, 160 };
	iLifebarColorTimer = CGameTime::ms_currGameTime;
	bInvertUpdate = false;

	pTXD =  CallAndReturn<int, 0x5EA510, const char*>("pictures/re4h.txd");

	if (!pTXD)
		MessageBoxA(0, "Failed to load re4h.txd!", 0, MB_ICONERROR);

	if (pTXD)
	{
		char tmp[100];
		pLifebarTexture = CFrontend::GetTextureFromTXD(pTXD, "back");
		pNameTexture = CFrontend::GetTextureFromTXD(pTXD, "name");
		pEmptyTexture = CFrontend::GetTextureFromTXD(pTXD, "empty");
		pInsideTexture = CFrontend::GetTextureFromTXD(pTXD, "inside");
		pBulletTexture[BULLET_NORMAL] = CFrontend::GetTextureFromTXD(pTXD, "i_bullet");
		pBulletTexture[BULLET_GRENADE] = CFrontend::GetTextureFromTXD(pTXD, "i_grenade");
		pBulletTexture[BULLET_SHOTGUN] = CFrontend::GetTextureFromTXD(pTXD, "i_shotgun");
		pBulletTexture[BULLET_RIFLE] = CFrontend::GetTextureFromTXD(pTXD, "i_sniper");

		for (int i = 0; i < 21; i++)
		{
			sprintf(tmp, "h%02d", (5 * i));
			pHealthTexture[i] = CFrontend::GetTextureFromTXD(pTXD, tmp);
		}
		for (int i = 0; i < 10; i++)
		{
			sprintf(tmp, "%d", i);
			pNumberTexture[i] = CFrontend::GetTextureFromTXD(pTXD, tmp);
		}
	}

}




void eResidentEvil4HUD::DrawLifeBar(float posx, float posY, float scalex, float scaley, int r, int g, int b, int a, int pTexture)
{

	Memory::VP::Patch<int>(0x7C9C80, 1);

	if (iTotalAmmo == 0 && iLoadedAmmo == 0)
		bNoAmmoLeft = true;
	else
		bNoAmmoLeft = false;

	UpdateLifeColor();


	if (IsPlayerHoldingAFirearm())
	{
		int first = iLoadedAmmo % 10;
		int second = (iLoadedAmmo / 10) % 10;

		if (!bNoAmmoLeft)
		{
			DrawNumber(0.83f, 0.815f, first);
			DrawNumber(0.805f, 0.815f, second);

			if (IsPlayerHoldingNormalFirearm())
				CRenderer::DrawQuad2d(0.795f, 0.76, 0.055, 0.078, infoColor.r, infoColor.g, infoColor.b, infoColor.a, pBulletTexture[BULLET_NORMAL]);

			if (IsPlayerHoldingShotgun())
				CRenderer::DrawQuad2d(0.795f, 0.76, 0.055, 0.078, infoColor.r, infoColor.g, infoColor.b, infoColor.a, pBulletTexture[BULLET_SHOTGUN]);

			if (CGameInventory::GetCurrentItem() == CT_SNIPER_RIFLE)
				CRenderer::DrawQuad2d(0.795f, 0.76, 0.055, 0.078, infoColor.r, infoColor.g, infoColor.b, infoColor.a, pBulletTexture[BULLET_RIFLE]);
		}

	
	}


	if (bNoAmmoLeft && IsPlayerHoldingAFirearm())
	{
		CRenderer::DrawQuad2d(0.77, 0.748, 0.111, 0.189, 210, 255, 255, 160, pInsideTexture);
		CRenderer::DrawQuad2d(0.77, 0.748, 0.111, 0.189, 255, 255, 255, 200, pEmptyTexture);
	}
	else
		CRenderer::DrawQuad2d(0.77, 0.748, 0.111, 0.189, ballColor.r, ballColor.g, ballColor.b, ballColor.a, pInsideTexture);

	CRenderer::DrawQuad2d(0.75, 0.73, 0.24, 0.24, lifebarColor.r, lifebarColor.g, lifebarColor.b, 255, pHealthTexture[GetHealthTexture()]);


	CRenderer::DrawQuad2d(0.75,0.73,0.24, 0.24, 255, 255, 255, 255, pLifebarTexture);

	CRenderer::DrawQuad2d(0.89, 0.905, 0.075, 0.072, 255, 255, 255, 255, pNameTexture);
}

int eResidentEvil4HUD::GetHealthTexture()
{
	int id = 20;

	CEntity* plr = CScene::FindPlayer();

	if (plr->m_fHealth >= 100.0f)
		id = 20;
	if (plr->m_fHealth < 95.0f)
		id--;
	if (plr->m_fHealth < 90.0f)
		id--;
	if (plr->m_fHealth < 85.0f)
		id--;
	if (plr->m_fHealth < 80.0f)
		id--;
	if (plr->m_fHealth < 75.0f)
		id--;
	if (plr->m_fHealth < 70.0f)
		id--;
	if (plr->m_fHealth < 65.0f)
		id--;
	if (plr->m_fHealth < 60.0f)
		id--;
	if (plr->m_fHealth < 55.0f)
		id--;
	if (plr->m_fHealth < 50.0f)
		id--;
	if (plr->m_fHealth < 45.0f)
		id--;
	if (plr->m_fHealth < 40.0f)
		id--;
	if (plr->m_fHealth < 35.0f)
		id--;
	if (plr->m_fHealth < 30.0f)
		id--;
	if (plr->m_fHealth < 25.0f)
		id--;
	if (plr->m_fHealth < 20.0f)
		id--;
	if (plr->m_fHealth < 15.0f)
		id--;
	if (plr->m_fHealth < 10.0f)
		id--;
	if (plr->m_fHealth < 5.0f)
		id--;
	if (plr->m_fHealth < 0.0f)
		id = 0;
	return id;
}

void eResidentEvil4HUD::UpdateLifeColor()
{
	if (CGameTime::ms_currGameTime - iLifebarColorTimer <= 20) return;
	iLifebarColorTimer = CGameTime::ms_currGameTime;



	if (!bInvertUpdate)
	{

		lifebarColor.g++;
		if (lifebarColor.g > 75)
		{
			lifebarColor.g = 75;
			bInvertUpdate = true;
		}
	}
	else
	{
		lifebarColor.g--;
		if (lifebarColor.g < 48)
		{
			lifebarColor.g = 48;
			bInvertUpdate = false;
		}
	}

}

void eResidentEvil4HUD::ResetTimer()
{
	iLifebarColorTimer = 0;
}

void eResidentEvil4HUD::DrawNumber(float x, float y, int number)
{
	CRenderer::DrawQuad2d(x, y, 0.035,0.1, infoColor.r,infoColor.g, infoColor.b,infoColor.a, pNumberTexture[number]);
}

void eResidentEvil4HUD::StoreLoadedAmmoCount(char *, char *, int count)
{
	iLoadedAmmo = count;

}

void eResidentEvil4HUD::StoreTotalAmmoCount(char *, char *, int count)
{
	iTotalAmmo = count;
}

bool eResidentEvil4HUD::IsPlayerHoldingAFirearm()
{
	switch (CGameInventory::GetCurrentItem())
	{
	case CT_GLOCK:
	case CT_DESERT_EAGLE:
	case CT_6SHOOTER:
	case CT_TRANQ_RIFLE:
	case CT_SHOTGUN:
	case CT_NAILGUN:
	case CT_SHOTGUN_TORCH:
	case CT_SAWNOFF:
	case CT_UZI:
	case CT_COLT_COMMANDO:
	case CT_SNIPER_RIFLE:
	case CT_GLOCK_SILENCED:
	case CT_GLOCK_TORCH:
		return true;
	}

	return false;
}

bool eResidentEvil4HUD::IsPlayerHoldingShotgun()
{
	switch (CGameInventory::GetCurrentItem())
	{
	case CT_SHOTGUN:
	case CT_SHOTGUN_TORCH:
	case CT_SAWNOFF:
		return true;
	}

	return false;
}

bool eResidentEvil4HUD::IsPlayerHoldingNormalFirearm()
{
	switch (CGameInventory::GetCurrentItem())
	{
	case CT_GLOCK:
	case CT_DESERT_EAGLE:
	case CT_6SHOOTER:
	case CT_TRANQ_RIFLE:
	case CT_UZI:
	case CT_COLT_COMMANDO:
	case CT_GLOCK_SILENCED:
	case CT_GLOCK_TORCH:
		return true;
	}

	return false;
}
