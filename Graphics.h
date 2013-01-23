
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "XLibWindow.h"
#include "mathutil.h"

#include <string>

class CGraphicsEngine
{
public:
	CGraphicsEngine( CXLibWindow &window );
	~CGraphicsEngine();
	
	bool		SupportsDBX() const;
	
	void		DrawText( float x, float y, const std::string &text ) const;
	
	void		ClearWindow();
	
	void		BeginFrame();
	void		EndFrame();
	
private:
	// converts the 0.0f-1.0f range of x,y to a uint_point_t in terms of pixels on the screen
	void		NormalizedToScreenRes( float in_x, float in_y, uint_point_t &out ) const;
	
private:
	CXLibWindow &m_window;
	GC m_graphicsContext;
};

#endif // GRAPHICS_H
