#pragma once
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"
class Bullet final
{
public:
	Bullet(Point2f pos, Vector2f direction ,std::string actorType);
	~Bullet();
	Bullet(const Bullet&) = delete;
	Bullet& operator=(const Bullet&) = delete;
	Bullet(Bullet&&) = delete;
	Bullet& operator=(Bullet&&) = delete;
	
	static int m_AvatarPower;
	std::string m_ActorType;

	void Draw();
	void Update(float elapsedSec);
	
	
	bool CheckBulletBoundary(const Rectf& CameraBoundary);
	bool CheckBulletCollision();
	void SetAvatarPos(const Rectf& avatarpos);
	void CheckAvatarFacingDirection(const bool avaratDirection);
	void SetDestroyBullet(const bool isBulletDestroyed);
	bool GetBulletDetroyed() const;
	void SetBulletPowerUp(const int& power);
	Rectf GetBulletPos()const;
	

private:

	void InitializeBulletTexture();
	void DrawEnemyBullet();
	void DrawAvatarBullet();
	void UpdateAvatarBullet(float elapsedSec);
	void CheckCurrentFrame(float elapsedSec);

	Rectf m_AvatarPos;
	Point2f m_Pos;
	Texture* m_pBulletTexture;
	Vector2f m_BulletVelocity{};
	Vector2f m_Acceleration;
	
	bool m_DestroyBullet;
	int m_CurrFrame;
	float m_AccuSec;
	int m_NrFrames;
	int m_NrFramePerSec;
	float m_AnimeTime;
	bool m_IsProning;
	float m_BulletRotateAngle;
	float m_HorSpeed;
	float m_verticalSpeed;

	bool m_ToggleLeftRight;

	
};