
#include "Graphics.h"

#include <X11/Xlib.h>
#include "Renderable.h"

#include <algorithm>

#include "utils.h"
#include "logger.h"
#include <cmath>

CGraphicsEngine::CGraphicsEngine( CXLibWindow &window ):
	m_window(window)
{
	m_graphicsContext = XCreateGC(m_window.GetDisplay(), m_window.GetBackBuff(), 0, NULL);
	
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
	return m_window.SupportsDBE();
}

void CGraphicsEngine::DrawText( float x, float y, const std::string &text )
{
	uint_point_t position = {0, 0};
	NormalizedToScreenRes(x, y, position);
	// the height of the default text is 10 or 11 pixels
	text_t t = {position.m_x, position.m_y + 11, text};
	m_textQueue.emplace_back( t );
}

void CGraphicsEngine::ClearWindow()
{
	XClearWindow( m_window.GetDisplay(), m_window.GetWindow() );
	
}

void CGraphicsEngine::BeginFrame()
{
	//ClearWindow();
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
	
	SetActiveColour(GetColour("black"));
	for ( text_t &t : m_textQueue )
	{
		XDrawImageString(m_window.GetDisplay(), m_window.GetBackBuff(), m_graphicsContext, t.x, t.y, t.text.c_str(), t.text.length());
	}
	m_textQueue.clear();
	// this causes terrible flickering
	m_window.SwapBuffers();
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
	SetActiveColour(GetColour(rC.gcomp.GetColour()));
	EShape shape = rC.gcomp.GetShape();
	switch (shape)
	{
		case k_ePolygon:
			{
			}
			break;
		case k_eRectangle:
			{
				const rectangle_t *pRect = rC.gcomp.GetRectangle();
				// bad cast from uint64_t to uint32_t
				int32_t x = sizzUtil::RoundDBL(floor(rC.position.m_x + pRect->m_min.m_x));
				int32_t y = sizzUtil::RoundDBL(floor(rC.position.m_y + pRect->m_min.m_y));
				int32_t width = sizzUtil::RoundDBL(ceil(pRect->m_max.m_x - pRect->m_min.m_x));
				int32_t height = sizzUtil::RoundDBL(ceil(pRect->m_max.m_y - pRect->m_min.m_y));
				XFillRectangle(m_window.GetDisplay(), m_window.GetBackBuff(), m_graphicsContext,
					x, y, width, height);
			}
			break;
		case k_eArc:
			{
				const arc_t *pArc = rC.gcomp.GetArc();
				XFillArc(m_window.GetDisplay(), m_window.GetBackBuff(),
					m_graphicsContext,
					sizzUtil::RoundDBL(ceil(rC.position.m_x + pArc->m_originOffset.m_x)),
					sizzUtil::RoundDBL(ceil(rC.position.m_y + pArc->m_originOffset.m_y)),
					sizzUtil::RoundDBL(ceil(pArc->m_size.m_x)),
					sizzUtil::RoundDBL(ceil(pArc->m_size.m_y)),
					0, 360*64);
			}
			break;
		default:
			break;
	}
	sizzLog::LogDebug("rendered object: %, %", rC.position.m_x, rC.position.m_y);
}

uint32_t CGraphicsEngine::GetColour( const char *colour )
{
	Colormap map = DefaultColormap( m_window.GetDisplay(), 0 );
	XColor near_color;
	XColor true_color;
	XAllocNamedColor( m_window.GetDisplay(), map, colour, &near_color, &true_color );
	
	return near_color.pixel;
}

void CGraphicsEngine::SetActiveColour( uint32_t colour )
{
	XSetForeground( m_window.GetDisplay(), m_graphicsContext, BlackPixel(m_window.GetDisplay(), 0)^colour);
}
