
#ifndef XLIB_GRAPHICS_H
#define XLIB_GRAPHICS_H

#include <queue>
#include <stdint.h>
#include <memory>

class CXLibGraphics;

class CDrawable
{
public:
	virtual void Draw( const CXLibGraphics &device ) = 0;
	
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
	void EnqueueDrawable( const std::shared_ptr<CDrawable> &pDrawable );
	
protected:
	// this function should be called once per frame by the game
	void Render();
	
private:
	std::priority_queue< std::shared_ptr<CDrawable> > m_displayList;
};

#endif // XLIB_GRAPHICS_H
