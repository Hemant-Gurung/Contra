#include "pch.h"
#include "Hud.h"

Hud::Hud(const Point2f& topleft, int totalLives)
	:m_pLifeTexture{new Texture("Resources/Life.png")}
	,m_TotalLives{totalLives}
	,m_TopLeft{topleft}
	,m_CurrentLives{}
	
{
	m_ClipHeight = m_pLifeTexture->GetHeight();
	m_ClipWidth = m_pLifeTexture->GetWidth();
}

Hud::~Hud()
{
	delete m_pLifeTexture;
}

void Hud::Draw()
{
	for (int i = 0;i < m_TotalLives; i++)
	{
		m_pLifeTexture->Draw(Rectf(m_TopLeft.x, m_TopLeft.y, m_ClipWidth+5, m_ClipHeight+10), Rectf(0, m_ClipHeight, m_ClipWidth, m_ClipHeight));
		m_TopLeft.x += 20.f;
	}
}

void Hud::Update(const Point2f& topleft)
{
	m_TopLeft = topleft;
}

void Hud::UpdateLives(const int lives)
{

	if (lives >m_CurrentLives )
	{
		m_CurrentLives = lives;
		m_TotalLives--;
	}
	if (m_TotalLives <= 0)
	{
		m_CurrentLives = 0;
	}
	
}
void Hud::ResetNumberOfLives(const int lives)
{
	m_TotalLives = lives;
}
int Hud::GetNumberOfRemainingLives() const
{
	return m_TotalLives;
}
