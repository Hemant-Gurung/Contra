#pragma once
#include "Level.h"
#include "Enemy.h"
class EnemyRunning final :public Enemy
{
public:
	EnemyRunning(Point2f pos, std::string path);
	
	virtual ~EnemyRunning()override;
	virtual void Draw() const override;
    virtual void Update(float elapsedSec) override;
	virtual bool GetActorIsInRange(const Rectf& actorPos) override;
	virtual Vector2f GetVelocity() const override;
	virtual Rectf GetPos()const override;
	virtual void IsOnGround(bool isonground)override;
	virtual bool UpdateIfPlayerIsShot(Rectf avatarpos) override;
	virtual bool GetEnemyIsHit() const override;
	virtual void SetEnemyIsHit(const bool isHit) override;

private:

	Rectf m_DestRect;
	int m_CurFrame;
	Rectf m_ActorPos;
	bool m_IsTurningRight;
	Vector2f m_VelocityRunningEnemy;
	Vector2f m_Acceleration;
	int m_HorSpeed{};
	int m_VerticalSpeed{};
	bool m_IsOnGround;
	float m_EnemyWidthOnGame{}, m_EnemyHeightOnGame{};
	void UpdateSrcRect();
	void UpdateEnemyFallingRange(float elapsedSec);
	void UpdateEnemySprite(float elapsedSec);
	
};