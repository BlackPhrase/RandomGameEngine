
#include "Graphics.h"

#include <X11/Xlib.h>
#include "Renderable.h"

#include <algorithm>

#include "utils.h"
#include "logger.h"

CGraphicsEngine::CGraphicsEngine( CXLibWindow &window ):
	m_window(window)
{
	m_graphicsContext = XCreateGC(m_window.GetDisplay(), m_window.GetWindow(), 0, 0);
	
	uint32_t background = WhitePixel( m_window.GetDisplay(), m_window.GetScreen() );
    uint32_t foreground = BlackPixel( m_window.GetDisplay(), m_window.GetScreen() );

	XSetBackground( m_window.GetDisplay(), m_graphicsContext, background );
	XSetForeground( m_window.GetDisplay(), m_graphicsContext, foreground );
}

CGraphicsEngine::~CGraphicsEngine()
{
}

bool CGraphicsEngine::SupportsDBX() const
{
	return false;
}

void CGraphicsEngine::DrawText( float x, float y, const std::string &text ) const
{
	uint_point_t position = {0, 0};
	NormalizedToScreenRes(x, y, position);
	// the height of the default text is 10 or 11 pixels
	XDrawImageString(m_window.GetDisplay(), m_window.GetWindow(), m_graphicsContext, position.m_x, position.m_y + 11, text.c_str(), text.length());
}

void CGraphicsEngine::ClearWindow()
{
	XClearWindow( m_window.GetDisplay(), m_window.GetWindow() );
}

void CGraphicsEngine::BeginFrame()
{
	ClearWindow();
}

void CGraphicsEngine::EndFrame()
{
	if (m_pRenderables)
	{
		std::sort(m_pRenderables->begin(), m_pRenderables->end(), &renderableContext_t::zCompLess);
		for ( renderableContext_t &renderable : *m_pRenderables )
		{
			RenderObject(renderable);
		}
	}
	m_pRenderables = NULL;
	XFlush(m_window.GetDisplay());
}

void CGraphicsEngine::QueueRenderables( const std::shared_ptr< std::vector<renderableContext_t> > &pRenderables )
{
	m_pRenderables = pRenderables;
}

void CGraphicsEngine::NormalizedToScreenRes( float in_x, float in_y, uint_point_t &out ) const
{
	out.m_x = sizzUtil::RoundFlt(in_x * (float)m_window.GetWindowWidth());
	out.m_y = sizzUtil::RoundFlt(in_y * (float)m_window.GetWindowHeight());
}

void CGraphicsEngine::RenderObject( const renderableContext_t &rC )
{
	XDrawRectangle(m_window.GetDisplay(), m_window.GetWindow(), m_graphicsContext, rC.position.m_x, rC.position.m_y, 32, 32);
	sizzLog::LogDebug("rendered object: %, %", rC.position.m_x, rC.position.m_y);
}
