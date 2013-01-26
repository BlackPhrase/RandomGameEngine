
#ifndef GAME_ENTS_H
#define GAME_ENTS_H

#include "Entity.h"
#include "Physics.h"
#include "Renderable.h"

class C2DViewBounds: public CEntity
{
public:
	C2DViewBounds()
	{
		point_2d_t size = {480.0, 480.0};
		m_physics.SetAABBSize(size);
	}
	
	virtual void Update( double dt )
	{
		m_physics.Update(dt);
	}
	
	virtual const CGraphicsComponent *GetGraphicsComponent() const
	{
		return &m_graphics;
	}
	
	virtual const CPhysicsComponent *GetPhysComponent() const
	{
		return &m_physics;
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
	CGraphicsComponent m_graphics;
};

class CBuilding: public CEntity
{
public:
	CBuilding() {}
	~CBuilding() {}
	
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
		m_physics.SetAABBSize({x, y});
	}
	
	void SetPosition(double x, double y)
	{
		m_physics.Set2DPosition({x, y});
	}
	
private:
	CPhysicsComponent m_physics;
	CGraphicsComponent m_graphics;
};

class CHelicopter: public CEntity
{
public:
	CHelicopter() {}
	~CHelicopter() {}
	
	virtual const CPhysicsComponent *GetPhysComponent() const
	{
		return &m_physics;
	}
	
	virtual const CGraphicsComponent *GetGraphicsComponent() const
	{
		return &m_graphics;
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
	
private:
	CPhysicsComponent m_physics;
	CGraphicsComponent m_graphics;
};

#endif // GAME_ENTS_H