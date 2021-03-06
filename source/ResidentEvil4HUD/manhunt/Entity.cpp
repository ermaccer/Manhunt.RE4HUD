#include "Entity.h"
#include "core.h"



void CEntity::Spawn(CVector * pos, float arg)
{
	CallMethod<0x4342E0, int,CVector*, float>((int)this, pos, arg);
}

void CEntity::SetFrozen(int state)
{
	CallMethod<0x436160, int,int>((int)this, state);
}

void CEntity::SetName(char * name)
{
	CallMethod<0x4313D0, CEntity*, char*>(this, name);
}

void CEntity::SetLocation(CVector * pos)
{
	(*((void(__thiscall **)(CEntity *, CVector *, int))this->vTable + 15))(this,pos,1);
}

void CEntity::SetFlag(int flag, int status)
{
	if (status)
		this->m_iEntityFlags |= flag;
	else
		this->m_iEntityFlags &= ~flag;

}

CVector * CEntity::GetLocation()
{
	return CallMethodAndReturn<CVector*,0x4317E0, CEntity*>(this);
}
