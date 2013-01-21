
#include "Entity.h"

CEntity::CEntity():
	m_bIsMarkedForDeletion(false)
{
	Reset();
}

CEntity::~CEntity()
{
}

void CEntity::MarkForDeletion()
{
	m_bIsMarkedForDeletion = true;
}

void CEntity::Reset()
{
	m_bIsMarkedForDeletion = false;
}

bool CEntity::IsMarkedForDeletion() const
{
	return m_bIsMarkedForDeletion;
}