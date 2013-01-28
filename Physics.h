
#ifndef PHYSICS_H
#define PHYSICS_H

#include "mathutil.h"
#include "Entity.h"
#include "logger.h"
#include <cassert>

class CPhysicsComponent
{
public:
	CPhysicsComponent()
	{
		m_position = {0.0, 0.0, 0.0};
		m_velocity = {0.0, 0.0};
		m_accel = {0.0, 0.0};
		m_AABBsize = {0.0, 0.0};
	}
	
	void Update( double dt )
	{
		m_velocity.m_x += (m_accel.m_x * dt);
		m_velocity.m_y += (m_accel.m_y * dt);
		
		m_position.m_x = m_position.m_x + (m_velocity.m_x * dt);
		m_position.m_y = m_position.m_y + (m_velocity.m_y * dt);
	}
	
	point_3d_t GetPosition() const	{ return m_position; }
	point_2d_t Get2DPosition() const { return {m_position.m_x, m_position.m_y}; }
	void SetPosition( const point_3d_t &pos )	{ m_position = pos; }
	void Set2DPosition( const point_2d_t &pos )
	{
		m_position.m_x = pos.m_x;
		m_position.m_y = pos.m_y;
	}
	
	void SetXVelocity( double vel )		{ m_velocity.m_x = vel; }
	void SetYVelocity( double vel )		{ m_velocity.m_y = vel; }
	
	point_2d_t GetVelocity() const
	{
		return m_velocity;
	}
	
	void SetYAccel( double accel )	{ m_accel.m_y = accel; }
	
	void SetAABBSize( const point_2d_t &AABBsize ) { m_AABBsize = AABBsize; }
	
	point_2d_t GetAABBSize() const
	{
		return m_AABBsize;
	}
	
private:
	point_3d_t m_position;
	// units/second
	point_2d_t m_velocity;
	point_2d_t m_accel;
	point_2d_t m_AABBsize;
};

namespace Physics
{
	static point_3d_t GetPosition( const CEntity *pEntity )
	{
		if (pEntity)
		{
			const CPhysicsComponent *pPhys = pEntity->GetPhysComponent();
			if (pPhys)
			{
				return pPhys->GetPosition();
			}
		}
		return {0.0, 0.0, 0.0};
	}
	
	static bool IsAABBColliding( const CPhysicsComponent &a, const CPhysicsComponent &b )
	{
		point_2d_t rect1_min = a.Get2DPosition();
		point_2d_t rect1_max = { rect1_min.m_x + a.GetAABBSize().m_x, rect1_min.m_y + a.GetAABBSize().m_y };
		point_2d_t rect2_min = b.Get2DPosition();
		point_2d_t rect2_max = { rect2_min.m_x + b.GetAABBSize().m_x, rect2_min.m_y + b.GetAABBSize().m_y };
		
		assert( rect1_min.m_x <= rect1_max.m_x );
		assert( rect1_min.m_y <= rect1_max.m_y );
		assert( rect2_min.m_x <= rect2_max.m_x );
		assert( rect2_min.m_y <= rect2_max.m_y );
		
		if ( (rect1_min.m_x > rect2_max.m_x) || (rect1_max.m_x < rect2_min.m_x) )
		{
			return false;
		}
		if ( (rect1_min.m_y > rect2_max.m_y) || (rect1_max.m_y < rect2_min.m_y) )
		{
			return false;
		}
		return true;
	}
}

#endif // PHYSICS_H
