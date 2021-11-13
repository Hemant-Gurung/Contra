#include "pch.h"
#include"Bridge.h"

Bridge::Bridge(Point2f pos, std::string path)
	:m_BridgePos{pos}
	,m_filePath{path}
	,m_pBridgeTexture{}
	,m_BridgePieces{3}
	,m_AvaratPos{}
	,m_CurrFrame{}
	,m_AccuSec{}
	, m_StartExplosion{ false }
	,m_pBridgeExplodingSound{new SoundEffect("Resources/Sounds/explode-2.mp3")}
{
	m_AnimeTime = 1 / float(m_BridgePieces);
	m_pBridgeTexture = new Texture(m_filePath);
	m_pExplodingTexture = new Texture("Resources/Bridge/explosion.png");
	m_ClipHeight = m_pBridgeTexture->GetHeight();
	m_ClipWidth = m_pBridgeTexture->GetWidth();
}

Bridge::~Bridge()
{
	delete m_pBridgeTexture;
	m_pBridgeTexture = nullptr;

	delete m_pExplodingTexture;
	m_pExplodingTexture = nullptr;

	delete m_pBridgeExplodingSound;
	m_pBridgeExplodingSound = nullptr;
}

void Bridge::Draw()
{
	if (!m_StartExplosion && !m_IsDetroyed)
	{
		m_pBridgeTexture->Draw(m_DestRect, Rectf(0, m_pBridgeTexture->GetHeight(), m_pBridgeTexture->GetWidth(), m_pBridgeTexture->GetHeight()));

	}
	
		
		if (m_StartExplosion && !m_IsDetroyed)
		{
			m_DestRect.left =m_BridgePos.x  ;
			m_DestRect.bottom = m_BridgePos.y;
			m_DestRect.width =m_ClipWidth;
			m_DestRect.height =m_ClipHeight;
			m_pExplodingTexture->Draw(m_DestRect, Rectf((m_pExplodingTexture->GetWidth() / 3) * m_CurrFrame, m_pExplodingTexture->GetHeight(), (m_pExplodingTexture->GetWidth()/3), m_pExplodingTexture->GetHeight()));
		}
	}

void Bridge::Update(float elapsedSec)
{
	if (m_AvaratPos.left > 0 && m_BridgePos.x - m_AvaratPos.left <= 1)
	{
		m_StartExplosion = true;
	}
	m_DestRect.left = m_BridgePos.x;
	m_DestRect.bottom = m_BridgePos.y;
	m_DestRect.height = m_pBridgeTexture->GetHeight();
	m_DestRect.width = m_pBridgeTexture->GetWidth();

	if (m_StartExplosion)
	{
		m_AccuSec += elapsedSec;
		if (m_AccuSec >= m_AnimeTime*0.1f)
		{
			m_AccuSec = 0;
			if (m_CurrFrame >= 3)
			{
				m_CurrFrame = 0;
				if (!m_IsDetroyed)
				{
					m_IsDetroyed = true;
					m_pBridgeExplodingSound->Play(0);
				}
			}
			m_CurrFrame++;
		}
	}
}


void Bridge::SetAvatarPos(const Rectf& avatarPos) 
{
	m_AvaratPos = avatarPos;
}

bool Bridge::GetIsDestroyed() const
{
	return m_IsDetroyed;
}

Rectf Bridge::GetBridgePos() const 
{
	return Rectf(m_BridgePos.x, m_BridgePos.y, m_ClipWidth, m_ClipHeight);
}