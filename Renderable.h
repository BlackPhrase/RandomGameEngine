
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <vector>
#include <X11/Xlib.h>
#include "mathutil.h"
#include <cassert>

enum EShape
{
	k_ePolygon = 0,
	k_eArc,
	k_eRectangle,
	k_eNumShapes
};

typedef struct polygon_s
{
	std::vector<XPoint> *m_points;
} polygon_t;

typedef struct arc_s
{
	point_2d_t m_originOffset;
	uint32_t m_radius;
} arc_t;

typedef struct rectangle_s
{
	point_2d_t m_min;
	point_2d_t m_max;
} rectangle_t;

class CGraphicsComponent
{
public:
	CGraphicsComponent():
		m_shape(k_eNumShapes)
	{
	}
	
	~CGraphicsComponent()
	{
		if (m_shape == k_ePolygon)
		{
			delete m_data.m_polygon.m_points;
		}
	}

	EShape GetShape() const
	{
		return m_shape;
	}
	
	const polygon_t *GetPolygon() const
	{
		return &m_data.m_polygon;
	}
	
	void SetPolygon( const polygon_t &polygon )
	{
		assert(polygon.m_points);
		m_shape = k_ePolygon;
		m_data.m_polygon = polygon;
	}
	
	const rectangle_t *GetRectangle() const
	{
		return &m_data.m_rect;
	}
	
	void SetRectangle( const rectangle_t &rect )
	{
		if (m_shape == k_ePolygon)
		{
			delete m_data.m_polygon.m_points;
		}
		m_shape = k_eRectangle;
		m_data.m_rect = rect;
	}
	
	const arc_t *GetArc() const
	{
		return &m_data.m_arc;
	}
	
	void SetArc( const arc_t &arc )
	{
		if (m_shape == k_ePolygon)
		{
			delete m_data.m_polygon.m_points;
		}
		m_shape = k_eArc;
		m_data.m_arc = arc;
	}
	
private:
	EShape m_shape;
	union shapeInfo
	{
		polygon_t	m_polygon;
		rectangle_t	m_rect;
		arc_t		m_arc;
	} m_data;
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