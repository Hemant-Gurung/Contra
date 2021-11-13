#pragma once
#include "SoundEffect.h"
#include "structs.h"
#include "Texture.h"
#include "Avatar.h"
#include <vector>

class Bullet;
class Enemy
{
public:
	enum class EnemyStates 
	{
		  up
		, down
		, left
		, right
		, upLeft
		, upRight
		, downRight
		, downLeft
	};

	Enemy(Point2f pos, std::string enemypath);
	Enemy(const Enemy&) = delete;
	Enemy& operator=(const Enemy&) = delete;
	Enemy(Enemy&&) = delete;
	Enemy& operator=(Enemy&&) = delete;

	virtual ~Enemy();
	virtual void Draw() const;
	virtual void Update(float elaspsedSec);
	virtual void FindActorLocation(Rectf actorPos);
	virtual bool GetActorIsInRange(const Rectf& actorPos);
	virtual Vector2f GetVelocity() const;
	virtual Rectf GetPos()const;
	virtual void IsOnGround(bool isonground);
	virtual void UpdateSrcRect(float elapsedSec);
	virtual bool UpdateIfPlayerIsShot(Rectf avatarpos);
	virtual bool GetEnemyIsHit() const;
	virtual void SetEnemyIsHit(const bool isHit);

	bool BulletCollisionOnEnemy();
	void SetCameraBoundary(const Rectf& cameraBoundary);
	
	float Distance(Point2f actorPos, Point2f enemyPos);
	
	//Rectf GetEnemyPos();
protected:
	std::vector<Bullet*> m_pBullet;

	EnemyStates m_EnemyStates;
	Point2f m_Pos;

	Rectf m_AvatarPos;
	Rectf m_SrcRect;
	Rectf m_CameraBoundary;

	SoundEffect* m_pEnemyDeadSound;
	SoundEffect* m_pEnemyGunSound;
	Texture* m_pEnemyTexture;
	Texture* m_pExplodingTexture;

	int m_CurrFrame{};
	int m_NrFrames;
	int m_NrFramePerSec;
	int m_CountTimesHit;
	float m_AccuSecExploding{};
	float m_AnimeTime;
	float m_AccuSec;
	float m_ClipWidth;
	float m_ClipHeight;

	bool m_IsActorInRange{};
	bool m_IsFinishedExploding;
	bool m_IsHit{};

	void UpdateEnemyBulletIfActorInRange(float elapsedSec);
    void UpdateEnemyBullet(float elapsedSec);
	void DrawExplodingTexture() const;
	
	
};