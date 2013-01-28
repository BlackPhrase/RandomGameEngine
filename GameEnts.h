
#ifndef GAME_ENTS_H
#define GAME_ENTS_H

#include "Entity.h"
#include "Physics.h"
#include "Renderable.h"

enum EObjectType
{
	k_eBullet = 0,
	k_eLocalPlayer,
	k_eBuilding,
	k_eViewBounds,
	k_eTurret,
	k_eNumObjectTypes
};

class CEntInfo
{
public:
	CEntInfo( EObjectType type ):
		m_objType(type)
	{
	}
	
	EObjectType GetObjType() const
	{
		return m_objType;
	}
	
private:
	EObjectType m_objType;
};

class C2DViewBounds: public CEntity
{
public:
	C2DViewBounds():
		m_entInfo(k_eViewBounds)
	{
		point_2d_t size = {480.0, 480.0};
		m_physics.SetAABBSize(size);
	}
	
	virtual void Update( double dt )
	{
		m_physics.Update(dt);
	}
	
	virtual const CPhysicsComponent *GetPhysComponent() const
	{
		return &m_physics;
	}
	
	virtual const CEntInfo *GetInfo() const
	{
		return &m_entInfo;
	}
	
	void SetHorizontalScrollSpeed( double units )
	{
		m_physics.SetXVelocity(units);
	}
	
	void SetHorizontalFov( double units )
	{
		point_2d_t out = m_physics.GetAABBSize();
		out.m_x = units;
		m_physics.SetAABBSize(out);
	}
	
	void SetVerticalFov( double units )
	{
		point_2d_t out = m_physics.GetAABBSize();
		out.m_y = units;
		m_physics.SetAABBSize(out);
	}
	
	point_2d_t GetFov() const
	{
		return m_physics.GetAABBSize();
	}
	
private:
	CPhysicsComponent m_physics;
	CEntInfo m_entInfo;
};

class CBuilding: public CEntity
{
public:
	CBuilding():
		m_entInfo(k_eBuilding)
	{
		m_graphics.SetColour("grey");
	}
	
	virtual void Update( double dt )
	{
		m_physics.Update(dt);
	}
	
	virtual const CPhysicsComponent *GetPhysComponent() const
	{
		return &m_physics;
	}
	
	virtual const CGraphicsComponent *GetGraphicsComponent() const
	{
		return &m_graphics;
	}
	
	virtual const CEntInfo *GetInfo() const
	{
		return &m_entInfo;
	}
	
	void SetXVelocity( double vel )
	{
		m_physics.SetXVelocity(vel);
	}
	
	void SetYVelocity( double vel )
	{
		m_physics.SetYVelocity(vel);
	}
	
	void SetSize(double x, double y)
	{
		m_graphics.SetRectangle({{0.0, 0.0}, {x, y}});
		m_physics.SetAABBSize({x, y});
	}
	
	void SetPosition(double x, double y)
	{
		m_physics.Set2DPosition({x, y});
	}
	
private:
	CPhysicsComponent m_physics;
	CGraphicsComponent m_graphics;
	CEntInfo m_entInfo;
};

class CHelicopter: public CEntity
{
public:
	CHelicopter():
		m_entInfo(k_eLocalPlayer)
	{
		m_graphics.SetColour("blue");
	}
	
	virtual const CPhysicsComponent *GetPhysComponent() const
	{
		return &m_physics;
	}
	
	virtual const CGraphicsComponent *GetGraphicsComponent() const
	{
		return &m_graphics;
	}
	
	virtual const CEntInfo *GetInfo() const
	{
		return &m_entInfo;
	}
	
	virtual void Update( double dt )
	{
		m_physics.Update(dt);
	}
	
	void SetXVelocity( double vel )
	{
		m_physics.SetXVelocity(vel);
	}
	
	void SetYVelocity( double vel )
	{
		m_physics.SetYVelocity(vel);
	}
	
	void IncreaseXVelocity( double vel )
	{
		point_2d_t cur = m_physics.GetVelocity();
		m_physics.SetXVelocity(cur.m_x + vel);
	}
	
	void IncreaseYVelocity( double vel )
	{
		point_2d_t cur = m_physics.GetVelocity();
		m_physics.SetYVelocity(cur.m_y + vel);
	}
	
	void SetPosition(double x, double y)
	{
		m_physics.Set2DPosition({x, y});
	}
	
	void SetSize(double x, double y)
	{
		m_graphics.SetArc({{0.0, 0.0}, {x, y}});
		m_physics.SetAABBSize({x, y});
	}
	
private:
	CPhysicsComponent m_physics;
	CGraphicsComponent m_graphics;
	CEntInfo m_entInfo;
};

class CBullet: public CEntity
{
public:
	CBullet():
		m_entInfo(k_eBullet)
	{
		m_graphics.SetColour("red");
		SetSize(5.0, 5.0);
		
		m_physics.SetYAccel(150.0);
	}
	
	virtual const CPhysicsComponent *GetPhysComponent() const
	{
		return &m_physics;
	}
	
	virtual const CGraphicsComponent *GetGraphicsComponent() const
	{
		return &m_graphics;
	}
	
	virtual const CEntInfo *GetInfo() const
	{
		return &m_entInfo;
	}
	
	virtual void Update( double dt )
	{
		m_physics.Update(dt);
	}
	
	void SetXVelocity( double vel )
	{
		m_physics.SetXVelocity(vel);
	}
	
	void SetYVelocity( double vel )
	{
		m_physics.SetYVelocity(vel);
	}
	
	void SetPosition(double x, double y)
	{
		m_physics.Set2DPosition({x, y});
	}
	
	void SetSize(double x, double y)
	{
		m_graphics.SetArc({{0.0, 0.0}, {x, y}});
		m_physics.SetAABBSize({x, y});
	}
	
private:
	CPhysicsComponent m_physics;
	CGraphicsComponent m_graphics;
	CEntInfo m_entInfo;
};

class CTurret: public CEntity
{
public:
	CTurret():
		m_entInfo(k_eTurret)
	{
		m_graphics.SetColour("brown");
	}
	
	virtual const CPhysicsComponent *GetPhysComponent() const
	{
		return &m_physics;
	}
	
	virtual const CGraphicsComponent *GetGraphicsComponent() const
	{
		return &m_graphics;
	}
	
	virtual const CEntInfo *GetInfo() const
	{
		return &m_entInfo;
	}
	
	virtual void Update( double dt )
	{
		m_physics.Update(dt);
	}
	
	void SetXVelocity( double vel )
	{
		m_physics.SetXVelocity(vel);
	}
	
	void SetYVelocity( double vel )
	{
		m_physics.SetYVelocity(vel);
	}
	
	void SetPosition(double x, double y)
	{
		m_physics.Set2DPosition({x, y});
	}
	
	void SetSize(double x, double y)
	{
		m_graphics.SetRectangle({{0.0, 0.0}, {x, y}});
		m_physics.SetAABBSize({x, y});
	}
	
private:
	CPhysicsComponent m_physics;
	CGraphicsComponent m_graphics;
	CEntInfo m_entInfo;
};

#endif // GAME_ENTS_H