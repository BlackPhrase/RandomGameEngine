
#include "GameServer.h"
#include "GameEnts.h"
#include "Physics.h"

#include "logger.h"

CGameServer::CGameServer():
	m_pEngine(NULL)
{
}

CGameServer::~CGameServer()
{
}

uint32_t CGameServer::Init( IEngineServer *pEngine )
{
	m_pEngine = pEngine;
	m_entViewBounds = pEngine->CreateEntity( new C2DViewBounds() );
	return m_entViewBounds;
}

void CGameServer::Shutdown()
{
}

void CGameServer::GameStart()
{
	C2DViewBounds *pView = GetViewBoundsEnt();
	// set view position here too...
	pView->SetHorizontalScrollSpeed(10.0);
	
	CBuilding *pBuilding = new CBuilding();
	m_pEngine->CreateEntity(pBuilding);
	pBuilding->SetPosition(100.0, 100.0);
	pBuilding->SetXVelocity(20.0);
	pBuilding->SetYVelocity(0.0);
	pBuilding->SetSize(32.0, 32.0);
}

void CGameServer::GameEnd()
{
	C2DViewBounds *pView = GetViewBoundsEnt();
	pView->SetHorizontalScrollSpeed(0.0);
}

// called each frame by the engine
void CGameServer::GameFrame( double dt )
{
	uint32_t num = m_pEngine->GetNumEntites();
	int i = 0;
	while (num)
	{
		CEntity *pEnt = m_pEngine->GetEntity(i++);
		if (pEnt)
		{
			pEnt->Update(dt);
			--num;
		}
	}
	sizzLog::LogDebug( "view at: %", GetViewBoundsEnt()->GetPhysComponent()->GetPosition().m_x );
}

bool CGameServer::IsInViewBounds( CEntity *pEntity )
{
	if (pEntity)
	{
		C2DViewBounds *pView = GetViewBoundsEnt();
		//if (pView != pEntity)
		{
			return IsColliding(pEntity, pView);
		}
	}
	return false;
}

C2DViewBounds *CGameServer::GetViewBoundsEnt()
{
	return dynamic_cast<C2DViewBounds*>(m_pEngine->GetEntity(m_entViewBounds));
}

bool CGameServer::IsColliding( const CEntity *pEnt1, const CEntity *pEnt2 )
{
	if (pEnt1 && pEnt2)
	{
		if ( pEnt1->GetPhysComponent() && pEnt2->GetPhysComponent() )
		{
			bool colliding = Physics::IsAABBColliding(*(pEnt1->GetPhysComponent()), *(pEnt2->GetPhysComponent()));
			return colliding;
		}
	}
	return false;
}
