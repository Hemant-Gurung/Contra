#include "pch.h"
#include "Avatar.h"
#include "Level.h"
#include <iostream>
#include "utils.h"
#include "Enemy.h"
#include "Matrix2x3.h"
#include "Texture.h"

Avatar::Avatar()
	:m_HorSpeed{150.f}
	,m_JumpSpeed{400.f}
	,m_Velocity{Vector2f(0.0f,0.0f)}
	,m_Acceleration{Vector2f(0,-981.f)}
	,m_ActionState{ActionState::moving}
	,m_IsOnGround{false}
	,m_pSpriteTexture{new Texture("Resources/player.png")}
	,m_ClipWidth{72.f}
	,m_ClipHeight{97.f}
	,m_AccuSec{}
	,m_CurFrame{}
	,m_DestRect{}
	,m_SrcRect{}
	,m_Cols{10}
	,m_Rows{11}
	,m_StartPoint{50.f}
	,m_SpriteSheetWidth{720.f}
	,m_SpriteSheetHeight{ 1067.f}
	,m_IsShot{false}
	,m_InitialDestRectBottom{500.f}
	,m_WaterSectionOnLevel{ 3695.f }
	,m_CameraBoundary{}
	,m_CountPlayerLives{}
	,m_AccuSecBeginning{}
	,m_Beginning{true}
	,m_ShootDirection{ShootDirection::Right}
	,m_pLevel1GunSound{ new SoundEffect("Resources/Sounds/machine-gun.mp3") }
	,m_pLevel2GunSound{ new SoundEffect("Resources/Sounds/flame-gun.mp3") }
	,m_pAvatarDeadSound{new SoundEffect("Resources/Sounds/death.mp3")}
{
	InitializeFrames();
	
}

Avatar::~Avatar()
{
	for (size_t i = 0;i<m_pBullet.size(); i++)
	{
		delete m_pBullet[i];
	}
	m_pBullet.clear();
	
	delete m_pSpriteTexture;
	delete m_pLevel1GunSound;
	delete m_pLevel2GunSound;
	delete m_pAvatarDeadSound;
	m_pSpriteTexture = nullptr;
	m_pLevel1GunSound = nullptr;
	m_pLevel2GunSound = nullptr;
	m_pAvatarDeadSound = nullptr;
}
void Avatar::InitializeFrames()
{
	m_NrFrames = 1;
	m_NrFramePerSec = 10;
	m_AnimeTime = 1 / float(m_NrFramePerSec);
	InitSourceRect();
	InitDestRect();
}

void Avatar::Update(float elapsedSec,Level& level)
{
	CheckIfPlayerIsOnGround(level);
	CheckIfPlayerIsOnWater(level);
	UpdateContraStateMovement(elapsedSec, level);
	UpdateAvatarState(elapsedSec);
	CountAccumulatedSecond(elapsedSec);
	UpdateBullet(elapsedSec);
}

void Avatar::Draw() const
{
	
	glPushMatrix();
	glTranslatef(m_DestRect.left, m_DestRect.bottom, 1);
		if (m_ToggleLeftRight)
		{
			m_pSpriteTexture->Draw(Rectf{}, m_SrcRect);
		}
		else
		{
			glScalef(-1, 1, 0);
			glTranslatef(-m_DestRect.width, 0.f, 0.0f);
			m_pSpriteTexture->Draw(Rectf{}, m_SrcRect);
		}
	m_pSpriteTexture->Draw(Rectf{}, m_SrcRect);
	glPopMatrix();
	DrawBullets();
	
}

void Avatar::HandleAvatarButtonInput()
{
	if (m_IsOnWater)
	{
		HandleAvatarButtonInputOnWater();
	}
	else
	{
	
		HandleAvatarButtonInputOnGround();
	}
	
}

void Avatar::SetIsShot(bool isshot)
{
	if (!m_IsShot && !m_Beginning)
	{
		m_IsShot = isshot;
		if (m_IsShot)
		{
			m_pAvatarDeadSound->Play(0);
			m_CountPlayerLives++;
			if (m_CountPlayerLives >= 3)m_CountPlayerLives = 3;
			m_Beginning = true;
		}
		
	}
}

Rectf Avatar::GetShape() const
{
	return m_DestRect;
}


void Avatar::InitSourceRect()
{
	m_SrcRect.width = m_SpriteSheetWidth / m_Cols;
	m_SrcRect.height = m_SpriteSheetHeight / m_Rows;
	UpdateSourceRect();
}
void Avatar::UpdateSourceRect()
{

	if (m_CurFrame >=m_NrFrames )
	{
		if (m_IsShot)
		{
			m_ActionState = ActionState::waiting;
			m_DestRect.bottom = m_InitialDestRectBottom;
			m_IsShot = false;
		}
		else if (m_ActionState == ActionState::lookRightDown || m_ActionState == ActionState::lookLeftDown)
		{
			if (m_CurFrame > 3 || m_CurFrame < 1)
			{
				m_CurFrame = 1;
			}
		}
	
		else m_CurFrame = 0;
		
	}

	m_SrcRect.left = m_ClipWidth * m_CurFrame;
	m_SrcRect.bottom = m_ClipHeight + (m_Cols + m_ChooseSprite) * m_SrcRect.height;
}
void Avatar::InitDestRect()
{
	const int  AvatarHeight = 11;
	const int AvatarWidth = 15;
	m_DestRect.width = m_SpriteSheetWidth / AvatarWidth;
	m_DestRect.height = m_SpriteSheetHeight /AvatarHeight;
	m_DestRect.left = m_StartPoint;
	m_DestRect.bottom = m_InitialDestRectBottom;
}

void Avatar::UpdateAvatarState(float elapsedSec)
{

	switch (m_ActionState)
	{
		if (!m_IsOnWater)
		{
			case ActionState::moving:
			{
				if (m_IsOnGround)
				{
					m_ActionState = ActionState::waiting;
				}
			}
			case ActionState::waiting:
			{
				const Uint8* pStates = SDL_GetKeyboardState(nullptr);
				if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_UP])
				{
					m_ActionState = ActionState::moving;
				}
				break;
			}
			case ActionState::jumping:
			{
				if (m_IsOnGround)
				{
					m_ActionState = ActionState::waiting;
				}
				break;
			}
		}
		else
		{
			case ActionState::swimming:
			{
				m_ChooseSprite = 11;
				m_NrFrames = 2;
				m_Velocity += m_Acceleration * elapsedSec;
				m_DestRect.left += m_Velocity.x * elapsedSec;
			}
			break;

			case ActionState::lookUpSwimming:
			{
				m_ShootDirection = ShootDirection::Right;
				HandleAvatarButtonInput();
				m_ChooseSprite = 8;
				m_NrFrames = 1;
				//level.HandleCollision(m_DestRect, m_Velocity);
			}
			break;

			case ActionState::lookRightUpSwimming:
			{
				m_ShootDirection = ShootDirection::RightUp;
				HandleAvatarButtonInput();
				m_ChooseSprite = 7;
				m_NrFrames = 1;
			}
			break;
			
			case ActionState::lookLeftUpSwimming:
			{
				m_ShootDirection = ShootDirection::LeftUp;
				HandleAvatarButtonInput();
				m_ChooseSprite = 7;
				m_NrFrames = 1;
			}
			break;

		}

	}
}
void Avatar::CountAccumulatedSecond(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_Beginning)
	{
		m_AccuSecBeginning += elapsedSec;
		if (m_AccuSecBeginning >= 2.f)
		{
			m_Beginning = false;
			m_AccuSecBeginning = 0;
		}
	}
	if (m_AccuSec > m_AnimeTime)
	{
		++m_CurFrame %= (m_Cols * m_Rows);
		m_AccuSec -= m_AnimeTime;
		UpdateSourceRect();
		if (m_CurFrame >= 2 && m_IsOnWater)
		{
			m_ActionState = ActionState::OnWaterWait;
		}
	}
}

void Avatar::UpdateContraStateMovement(float elapsedSec,const Level& level)
{
	if (m_IsShot)
	{
		m_ChooseSprite = 9;
 		m_NrFrames = 5; 
		level.HandleCollision(m_DestRect, m_Velocity);
		level.HandleCollisionOnBridge(m_DestRect, m_Velocity);
	}
	else
	{
		switch (m_ActionState)
		{
			if (!m_IsOnWater)
			{
				case ActionState::jumping:
				{
					if(m_ToggleLeftRight)m_ShootDirection = ShootDirection::Right;
					else m_ShootDirection = ShootDirection::Left;
					HandleAvatarButtonInput();
					m_ChooseSprite = 2;
					m_NrFrames = 4;
					CheckCollisionOnJump(elapsedSec, level);
					break;
				}
				case ActionState::moving:
				{
					if (m_ToggleLeftRight)m_ShootDirection = ShootDirection::Right;
					else m_ShootDirection = ShootDirection::Left;
					HandleAvatarButtonInput();
					ChangeVelocityBasedOnAcceleration(elapsedSec, level);
					m_ChooseSprite = 1;
					m_NrFrames = 6;
					break;
				}
				case ActionState::waiting:
				{
					if (m_ToggleLeftRight)m_ShootDirection = ShootDirection::Right;
					else m_ShootDirection = ShootDirection::Left;
					m_ChooseSprite = 3;
					m_NrFrames = 1;
					break;
				}
				case ActionState::proning:
				{
					if (m_ToggleLeftRight)m_ShootDirection = ShootDirection::Right;
					else m_ShootDirection = ShootDirection::Left;
					m_ChooseSprite = 4;
					m_NrFrames = 1;
					break;
				}
				case ActionState::lookUp:
				{
					m_ShootDirection = ShootDirection::Up;
					m_ChooseSprite = 10;
					m_NrFrames = 1;
					break;
				}
				case ActionState::lookRightUp:
				{
					m_ShootDirection = ShootDirection::RightUp;
				
					HandleAvatarButtonInput();

					m_ChooseSprite = 5;
					m_NrFrames = 6;
					m_Velocity += m_Acceleration * elapsedSec;
					m_DestRect.left += m_Velocity.x * elapsedSec;
					break;
				}
				case ActionState::lookRightDown:
				{
					m_ShootDirection = ShootDirection::RightDown;
					
					ChangeVelocityBasedOnAcceleration(elapsedSec, level);
					m_ChooseSprite = 8;
					m_NrFrames = 3;
					break;
				}
				case ActionState::lookleftUp:
				{
					m_ShootDirection = ShootDirection::LeftUp;
					ChangeVelocityBasedOnAcceleration(elapsedSec, level);
					m_ChooseSprite = 5;
					m_NrFrames = 6;
					break;
				}
				case ActionState::lookLeftDown:
				{
		            m_ShootDirection = ShootDirection::LeftDown;
					ChangeVelocityBasedOnAcceleration(elapsedSec, level);
					m_ChooseSprite = 8;
					m_NrFrames = 3;
					break;
				}
			}
			else
			{
				case ActionState::OnWaterWait:
				{
					m_ShootDirection = ShootDirection::Right;
					HandleAvatarButtonInput();
					m_ChooseSprite = 11;
					m_NrFrames = 1;
					level.HandleCollision(m_DestRect, m_Velocity);
					level.HandleCollisionOnBridge(m_DestRect, m_Velocity);
				}
						break;	
			}
		}
		
	}
}

Avatar::ActionState Avatar::GetActionState()
{
	return m_ActionState;
}

void Avatar::HandleAvatarButtonInputOnWater()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_UP])
	{
		m_ShootDirection = ShootDirection::Up;
		m_ActionState = ActionState::lookUpSwimming;
	}

	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_ShootDirection = ShootDirection::Right;
		m_ActionState = ActionState::swimming;
		m_ToggleLeftRight = true;
		m_Velocity.x = m_HorSpeed;
	}

	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_ShootDirection = ShootDirection::Left;
		m_ActionState = ActionState::swimming;
		m_ToggleLeftRight = false;
		m_Velocity.x = -m_HorSpeed;
	}
	if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_UP])
	{
		m_ShootDirection = ShootDirection::RightUp;
		m_ActionState = ActionState::lookRightUpSwimming;
		m_Velocity.x = m_HorSpeed;
	}
	
	if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	{
		m_ShootDirection = ShootDirection::LeftUp;
		m_ActionState = ActionState::lookLeftUpSwimming;
		m_Velocity.x = m_HorSpeed;
	}

}
void Avatar::HandleAvatarButtonInputOnGround()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (m_IsOnGround && pStates[SDL_SCANCODE_X])
	{
		m_ActionState = ActionState::jumping;
		m_Velocity.y = m_JumpSpeed;

	}

	if (m_IsOnGround && pStates[SDL_SCANCODE_UP])
	{
		m_ActionState = ActionState::lookUp;
	}

	if (m_IsOnGround && pStates[SDL_SCANCODE_DOWN])
	{
		m_ActionState = ActionState::proning;
	}

	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_ActionState = ActionState::moving;
		m_ToggleLeftRight = true;
		m_Velocity.x = m_HorSpeed;
	}

	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_ActionState = ActionState::moving;
		m_ToggleLeftRight = false;
		m_Velocity.x = -m_HorSpeed;
	}

	if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_UP])
	{
		m_ActionState = ActionState::lookRightUp;
		m_Velocity.x = m_HorSpeed;
	}

	if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_DOWN])
	{
		m_ActionState = ActionState::lookRightDown;
		m_Velocity.x = m_HorSpeed;
	}

	if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	{
		m_ActionState = ActionState::lookleftUp;
		m_Velocity.x = -m_HorSpeed;
	}

	if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_DOWN])
	{
		m_ActionState = ActionState::lookLeftDown;
		m_Velocity.x = -m_HorSpeed;
	}
}
void Avatar::CheckIfPlayerIsOnWater(Level& level)
{
	Point2f lowestVertex = level.GetLowestVertex();
	if (m_DestRect.bottom + m_DestRect.height < lowestVertex.y)
	{
		m_IsShot = true;
		m_CountPlayerLives += 1;
		m_DestRect.bottom = m_InitialDestRectBottom;
	}
	

	if (m_DestRect.bottom - lowestVertex.y <= 5.f && m_DestRect.left < m_WaterSectionOnLevel)
	{
		m_IsOnWater = true;
		m_ActionState = ActionState::OnWaterWait;
	}
	else
	{
		m_IsOnWater = false;
	}
}

void Avatar::CheckIfPlayerIsOnGround(const Level& level)
{
	m_IsOnWater = false;
	if (m_IsOnGround = level.IsOnGround(m_DestRect, m_Velocity))
	{

	}
	else
	{
		m_IsOnGround = level.IsOnBridge(m_DestRect, m_Velocity);
	}
	
	HandleAvatarButtonInput();
	if (!m_IsOnGround)
	{
		m_ActionState = ActionState::jumping;
	}
}

void Avatar::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_c:
		if (Bullet::m_AvatarPower <= 0)
		{
			m_pLevel1GunSound->Play(0);
		}
		else
		{ 
			m_pLevel2GunSound->Play(0);
		}
		
		switch (m_ShootDirection)
		{
		case Avatar::ShootDirection::Right:
			m_pBullet.push_back(new Bullet(Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom+m_DestRect.height/2), Vector2f(1.0f, 0.0f), "Avatar"));
		break;
		case Avatar::ShootDirection::Left:
			m_pBullet.push_back(new Bullet(Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2), Vector2f(-1.0f, 0.0f), "Avatar"));
			break;
		case Avatar::ShootDirection::RightUp:
			m_pBullet.push_back(new Bullet(Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2), Vector2f(1.0f, 1.0f), "Avatar"));
			break;
		case Avatar::ShootDirection::Up:
			m_pBullet.push_back(new Bullet(Point2f(m_DestRect.left+m_DestRect.width/2, m_DestRect.bottom + m_DestRect.height / 2), Vector2f(0.0f, 1.0f), "Avatar"));
			break;
		case Avatar::ShootDirection::RightDown:
			m_pBullet.push_back(new Bullet(Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2), Vector2f(1.0f, -1.0f), "Avatar"));
			break;
		case Avatar::ShootDirection::LeftUp:
			m_pBullet.push_back(new Bullet(Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2), Vector2f(-1.0f, 1.0f), "Avatar"));
			break;
		case Avatar::ShootDirection::LeftDown:
			m_pBullet.push_back(new Bullet(Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2), Vector2f(-1.0f, -1.0f), "Avatar"));
			break;
		
		}
		
		break;
	}
}

void Avatar::ChangeVelocityBasedOnAcceleration(float elapsedSec,const Level& level)
{
	m_Velocity += m_Acceleration * elapsedSec;
	m_DestRect.bottom += m_Velocity.y * elapsedSec;
	m_DestRect.left += m_Velocity.x * elapsedSec;
	if (m_Velocity.y <= 0)
	{
		level.HandleCollision(m_DestRect, m_Velocity);
		level.HandleCollisionOnBridge(m_DestRect, m_Velocity);
	}
}

void Avatar::CheckCollisionOnJump(float elapsedSec, const Level& level)
{
	m_Velocity += m_Acceleration * elapsedSec;
	m_DestRect.bottom += m_Velocity.y * elapsedSec;
	if (m_Velocity.x > 0)
	{
		m_DestRect.left += m_Velocity.x * elapsedSec;
	}

	if (m_Velocity.x < 0)
	{
		m_DestRect.left += m_Velocity.x * elapsedSec;
	}
	m_Velocity.x = 0;

	if (m_Velocity.y <= 0)
	{
		level.HandleCollision(m_DestRect, m_Velocity);
		level.HandleCollisionOnBridge(m_DestRect, m_Velocity);
	}
}


void Avatar::UpdateBullet(float elapsedSec)
{
	for ( size_t i = 0; i < m_pBullet.size(); i++)
	{
		if (!m_pBullet[i]->GetBulletDetroyed())
		{
			
			m_pBullet[i]->Update(elapsedSec);
			m_pBullet[i]->CheckBulletBoundary(m_CameraBoundary);
		}
	}
}

void Avatar::SetEnemyPos(Rectf enemyPos)
{
	m_EnemyPos = enemyPos;
}

void Avatar::DrawBullets() const
{
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		if (!m_pBullet[i]->GetBulletDetroyed())
		{
			m_pBullet[i]->Draw();
		}
	}
}

void Avatar::SetCameraBoundary(Rectf cameraBoundary)
{
	m_CameraBoundary = cameraBoundary;
}

Rectf Avatar::GetBulletPos() const
{
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		if (!m_pBullet[i]->GetBulletDetroyed())
		{
			return m_pBullet[i]->GetBulletPos();
		}
	}
	return Rectf();
}

int Avatar::GetCurrentLives() const
{
	return m_CountPlayerLives;
}

void Avatar::UpdateIfEnemyIsDestroyed(std::vector<Enemy*> pEnemy)
{
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		for (size_t j = 0; j < pEnemy.size(); j++)
		{
			if (utils::IsOverlapping(m_pBullet[i]->GetBulletPos(), pEnemy[j]->GetPos()))
			{
				if (!pEnemy[j]->GetEnemyIsHit())
				{
					pEnemy[j]->SetEnemyIsHit(true);
				}
				
				if (!m_pBullet[i]->GetBulletDetroyed())
				{
					m_pBullet[i]->SetDestroyBullet(true);
				}
			}

		}
	}
}

void Avatar::UpdateNumberOfLives(int lives)
{
	m_CountPlayerLives = 0;
}