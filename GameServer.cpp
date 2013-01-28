
#include "GameServer.h"
#include "GameEnts.h"
#include "Physics.h"

#include "logger.h"

CGameServer::CGameServer():
	m_pEngine(NULL),
	m_entViewBounds(0),
	m_entHelicopter(0),
	m_flLastBuildingWidth(0.0),
	m_flViewPosOnLastBuildingSpawn(0.0),
	m_flLastFireBullet(0.0),
	m_bGameOver(false),
	m_bViewMoving(false),
	m_bFiringBullets(false)
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
	pView->SetHorizontalScrollSpeed(100.0);
	m_bViewMoving = true;
	
	CHelicopter *pHelicopter = new CHelicopter();
	m_entHelicopter = m_pEngine->CreateEntity(pHelicopter);
	pHelicopter->SetXVelocity(100.0);
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
	std::vector<CEntity*> m_ents;
	{
		m_ents.reserve(m_pEngine->GetNumEntites());
		int i = 0;
		for (int ents_processed = 0; ents_processed < m_pEngine->GetNumEntites();)
		{
			CEntity *pEnt = m_pEngine->GetEntity(i++);
			if (pEnt)
			{
				if (pEnt->IsMarkedForRemoval())
				{
					int index = m_pEngine->IndexOfEnt(pEnt);
					m_pEngine->RemoveEntity(index);
				}
				else
				{
					// remove viewable entities not in the view
					if (pEnt->GetGraphicsComponent() && !IsInViewBounds(pEnt))
					{
						// don't delete things to the right, cause they are just spawning
						if (!IsToTheRightOfViewBounds(pEnt))
						{
							pEnt->MarkForRemoval();
						}
					}
					else
					{
						pEnt->Update(dt);
						m_ents.emplace_back(pEnt);
					}
				}
				++ents_processed;
			}
			if (i > 100)
				break;
		}
	}
	
	for (uint32_t j = 0; j < m_ents.size(); ++j)
	{
		for (uint32_t k = 0; k < m_ents.size(); ++k)
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
	
	// this makes sure the heli doesn't go out of bounds
	if (!m_bGameOver)
	{
		point_2d_t pos = GetHelicopter()->GetPhysComponent()->Get2DPosition();
		point_2d_t heli_size = GetHelicopter()->GetPhysComponent()->GetAABBSize();
		point_2d_t view_pos = GetViewBoundsEnt()->GetPhysComponent()->Get2DPosition();
		point_2d_t view_pos_max = GetViewBoundsEnt()->GetPhysComponent()->GetAABBSize();
		view_pos_max.m_x += view_pos.m_x;
		view_pos_max.m_y += view_pos.m_y;
		if (pos.m_x < view_pos.m_x)
		{
			pos.m_x = view_pos.m_x;
		}
		if (pos.m_y < view_pos.m_y)
		{
			pos.m_y = view_pos.m_y;
		}
		if (pos.m_x + heli_size.m_x > view_pos_max.m_x)
		{
			pos.m_x = view_pos_max.m_x - heli_size.m_x;
		}
		if (pos.m_y + heli_size.m_y > view_pos_max.m_y)
		{		
			pos.m_y = view_pos_max.m_y - heli_size.m_y;
		}
		GetHelicopter()->SetPosition(pos.m_x, pos.m_y);
		
		// do checks for firing and stuff
		CheckHeliFireBullet();
		CheckSpawnBuilding();
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
		static const double heli_speed = 200.0;
		sizzLog::LogDebug("got command: %", command);
		if (command == "+up")
		{
			GetHelicopter()->IncreaseYVelocity(-heli_speed);
		}
		else if (command == "-up")
		{
			GetHelicopter()->IncreaseYVelocity(heli_speed);
		}
		else if (command == "+down")
		{
			GetHelicopter()->IncreaseYVelocity(heli_speed);
		}
		else if (command == "-down")
		{
			GetHelicopter()->IncreaseYVelocity(-heli_speed);
		}
		else if (command == "+left")
		{
			GetHelicopter()->IncreaseXVelocity(-heli_speed);
		}
		else if (command == "-left")
		{
			GetHelicopter()->IncreaseXVelocity(heli_speed);
		}
		else if (command == "+right")
		{
			GetHelicopter()->IncreaseXVelocity(heli_speed);
		}
		else if (command == "-right")
		{
			GetHelicopter()->IncreaseXVelocity(-heli_speed);
		}
		else if (command == "+shoot")
		{
			//GetHelicopter()->StartShooting();
			m_bFiringBullets = true;
		}
		else if (command == "-shoot")
		{
			//GetHelicopter()->StopShooting();
			m_bFiringBullets = false;
		}
	}
}

void CGameServer::SpawnBullet( CBullet *pBullet )
{
	if (pBullet)
	{
		m_pEngine->CreateEntity(pBullet);
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
		else if (obj2 == k_eTurret)
		{
			pEnt1->MarkForRemoval();
			pEnt2->MarkForRemoval();
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
		else if (obj2 == k_eTurret)
		{
			pEnt1->MarkForRemoval();
			pEnt2->MarkForRemoval();
			GameEnd();
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
		else if (obj2 == k_eTurret)
		{
			// don't care
		}
	}
	else if (obj1 == k_eTurret)
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
			// don't care
		}
		else if (obj2 == k_eViewBounds)
		{
			// don't care
		}
		else if (obj2 == k_eTurret)
		{
			 //don't care
		}
	}
}

void CGameServer::CheckSpawnBuilding()
{
	if (m_bViewMoving)
	{
		// these are in the x domain
		C2DViewBounds *pView = GetViewBoundsEnt();
		const CPhysicsComponent *pViewPhys = pView->GetPhysComponent();
		double cur_view_pos = pViewPhys->Get2DPosition().m_x;
		
		static int32_t rand_distance = sizzUtil::Rand_Bounded(0.0, 50.0);
		if ((m_flViewPosOnLastBuildingSpawn + m_flLastBuildingWidth + rand_distance) <= cur_view_pos)
		{
			rand_distance = sizzUtil::Rand_Bounded(0.0, 50.0);
			m_flLastBuildingWidth = sizzUtil::Rand_Bounded(16.0, 70.0);
			m_flViewPosOnLastBuildingSpawn = cur_view_pos;
			
			CBuilding *pBuilding = new CBuilding();
			m_pEngine->CreateEntity(pBuilding);
			int32_t height = sizzUtil::Rand_Bounded(150, 430.0);
			int32_t building_pos = cur_view_pos + pViewPhys->GetAABBSize().m_x;
			
			pBuilding->SetPosition(building_pos, height);
			pBuilding->SetXVelocity(0.0);
			pBuilding->SetYVelocity(0.0);
			pBuilding->SetSize(m_flLastBuildingWidth, pView->GetFov().m_y);
			
			CTurret *pTurret = new CTurret(this);
			pTurret->SetPosition( (double)building_pos + m_flLastBuildingWidth/10, height - 10.0);
			pTurret->SetSize(5.0, 10.0);
			m_pEngine->CreateEntity(pTurret);
			
		}
	}
}

void CGameServer::CheckHeliFireBullet()
{
	if (m_bFiringBullets)
	{
		double cur_time = m_pEngine->GetEngineTime();
		if (m_flLastFireBullet + 0.1 <= cur_time)
		{
			m_flLastFireBullet = m_pEngine->GetEngineTime();
			const CPhysicsComponent *pPhys = GetHelicopter()->GetPhysComponent();
			point_2d_t heli_pos = pPhys->Get2DPosition();
			point_2d_t heli_vel = pPhys->GetVelocity();
			point_2d_t heli_aabb = pPhys->GetAABBSize();
			
			CBullet *pBullet = new CBullet();
			pBullet->SetPosition(heli_pos.m_x + heli_aabb.m_x, heli_pos.m_y + 40);
			pBullet->SetXVelocity(heli_vel.m_x);
			double y_vel = (heli_vel.m_y > 0) ? heli_vel.m_y + 90 : 90;
			pBullet->SetYVelocity(y_vel);
			m_pEngine->CreateEntity(pBullet);
		}
	}
}

bool CGameServer::IsToTheRightOfViewBounds( CEntity *pEnt )
{
	if (pEnt && pEnt->GetPhysComponent())
	{
		C2DViewBounds *pView = GetViewBoundsEnt();
		if (pView != pEnt)
		{
			const CPhysicsComponent *pPhysView = pView->GetPhysComponent();
			const CPhysicsComponent *pPhysEnt = pEnt->GetPhysComponent();
			
			double ent_x = pPhysEnt->Get2DPosition().m_x;
			double view_x_max = pPhysView->Get2DPosition().m_x + pPhysView->GetAABBSize().m_x;
			
			return ent_x > view_x_max;
		}
	}
	return false;
}
