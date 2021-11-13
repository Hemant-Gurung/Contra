#include "pch.h"
#include "EnemyRunning.h"
#include "Vector2f.h"
#include "utils.h"

EnemyRunning::EnemyRunning(Point2f pos, std::string path)
	:Enemy(pos,path)
	,m_DestRect{}
	,m_CurFrame{}
	,m_ActorPos{}
	, m_VelocityRunningEnemy{ Vector2f(0.0f,0.0f) }
	,m_Acceleration{Point2f(0,-981.f)}
	,m_HorSpeed{150}
	,m_VerticalSpeed{300}
	,m_IsTurningRight{false}
	,m_IsOnGround{false}
	,m_EnemyWidthOnGame{80.f}
	,m_EnemyHeightOnGame{90.f}
{
	m_ClipWidth = m_pEnemyTexture->GetWidth() /12;
	m_ClipHeight = m_pEnemyTexture->GetHeight();
	m_CurrFrame = 0;
	m_NrFrames = 6;
	m_AnimeTime = 1 / float(m_NrFrames);
}

EnemyRunning::~EnemyRunning()
{
	delete m_pEnemyTexture;
	m_pEnemyTexture = nullptr;
}

void EnemyRunning::Draw() const
{
	if (!m_IsHit && m_IsActorInRange)
	{
		m_pEnemyTexture->Draw(m_DestRect, Rectf(m_ClipWidth * m_CurFrame, m_ClipWidth, m_ClipWidth, m_ClipHeight));
	}
	else if (m_IsHit &&!m_IsFinishedExploding && m_pExplodingTexture != nullptr)
	{
		m_pExplodingTexture->Draw(Rectf(m_Pos.x, m_Pos.y+50.f, 39.f,36.f), m_SrcRect);
	}

}

void EnemyRunning::Update(float elapsedSec)
{
	UpdateSrcRect();

	if (!m_IsHit && m_IsActorInRange)
	{
		m_DestRect.left = m_Pos.x;
		m_DestRect.bottom = m_Pos.y;
		m_DestRect.width = m_EnemyWidthOnGame;
		m_DestRect.height = m_EnemyHeightOnGame;
	}
	//if lower than lowest then die
	
	if (!m_IsHit)
	{
		UpdateEnemyFallingRange(elapsedSec);
		UpdateEnemySprite(elapsedSec);
	}
	else
	{
		m_AccuSec += elapsedSec;
		if (m_AccuSec >= m_AnimeTime * 5)
		{
			if (m_CurrFrame > 10)m_CurrFrame = 0;

			else
			{
				m_AccuSec = 0;
				m_CurrFrame++;
				if (m_CurrFrame >= 3)
				{
					m_CurrFrame = 0;
					m_IsFinishedExploding = true;

				}
			}
		}
	}
}

void EnemyRunning::UpdateSrcRect()
{
	if (!m_IsActorInRange)
	{
		m_SrcRect.left = 0;
	}
	else
	{
		m_SrcRect.left = m_ClipWidth * m_CurrFrame;
	}

	m_SrcRect.bottom = m_ClipHeight;
	m_SrcRect.width = m_ClipWidth;
	m_SrcRect.height = m_SrcRect.bottom;
}
bool EnemyRunning::GetActorIsInRange(const Rectf& actorPos)
{
	const int maxEnemyRadious{ 300 };
	m_ActorPos = actorPos;
	if (Enemy::Distance(Point2f(actorPos.left, actorPos.bottom), m_Pos) < maxEnemyRadious)
	{
		m_IsActorInRange = true;
	}
	else
	{
		m_IsActorInRange = false;
	}

	return m_IsActorInRange;
}

Vector2f EnemyRunning::GetVelocity() const
{
	return m_VelocityRunningEnemy;
}

void EnemyRunning::UpdateEnemySprite(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_AnimeTime)
	{
		m_AccuSec = 0;
		++m_CurFrame;
		if (m_CurFrame >= m_NrFrames)
		{
			if (m_IsTurningRight)
			{
				m_CurFrame = 6;
				m_NrFrames = 12;
			}
			else
			{
				m_CurFrame = 0;
				m_NrFrames = 6;
			}

		}
	}
}

void EnemyRunning::UpdateEnemyFallingRange(float elapsedSec)
{
	const float minEnemyFallHeight{ 20.f };
	if (m_Pos.y < minEnemyFallHeight)
	{
		m_Pos.y--;
	}
	m_VelocityRunningEnemy.x = -float(m_HorSpeed);
	m_VelocityRunningEnemy.y = -float(m_VerticalSpeed);

	if (!m_IsOnGround)
	{
		m_Pos.y += m_VelocityRunningEnemy.y * elapsedSec;
	}
	else
	{
		m_VelocityRunningEnemy += m_Acceleration * elapsedSec;
		m_Pos.x += m_VelocityRunningEnemy.x * elapsedSec;
	}

}

Rectf EnemyRunning::GetPos() const
{
	if (!m_IsHit)
	{
		return Rectf{ m_Pos.x,m_Pos.y,m_EnemyWidthOnGame,m_EnemyHeightOnGame};
	}
	return Rectf(0.f,0.f,0.f,0.f);
}

void EnemyRunning::IsOnGround(bool isonground)
{
	m_IsOnGround = isonground;
}


bool EnemyRunning::UpdateIfPlayerIsShot(Rectf avatarpos)
{
	if (!m_IsHit)
	{
		return utils::IsOverlapping(avatarpos, m_DestRect);
	}
	return false;
}

bool EnemyRunning::GetEnemyIsHit() const
{
	return m_IsHit;
}

void EnemyRunning::SetEnemyIsHit(const bool isHit)
{
	m_pEnemyDeadSound->Play(0);
	m_IsHit = isHit;
}

