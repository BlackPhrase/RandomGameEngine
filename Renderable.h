
#ifndef RENDERABLE_H
#define RENDERABLE_H

class CGraphicsComponent
{
};

typedef struct renderableContext_s
{
	static bool zCompLess( const struct renderableContext_s &left, const struct renderableContext_s &right )
	{
		return (left.position.m_z < right.position.m_z);
	}
	
	point_3d_t position;
	CGraphicsComponent gcomp;
} renderableContext_t;



#endif // RENDERABLE_H