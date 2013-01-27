
#ifndef ENTITY_H
#define ENTITY_H

#include <string.h>

class CPhysicsComponent;
class CGraphicsComponent;
class CEntInfo;

class CEntity
{
public:
	CEntity():
		m_bMarkedForRemoval(false)
	{
	}
	
	virtual ~CEntity() {}
	
	virtual void Update( double dt ) = 0;
	
	virtual const CPhysicsComponent *GetPhysComponent() const
	{
		return NULL;
	}
	
	virtual const CGraphicsComponent *GetGraphicsComponent() const
	{
		return NULL;
	}
	
	virtual const CEntInfo *GetInfo() const = 0;
	
	bool IsMarkedForRemoval() const
	{
		return m_bMarkedForRemoval;
	}
	
	void MarkForRemoval()
	{
		m_bMarkedForRemoval = true;
	}
	
private:
	bool m_bMarkedForRemoval;
};

#endif // ENTITY_H
