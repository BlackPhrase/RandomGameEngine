
#ifndef GAME_ENTS_H
#define GAME_ENTS_H

#include "Entity.h"
#include "Physics.h"
#include "Renderable.h"

class C2DViewBounds: public CEntity
{
public:
	C2DViewBounds():
		m_verticalFov(400.0),
		m_horizontalFov(300.0)
	{
		point_2d_t size = {300.0, 400.0};
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
	
	void SetHorizontalFov( double units )	{ m_horizontalFov = units; }
	void SetVerticalFov( double units )		{ m_verticalFov = units; }
	
	void GetFov( double &vert_fov, double &horiz_fov ) const
	{
		vert_fov = m_verticalFov;
		horiz_fov = m_horizontalFov;
	}
	
private:
	CPhysicsComponent m_physics;
	CGraphicsComponent m_graphics;
	double m_verticalFov;
	double m_horizontalFov;
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
	
	void SetSize(double x, double y)
	{
		m_physics.SetAABBSize({x, y});
	}
	
private:
	CPhysicsComponent m_physics;
	CGraphicsComponent m_graphics;
};

#endif // GAME_ENTS_H