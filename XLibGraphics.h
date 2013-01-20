
#ifndef XLIB_GRAPHICS_H
#define XLIB_GRAPHICS_H

#include <queue>
#include <stdint.h>
#include <memory>

#include <X11/Xlib.h>

class CGraphicsComponent
{
public:
	CGraphicsComponent();
	~CGraphicsComponent();
	
	Pixmap GetPixmap() const;
	
	void SetDepth( uint8_t depth ) { m_zdepth = depth; }
	uint8_t GetDepth() const { return m_zdepth; }
	
private:
	uint8_t m_zdepth;
};

class CXLibGraphics
{
	friend class CGame;
public:
	// add a drawable item to be rendered next frame
	//void EnqueueDrawable( const std::shared_ptr<CDrawable> &pDrawable );
	
protected:
	// this function should be called once per frame by the game
	void Render();
	
private:
	//std::priority_queue< std::shared_ptr<CDrawable> > m_displayList;
};

#endif // XLIB_GRAPHICS_H
