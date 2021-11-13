#include "pch.h"
#include "EnemyTank.h"
#include "utils.h"
#include "Bullet.h"
EnemyTank::EnemyTank(Point2f pos, std::string path)
	:Enemy(pos,path)
	,m_IsFinishedExploding{false}
	, m_pEnemyTankHitSound{ new SoundEffect("Resources/Sounds/enemy-hit.mp3")}
	,m_EnemyTankDimension{80.f}
	
{
	m_ClipWidth = m_pEnemyTexture->GetWidth() / 14;
	m_ClipHeight = m_pEnemyTexture->GetHeight();

}
EnemyTank::~EnemyTank()
{
	delete m_pEnemyTexture;
	m_pEnemyTexture = nullptr;
	
	delete m_pExplodingTexture;
	m_pExplodingTexture = nullptr;

	delete m_pEnemyTankHitSound;
	m_pEnemyTankHitSound = nullptr;
}
void EnemyTank::Draw() const
{
	if (m_IsActorInRange)
	{
		if (!m_IsHit )
		{
			DrawEnemyAndBullet();
		}
		else if (!m_IsFinishedExploding && m_pExplodingTexture != nullptr)
		{
			m_pExplodingTexture->Draw(Rectf(m_Pos.x, m_Pos.y, m_ClipWidth, m_ClipHeight), m_SrcRect);
		}
	}

}

void EnemyTank::Update(float elapsedSec)
{
	UpdateSrcRect(elapsedSec);
	UpdateExplodingTexture(elapsedSec);
	UpdateEnemyTankBullet(elapsedSec);
}

void EnemyTank::FindActorLocation(Rectf actorPos)
{
	CheckIfActorIsInRange(actorPos);
	if (m_IsHit)
	{
		m_ClipWidth = m_pExplodingTexture->GetWidth() / 3;
		m_ClipHeight = m_pExplodingTexture->GetHeight();
	}
	else
	{
		Enemy::GetActorIsInRange(actorPos);
		UpdateEnemyTankSprite(actorPos);
	}
}

void EnemyTank::UpdateSrcRect(float elapsedSec)
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

void EnemyTank::UpdateExplodingTexture(float elapsedSec)
{
	
	if (m_IsHit)
	{
		if (m_pExplodingTexture != nullptr)
		{
			m_SrcRect.left = m_ClipWidth * m_CurrFrame;
			m_SrcRect.bottom = m_ClipHeight;
			m_SrcRect.width = m_ClipWidth;
			m_SrcRect.height = m_SrcRect.bottom;
		}


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
	else
	{
		if (m_IsActorInRange)
		{
			m_AccuSec += elapsedSec*3;
			const int maxShootRange{ 50 };
			const int minShootRange{ 20 };
			int randShootTimeEnemy = rand() % (maxShootRange - minShootRange +1 ) + minShootRange;
			if (m_AccuSec >= m_AnimeTime * randShootTimeEnemy)
			{
				m_AccuSec = 0;
				switch (m_EnemyStates)
				{
				case Enemy::EnemyStates::up:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x+m_ClipWidth/2,m_Pos.y+m_ClipHeight/2), Vector2f(0, 1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::down:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 2), Vector2f(0, -1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::upLeft:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 2), Vector2f(-1, 1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::upRight:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 2), Vector2f(1, 1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::downRight:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 2), Vector2f(1, -1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::downLeft:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 2), Vector2f(-1, -1), "EnemyRed"));
					break;
				default:
					break;
				}
			}
		}
	}
}

void EnemyTank::UpdateEnemyTankBullet(float elapsedSec)
{
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
			m_pBullet[i]->Update(elapsedSec);
	}
}


void EnemyTank::DrawEnemyAndBullet() const
{
	m_pEnemyTexture->Draw(Rectf(m_Pos.x, m_Pos.y, m_ClipWidth+20.f, m_ClipHeight+20.f), Rectf{ m_ClipWidth * m_CurrFrame,m_ClipHeight,m_ClipWidth,m_ClipHeight });
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		m_pBullet[i]->Draw();
	}
}


void EnemyTank::UpdateEnemyTankSprite(const Rectf& actorPos)
{
	const int maxDifferenceInPositon{ 150 };
	const int minDifferenceInPosition{ 10 };

	float distance = Enemy::Distance(Point2f(actorPos.left + actorPos.width / 2, actorPos.bottom + actorPos.height / 2), m_Pos);
	if (distance < maxDifferenceInPositon && m_Pos.x - actorPos.left < minDifferenceInPosition)
	{
		m_CurrFrame = 11;
		m_EnemyStates = EnemyStates::down;
	}
	if (actorPos.left < m_Pos.x )
	{
		m_CurrFrame = 3;
		m_EnemyStates = EnemyStates::upLeft;
	}
	if (actorPos.left > m_Pos.x && actorPos.bottom > m_Pos.y)
	{
		m_CurrFrame = 5;
		m_EnemyStates = EnemyStates::up;
	}
	if (distance < maxDifferenceInPositon && m_Pos.x - actorPos.left > minDifferenceInPosition)
	{
		m_CurrFrame = 12;
		m_EnemyStates = EnemyStates::downLeft;
	}

	if (distance >= maxDifferenceInPositon && actorPos.left-m_Pos.x > minDifferenceInPosition)
	{
		m_CurrFrame = 10;
		m_EnemyStates = EnemyStates::downRight;
	}

}

void EnemyTank::CheckIfActorIsInRange(const Rectf& actorPos)
{
	const int maxEnemyRadious{ 300 };
	if (Enemy::Distance(Point2f(actorPos.left,actorPos.bottom),m_Pos) < maxEnemyRadious)
	{
		m_IsActorInRange = true;
	}
	else
	{
		m_IsActorInRange = false;
	}
}

void EnemyTank::SetEnemyIsHit(const bool isHit)
{
	const int maxHitItCanTake{25};
	m_pEnemyTankHitSound->Play(0);
	m_CountTimesHit += 5;
	if (m_CountTimesHit > maxHitItCanTake)
	{
		m_pEnemyDeadSound->Play(0);
		m_IsHit = true;
	}
}
