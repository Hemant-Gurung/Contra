#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include "Bullet.h"
#include <iostream>
Enemy::Enemy(Point2f pos,std::string enemypath)
	:m_Pos{pos}
	, m_pEnemyTexture{ new Texture{enemypath} }
	,m_CurrFrame{1}
	,m_AccuSec{}
	,m_AccuSecExploding{}
	,m_NrFrames{ 4 }
	,m_IsHit{false}
	,m_IsFinishedExploding{false}
	,m_ClipHeight{100.f}
	,m_ClipWidth{80.f}
	,m_IsActorInRange{false}
	,m_pEnemyDeadSound{new SoundEffect("Resources/Sounds/explode-2.mp3")}
	,m_EnemyStates{}
	,m_CountTimesHit{}
	,m_pEnemyGunSound{new SoundEffect("Resources/Sounds/enemy-gun.mp3")}
{
	m_pExplodingTexture = new Texture("Resources/Enemy/explosion.png");
	m_NrFrames = 12;
	m_NrFramePerSec = 12;
	m_AnimeTime = 1 / float(4);
	
}

Enemy::~Enemy()
{
	delete m_pExplodingTexture;
	m_pExplodingTexture = nullptr;

	delete m_pEnemyDeadSound;
	m_pEnemyDeadSound = nullptr;
	
	delete m_pEnemyTexture;
	delete m_pEnemyGunSound;
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		delete m_pBullet[i];
	}
	m_pBullet.clear();
}

void Enemy::Draw() const
{
	
	if (m_IsActorInRange)
	{
		if (!m_IsHit)
		{
			m_pEnemyTexture->Draw(Rectf(m_Pos.x, m_Pos.y, m_ClipWidth,m_ClipHeight), m_SrcRect);
			for (size_t i = 0; i < m_pBullet.size(); i++)
			{
				m_pBullet[i]->Draw();
			}
		}
		else if (!m_IsFinishedExploding)
		{
			//m_pEnemyDeadSound->Play(0);
			DrawExplodingTexture();
		}
	}
	
}
void Enemy::Update(float elapsedSec)
{
	UpdateSrcRect(elapsedSec);
	UpdateEnemyBulletIfActorInRange(elapsedSec);
	BulletCollisionOnEnemy();
	UpdateEnemyBullet(elapsedSec);
}

void Enemy::FindActorLocation(Rectf actorPos)
{ 
	if (m_IsHit)
	{
		
		m_ClipWidth = m_pExplodingTexture->GetWidth()/3;
		m_ClipHeight = m_pExplodingTexture->GetHeight();

	}
	else
	{
		if (actorPos.left < m_Pos.x && actorPos.bottom > m_Pos.y)
		{
			m_CurrFrame = 2;
			m_EnemyStates = EnemyStates::upLeft;
		}
		 if (actorPos.left > m_Pos.x && actorPos.bottom > m_Pos.y)
		{
			m_CurrFrame = 5;
			m_EnemyStates = EnemyStates::upRight;
		}
		 if (actorPos.left < m_Pos.x && actorPos.bottom < m_Pos.y)
		{
			m_CurrFrame = 10;
			m_EnemyStates = EnemyStates::downLeft;
		}
	    if (actorPos.left > m_Pos.x && actorPos.bottom < m_Pos.y)
		{
			m_CurrFrame = 8;
			m_EnemyStates = EnemyStates::downRight;
		}
		 if (actorPos.left < m_Pos.x && m_Pos.y - actorPos.bottom <=40.f &&  m_Pos.y- actorPos.bottom >= 0.f)
		{
			m_CurrFrame = 0;
			m_EnemyStates = EnemyStates::left;
		}
		 if (actorPos.left > m_Pos.x && actorPos.bottom- m_Pos.y <= 40.f && actorPos.bottom - m_Pos.y >= 0.f)
		{
			m_CurrFrame = 8;
			m_EnemyStates = EnemyStates::right;
		}
	}
}

bool Enemy::BulletCollisionOnEnemy()
{
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		if (m_pBullet[i]->m_ActorType == "EnemyRed")
		{
			m_pBullet[i]->SetAvatarPos(m_AvatarPos);
			if (!m_IsHit && m_pBullet[i]->CheckBulletCollision())
			{
				return true;
			}
		}
	}
	return false;
}


bool Enemy::UpdateIfPlayerIsShot(Rectf avatarpos)
{
	m_AvatarPos = avatarpos;
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		const Rectf bulletpos = m_pBullet[i]->GetBulletPos();
			if (utils::IsOverlapping( bulletpos, avatarpos))
			{
				m_pBullet[i]->SetDestroyBullet(true);
				return true;
			}
	}
	return false;
}

bool Enemy::GetEnemyIsHit() const
{
	
	return m_IsHit;
}

void Enemy::SetCameraBoundary(const Rectf& cameraBoundary)
{
	m_CameraBoundary = cameraBoundary;
}

void Enemy::UpdateSrcRect(float elapsedSec)
{
	float TextEnemyWidth{ m_pEnemyTexture->GetWidth() };
	float TextEnemyHeight{ m_pEnemyTexture->GetHeight() };

	if (!m_IsActorInRange)
	{
		m_SrcRect.left = (TextEnemyWidth / 12) * 0;
	}
	else
	{
		m_SrcRect.left = (TextEnemyWidth / 12) * m_CurrFrame;
	}

	m_SrcRect.bottom = TextEnemyHeight;
	m_SrcRect.width = TextEnemyWidth / 12;
	m_SrcRect.height = TextEnemyHeight;
}
void Enemy::UpdateEnemyBulletIfActorInRange(float elapsedSec)
{
	const int minNum = 20;
	const int maxNum = 60;
	const int maxFrameAllowed = 10;
	const int maxExplodingFrameCount = 3;
	
	if (m_IsActorInRange && !m_IsHit)
	{
	
			m_AccuSec += elapsedSec*3;
			int randShootTimeEnemy = rand() % (maxNum - minNum) + minNum;
			if (m_AccuSec >= m_AnimeTime *randShootTimeEnemy)
			{
				m_AccuSec = 0;
				m_pEnemyGunSound->Play(0);
				switch (m_EnemyStates)
				{

				case Enemy::EnemyStates::up:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 1.5f), Vector2f(0, 1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::down:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 1.5f), Vector2f(0, -1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::left:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 1.5f), Vector2f(-1, 0), "EnemyRed"));
					break;
				case Enemy::EnemyStates::right:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 1.5f), Vector2f(1, 0), "EnemyRed"));
					break;
				case Enemy::EnemyStates::upLeft:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight /1.5f), Vector2f(-1, 1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::upRight:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 1.5f), Vector2f(1, 1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::downRight:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 1.5f), Vector2f(1, -1), "EnemyRed"));
					break;
				case Enemy::EnemyStates::downLeft:
					m_pBullet.push_back(new Bullet(Point2f(m_Pos.x + m_ClipWidth / 2, m_Pos.y + m_ClipHeight / 1.5f), Vector2f(-1, -1), "EnemyRed"));
					break;
				default:
					break;
				}

			}
	}
	else
	{
		m_AccuSecExploding += elapsedSec;
		if (m_AccuSecExploding >= m_AnimeTime)
		{
			if (m_CurrFrame >= maxFrameAllowed)
			{
				m_CurrFrame = 0;
			}
			else
			{
				m_AccuSecExploding = 0;
				m_CurrFrame++;
				if (m_CurrFrame >= maxExplodingFrameCount)
				{
					m_CurrFrame = 0;
					m_IsFinishedExploding = true;

				}
			}
		}
	}
}

void Enemy::UpdateEnemyBullet(float elapsedSec)
{
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		m_pBullet[i]->Update(elapsedSec);
	}
}

bool Enemy::GetActorIsInRange(const Rectf& actorPos)
{
	const int maxEnemyRadious = 400;
	if (Distance(Point2f(actorPos.left,0), Point2f(m_Pos.x,0)) < maxEnemyRadious)
	{
		m_IsActorInRange = true;
	}
	else
	{
		m_IsActorInRange = false;
	}

	return m_IsActorInRange;
}

void Enemy::DrawExplodingTexture() const
{
	m_pEnemyDeadSound->Play(0);
	m_pExplodingTexture->Draw(Rectf(m_Pos.x, m_Pos.y, m_ClipWidth, m_ClipHeight), m_SrcRect);
}

float Enemy::Distance(Point2f actorPos, Point2f enemyPos)
{
	float distance = float(sqrt(pow(enemyPos.x - actorPos.x, 2) + pow(enemyPos.y - actorPos.y, 2)));
	return distance;
}

Vector2f Enemy::GetVelocity() const
{
	return Vector2f{};
}
Rectf Enemy::GetPos() const
{
	if (!m_IsHit)
	{
		return Rectf(m_Pos.x, m_Pos.y,m_ClipWidth,m_ClipHeight);
	}
	return Rectf(0.f, 0.f, 0.f, 0.f);
}

void Enemy::SetEnemyIsHit(const bool isHit)
{
	m_pEnemyDeadSound->Play(0);
	m_IsHit = isHit;
}

void Enemy::IsOnGround(bool isonground)
{

}
