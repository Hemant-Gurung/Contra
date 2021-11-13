#pragma once
#include "Enemy.h"
class EnemyTank final :public Enemy
{
public:
	EnemyTank(Point2f pos, std::string path);
	virtual ~EnemyTank() override;

	 virtual void Draw() const override;
	 virtual void Update(float elapsedSec) override;
	 virtual void FindActorLocation(Rectf actorPos) override;
	// virtual void EnemyIsHit(Rectf PlayerbulletPos) override;
	 virtual void SetEnemyIsHit(const bool isHit) override;
private:
	void DrawEnemyAndBullet() const;
	
	void UpdateSrcRect(float elapsedSec);
	void UpdateExplodingTexture(float elapsedSec);
	void UpdateEnemyTankBullet(float elapsedSec);
	void CheckIfActorIsInRange(const Rectf& actorPos);
	void UpdateEnemyTankSprite(const Rectf& actorPos);
	
	bool m_IsFinishedExploding;
	SoundEffect* m_pEnemyTankHitSound;
	float m_EnemyTankDimension{};
};