#pragma once
#include "structs.h"
#include "Enemy.h"
#include "SoundEffect.h"
#include "Texture.h"

class EnemyBoss final: public Enemy
{
public:
	EnemyBoss(Point2f pos,std::string enemyType);
	
	virtual ~EnemyBoss() override;
	virtual Rectf GetPos()const override;
	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual bool GetActorIsInRange(const Rectf& actorPos) override;
	virtual void SetEnemyIsHit(const bool isHit) override;

private:
	Texture* m_pBossTexture{};
	Texture* m_pDestroyedBossTexture{};
	const float m_BossPos{};
	std::string m_EnemyType;
	float m_AccuSec{};
	float m_AnimeTime{};
	Rectf m_ActorPos;
	bool m_IsDefeated{};

	SoundEffect* m_pSoundEffect;
	SoundEffect* m_pEnemyBossGunSound;
	SoundEffect* m_pBossDefeatSound;
};