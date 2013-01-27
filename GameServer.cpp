
#include "GameServer.h"
#include "GameEnts.h"
#include "Physics.h"

#include "logger.h"

CGameServer::CGameServer():
	m_pEngine(NULL),
	m_entViewBounds(0),
	m_entHelicopter(0)
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
	m_bGameOver = false;
	C2DViewBounds *pView = GetViewBoundsEnt();
	// set view position here too...
	pView->SetHorizontalScrollSpeed(50.0);
	
	CBuilding *pBuilding = new CBuilding();
	m_pEngine->CreateEntity(pBuilding);
	pBuilding->SetPosition(100.0, 100.0);
	pBuilding->SetXVelocity(0.0);
	pBuilding->SetYVelocity(0.0);
	pBuilding->SetSize(32.0, 32.0);
	
	CHelicopter *pHelicopter = new CHelicopter();
	m_entHelicopter = m_pEngine->CreateEntity(pHelicopter);
	pHelicopter->SetXVelocity(50.0);
	pHelicopter->SetPosition(20.0, 20.0);
	pHelicopter->SetSize(32.0, 32.0);
}

void CGameServer::GameEnd()
{
	m_bGameOver = true;
	C2DViewBounds *pView = GetViewBoundsEnt();
	pView->SetHorizontalScrollSpeed(0.0);
}

// called each frame by the engine
void CGameServer::GameFrame( double dt )
{
	uint32_t num = m_pEngine->GetNumEntites();
	std::vector<CEntity*> m_ents;
	m_ents.reserve(num);
	int i = 0;
	while (num)
	{
		CEntity *pEnt = m_pEngine->GetEntity(i++);
		if (pEnt)
		{
			pEnt->Update(dt);
			m_ents.emplace_back(pEnt);
			--num;
		}
	}
	
	for (int j = 0; j < m_ents.size(); ++j)
	{
		for (int k = 0; k < m_ents.size(); ++k)
		{
			if (j != k)
			{
				CEntity *pEnt1 = m_ents[j];
				CEntity *pEnt2 = m_ents[k];
				// handling the collision could change the marked for removal status
				if (!pEnt1->IsMarkedForRemoval() && !pEnt2->IsMarkedForRemoval() && IsColliding(pEnt1, pEnt2))
				{
					HandleCollision(pEnt1, pEnt2);
				}
			}
		}
	}
	
	num = m_pEngine->GetNumEntites();
	i = 0;
	while (num)
	{
		CEntity *pEnt = m_pEngine->GetEntity(i++);
		if (pEnt)
		{
			if (pEnt->IsMarkedForRemoval())
			{
				int index = m_pEngine->IndexOfEnt(pEnt);
				m_pEngine->RemoveEntity(index);
			}
			--num;
		}
	}
	
	//sizzLog::LogDebug( "view at: %", GetViewBoundsEnt()->GetPhysComponent()->GetPosition().m_x );
}

bool CGameServer::IsInViewBounds( CEntity *pEntity )
{
	if (pEntity)
	{
		C2DViewBounds *pView = GetViewBoundsEnt();
		if (pView != pEntity)
		{
			return IsColliding(pEntity, pView);
		}
	}
	return false;
}

void CGameServer::ReceiveCommand( const std::string &command )
{
	if (!m_bGameOver)
	{
		sizzLog::LogDebug("got command: %", command);
		if (command == "+up")
		{
			GetHelicopter()->IncreaseYVelocity(-100.0);
		}
		else if (command == "-up")
		{
			GetHelicopter()->IncreaseYVelocity(100.0);
		}
		else if (command == "+down")
		{
			GetHelicopter()->IncreaseYVelocity(100.0);
		}
		else if (command == "-down")
		{
			GetHelicopter()->IncreaseYVelocity(-100.0);
		}
		else if (command == "+left")
		{
			GetHelicopter()->IncreaseXVelocity(-100.0);
		}
		else if (command == "-left")
		{
			GetHelicopter()->IncreaseXVelocity(100.0);
		}
		else if (command == "+right")
		{
			GetHelicopter()->IncreaseXVelocity(100.0);
		}
		else if (command == "-right")
		{
			GetHelicopter()->IncreaseXVelocity(-100.0);
		}
		else if (command == "+shoot")
		{
			//GetHelicopter()->StartShooting();
		}
		else if (command == "-shoot")
		{
			//GetHelicopter()->StopShooting();
		}
	}
}

C2DViewBounds *CGameServer::GetViewBoundsEnt()
{
	return dynamic_cast<C2DViewBounds*>(m_pEngine->GetEntity(m_entViewBounds));
}

CHelicopter *CGameServer::GetHelicopter()
{
	return dynamic_cast<CHelicopter*>(m_pEngine->GetEntity(m_entHelicopter));
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

void CGameServer::HandleCollision( CEntity *pEnt1, CEntity *pEnt2 )
{
	const CEntInfo *pInfo1 = pEnt1->GetInfo();
	const CEntInfo *pInfo2 = pEnt2->GetInfo();
	
	EObjectType obj1 = pInfo1->GetObjType();
	EObjectType obj2 = pInfo2->GetObjType();
	
	if (obj1 == k_eBullet)
	{
		if (obj2 == k_eBullet)
		{
			pEnt1->MarkForRemoval();
			pEnt2->MarkForRemoval();
			// spawn explosion?
		}
		else if (obj2 == k_eLocalPlayer)
		{
			pEnt1->MarkForRemoval();
			pEnt2->MarkForRemoval();
			// game over, spawn explosion?
			GameEnd();
		}
		else if (obj2 == k_eBuilding)
		{
			pEnt1->MarkForRemoval();
			// spawn explosion?
		}
	}
	else if (obj1 == k_eLocalPlayer)
	{
		if (obj2 == k_eBullet)
		{
			pEnt1->MarkForRemoval();
			pEnt2->MarkForRemoval();
			// game over, spawn explosion?
			GameEnd();
		}
		else if (obj2 == k_eLocalPlayer)
		{
			sizzLog::LogError("localplayer collided with localplayer???");
			assert(false);
		}
		else if (obj2 == k_eBuilding)
		{
			pEnt1->MarkForRemoval();
			// game over, spawn explosion?
			GameEnd();
		}
		else if (obj2 == k_eViewBounds)
		{
			// don't care
		}
	}
	else if (obj1 == k_eBuilding)
	{
		if (obj2 == k_eBullet)
		{
			pEnt2->MarkForRemoval();
			// spawn explosion?
		}
		else if (obj2 == k_eLocalPlayer)
		{
			pEnt2->MarkForRemoval();
			// game over, spawn explosion?
			GameEnd();
		}
		else if (obj2 == k_eBuilding)
		{
			// don't care
		}
		else if (obj2 == k_eViewBounds)
		{
			// don't care
		}
	}
}
