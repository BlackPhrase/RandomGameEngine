
#ifndef ENTITY_H
#define ENTITY_H

#include <string.h>

class CPhysicsComponent;
class CGraphicsComponent;

class CEntity
{
public:
	CEntity() {}
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
};

#endif // ENTITY_H
