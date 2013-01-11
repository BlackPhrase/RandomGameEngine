
#ifndef XLIB_GRAPHICS_CONTEXT_H
#define XLIB_GRAPHICS_CONTEXT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class CXLibGraphicsContext
{
public:
	CXLibGraphicsContext(Display *pDisplay, const Window &window);
	
private:
	Display *m_pDisplay;
	GC m_GraphicsContext;
};

inline CXLibGraphicsContext::CXLibGraphicsContext(Display *pDisplay, const Window &window):
	m_pDisplay(pDisplay),
	m_GraphicsContext(XCreateGC(m_pDisplay, window, 0, NULL))
{
}

#endif // XLIB_GRAPHICS_CONTEXT_H
