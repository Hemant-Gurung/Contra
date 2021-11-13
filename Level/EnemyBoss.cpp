#include "pch.h"
#include "EnemyBoss.h"
#include "utils.h"

EnemyBoss::EnemyBoss(Point2f pos,std::string enemyType)
	:Enemy(pos,enemyType)
	,m_pBossTexture{new Texture("Resources/Enemy/EnemyBoss.png")}
	,m_pDestroyedBossTexture{new Texture("Resources/Enemy/EnemyBossDead.png")}
	,m_BossPos{pos.x}
	,m_AccuSec{}
	,m_AnimeTime{}
	,m_EnemyType{enemyType}
	,m_pSoundEffect{new SoundEffect("Resources/Sounds/enemy-hit.mp3")}
	,m_pEnemyBossGunSound{new SoundEffect("Resources/Sounds/flame-gun.mp3")}
	,m_pBossDefeatSound{new SoundEffect("Resources/Sounds/boss-defeat.mp3")}
	,m_IsDefeated{false}
{
	m_AnimeTime = 1 / float(10);
}

EnemyBoss::~EnemyBoss()
{
	delete m_pBossTexture;
	delete m_pDestroyedBossTexture;
	delete m_pSoundEffect;
	delete m_pBossDefeatSound;
	delete m_pEnemyBossGunSound;
	
	m_pSoundEffect = nullptr;
	m_pBossTexture = nullptr;
	m_pDestroyedBossTexture = nullptr;
	m_pBossDefeatSound = nullptr;
	m_pEnemyBossGunSound = nullptr;
}

void EnemyBoss::Draw() const
{
	if (!m_IsHit)
	{
		m_pBossTexture->Draw(Rectf{ m_BossPos,0.f,m_pBossTexture->GetWidth(),m_pBossTexture->GetHeight() }, Rectf(0, m_pBossTexture->GetHeight(), m_pBossTexture->GetWidth(), m_pBossTexture->GetHeight()));	
		for (size_t i = 0; i < m_pBullet.size(); i++)
		{
			m_pBullet[i]->Draw();
		}
	}
	else
	{
		if (!m_IsDefeated)
		{
			m_pBossDefeatSound->Play(0);
		}
		m_pDestroyedBossTexture->Draw(Rectf{ m_BossPos,0.f,m_pDestroyedBossTexture->GetWidth(),m_pDestroyedBossTexture->GetHeight() }, Rectf(0, m_pDestroyedBossTexture->GetHeight(), m_pDestroyedBossTexture->GetWidth(), m_pDestroyedBossTexture->GetHeight()));
	}
	
}

void EnemyBoss::Update(float elapsedSec)
{
	if (m_IsActorInRange && !m_IsHit)
	{
		m_AccuSec += elapsedSec;
		if (m_AccuSec >= m_AnimeTime * 10.f)
		{
			m_AccuSec = 0;
			if (m_ActorPos.bottom >= 200)
			{
				m_pEnemyBossGunSound->Play(0);
				m_pBullet.push_back(new Bullet(Point2f(m_BossPos, 220.f), Vector2f(-1, 0), "EnemyBoss"));
				m_pBullet.push_back(new Bullet(Point2f(m_BossPos+50.f, 220.f), Vector2f(-1, 0), "EnemyBoss"));
			}
			else
			{
				m_pEnemyBossGunSound->Play(0);
				m_pBullet.push_back(new Bullet(Point2f(m_BossPos, 100.f), Vector2f(-1, 0), "EnemyBoss"));
			}
		}
	}
	if (!m_IsHit)
	{
		for (size_t i = 0; i < m_pBullet.size(); i++)
		{
			m_pBullet[i]->Update(elapsedSec);
			m_pBullet[i]->CheckBulletBoundary(m_CameraBoundary);
		}
	}
}

bool EnemyBoss::GetActorIsInRange(const Rectf& actorPos)
{
	m_ActorPos = actorPos;
		if (Distance(Point2f(actorPos.left, 0), Point2f(m_BossPos, 0)) < 500.f)
		{
			m_IsActorInRange = true;
		}
		else
		{
			m_IsActorInRange = false;
		}
		return m_IsActorInRange;
}

void EnemyBoss::SetEnemyIsHit(const bool isHit) 
{
	m_pSoundEffect->Play(0);
	m_CountTimesHit+=10;
	if (m_CountTimesHit >= 500)
	 {
		m_IsHit = true;		
	}
}
Rectf EnemyBoss::GetPos()const
{
	Rectf m_EnemyPos;
		m_EnemyPos.left = m_BossPos;
		m_EnemyPos.bottom = 0.f;
		m_EnemyPos.width = m_pBossTexture->GetWidth();
		m_EnemyPos.height = m_pBossTexture->GetHeight()/2;
		return m_EnemyPos;
}