#pragma once
#include "structs.h"
#include "SoundStream.h"
#include "SoundEffect.h"
#include "Vector2f.h"
#include <vector>
#include "Bullet.h"

class Enemy;
class Level;
class Texture;
class Avatar final
{
public:
	enum class ActionState
	{
		waiting,
		moving,
		OnWater,
		jumping,
		proning,
		lookUp,
		lookRightUp,
		lookleftUp,
		lookLeftDown,
		lookRightDown,
		swimming,
		OnWaterWait,
		lookUpSwimming,
		lookRightUpSwimming,
		lookLeftUpSwimming
	};

	enum class ShootDirection
	{
		Right,
		Left,
		Up,
		RightUp,
		RightDown,
		LeftUp,
		LeftDown
	};

	Avatar();
	~Avatar();
	
	Avatar(const Avatar&) = delete;
	Avatar& operator=(const Avatar&) = delete;
	Avatar(Avatar&&) = delete;
	Avatar& operator=(Avatar&&) = delete;

	void Draw() const;
	void Update(float elapsedSec,Level& level);
	void UpdateIfEnemyIsDestroyed(std::vector<Enemy*> enemy);
	void InitializeFrames();
	//Getters and Setters
	Rectf GetShape()const;
	Rectf GetBulletPos() const;
	Avatar::ActionState GetActionState();
	void SetIsShot(bool isshot);
	void SetCameraBoundary(Rectf cameraBoundary);
	void SetEnemyPos(Rectf enemyPos);
	int GetCurrentLives() const;
	void UpdateNumberOfLives(int lives);
	//keyboard input
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
private:
	//player properties
	Rectf m_Shape{};
	float m_HorSpeed{};
	float m_JumpSpeed{};
	Vector2f m_Velocity{};
	Vector2f m_Acceleration{};
	ActionState m_ActionState;
	ShootDirection m_ShootDirection;
	

	//sprite properties
	Texture* m_pSpriteTexture;
	float m_SpritesTexture{};
	float m_ClipWidth{};
	float m_ClipHeight{};
	float m_NrFramePerSec{};
	float m_NrFrames{};
	float m_AnimeTime{};
	int m_AnimeFrame{};
	float m_SpriteSheetWidth{};
	float m_SpriteSheetHeight{};

	//sprite data members
	float m_AccuSec;

	int m_CurFrame;
	int m_Cols{};
	int m_Rows{};
	bool m_ToggleLeftRight;

	Rectf m_SrcRect;
	Rectf m_DestRect;
	std::vector<Bullet*> m_pBullet;

	// class variables
	int m_ChooseSprite{};
	int m_CountPlayerLives;

	float m_InitialDestRectBottom{};
	float m_AccuSecBeginning;
	float m_WaterSectionOnLevel{};
	const float m_StartPoint;

	bool m_IsOnGround{};
	bool m_EnemyIsHit;
	bool m_IsOnWater{};
	bool m_IsShot;
	bool m_Beginning;
	
	Rectf m_CameraBoundary;
	Rectf m_EnemyPos;
	
	SoundEffect* m_pLevel1GunSound;
	SoundEffect* m_pAvatarDeadSound;
	SoundEffect* m_pLevel2GunSound;

	void DrawBullets() const;
	void HandleAvatarButtonInput();
	void InitSourceRect();
	void UpdateSourceRect();
	void InitDestRect();
	void UpdateAvatarState(float elapsedSec);
	void CountAccumulatedSecond(float elapsedSec);
	void UpdateContraStateMovement(float elapsedSec, const Level& level);
	void HandleAvatarButtonInputOnWater();
	void HandleAvatarButtonInputOnGround();
	void CheckIfPlayerIsOnWater(Level& level);
	void CheckIfPlayerIsOnGround(const Level& level);
	void ChangeVelocityBasedOnAcceleration(float elapsedSec,const Level& level);
	void CheckCollisionOnJump(float elapsedSec, const Level& level);
	//player bullet update
	void UpdateBullet(float elapsedSec);
	
};