
#include "XLibGraphics.h"

void CXLibGraphics::EnqueueDrawable( const std::shared_ptr<CDrawable> &pDrawable )
{
	m_displayList.push(pDrawable);
}
	
void CXLibGraphics::Render()
{
	while (!m_displayList.empty())
	{
		const std::shared_ptr<CDrawable> &pDrawable = m_displayList.top();
		m_displayList.pop();
		pDrawable->Draw(*this);
	}
}