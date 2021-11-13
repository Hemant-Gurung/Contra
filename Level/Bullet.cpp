#include "pch.h"
#include "Bullet.h"
#include "utils.h"
#include <iostream>

 int Bullet::m_AvatarPower{0};
Bullet::Bullet(Point2f pos,Vector2f direction, std::string actorType)
	:m_Pos{pos}
	,m_ActorType{actorType}
	, m_DestroyBullet{ false }
	,m_CurrFrame{}
	,m_AccuSec{}
	, m_BulletVelocity{ direction }
	, m_Acceleration{ Vector2f(0,-981.f) }
	, m_IsProning{ false }
	,m_NrFrames{4}
	,m_BulletRotateAngle{}
	,m_AvatarPos{}
	,m_pBulletTexture{}
	,m_ToggleLeftRight{false}
	,m_HorSpeed{500.f}
	,m_verticalSpeed{400.f}
{
	InitializeBulletTexture();
	m_NrFramePerSec = 4;
	m_AnimeTime = 1 / float(m_NrFramePerSec);
}
void Bullet::InitializeBulletTexture()
{
	if (m_ActorType == "Avatar")
	{
		if (m_AvatarPower <= 0)
		{
			m_pBulletTexture = new Texture("Resources/Bullet1.png");
		}
		else if (m_AvatarPower >= 1)
		{
		m_pBulletTexture = new Texture("Resources/BulletLevel2.png");
		}
	}
	else if (m_ActorType == "EnemyRed")
	{
		m_pBulletTexture = new Texture("Resources/Enemy/Bullet1.png");
	}
	else if (m_ActorType == "EnemyBoss")
	{
		m_pBulletTexture = new Texture("Resources/Enemy/BulletLevel2.png");
	}
}

Bullet::~Bullet()
{
	delete m_pBulletTexture;
	m_pBulletTexture = nullptr;
}

void Bullet::Draw()
{
	DrawEnemyBullet();
	DrawAvatarBullet ();
}

bool Bullet::CheckBulletCollision()
{
	if (m_ActorType == "EnemyRed" || m_ActorType =="EnenmyBoss")
	{
		if (utils::IsPointInRect(Point2f(m_Pos.x, m_Pos.y),Rectf( m_AvatarPos.left,m_AvatarPos.bottom,m_AvatarPos.width/2,m_AvatarPos.height/2)))
		{
			return true;
		}
	}
	return false;
	
}

Rectf Bullet::GetBulletPos() const
{
	if (!m_DestroyBullet )
	{
		return Rectf(m_Pos.x, m_Pos.y, m_pBulletTexture->GetWidth()/4, m_pBulletTexture->GetHeight());
	}
	return Rectf{ 0.f,0.f,.0f,0.f };
}

void Bullet::SetAvatarPos(const Rectf& avatarpos)
{
	m_AvatarPos = avatarpos;
}

void Bullet::CheckAvatarFacingDirection(const bool avatarDirection) 
{
	m_ToggleLeftRight = avatarDirection;
}
void Bullet::Update(float elapsedSec)
{
	const int bulletRotateSpeed = 500;
	m_BulletRotateAngle+= bulletRotateSpeed * elapsedSec;
	UpdateAvatarBullet(elapsedSec);
}



bool Bullet::CheckBulletBoundary(const Rectf& CameraBoundary)
{
	if (!utils::IsOverlapping(Rectf(m_Pos.x, m_Pos.y, m_pBulletTexture->GetWidth() / 4, m_pBulletTexture->GetHeight()), CameraBoundary))
	{
		m_DestroyBullet = true;
	}
	else
	{
		m_DestroyBullet = false;
	}
	return m_DestroyBullet;
}


void Bullet::CheckCurrentFrame(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_AnimeTime)
	{
		m_CurrFrame++;
		
		if (m_CurrFrame > m_NrFrames)
		{
			m_CurrFrame = 0;
		}
	}
}

void Bullet::SetBulletPowerUp(const int& powerUp)
{
	if (powerUp > 0)
	{
		m_AvatarPower++;
		if (m_AvatarPower > 4)m_AvatarPower = 1;
	}
	else
	{
		m_AvatarPower = 0;
	}
}

void Bullet::DrawEnemyBullet()
{
	if (!m_DestroyBullet && (m_ActorType == "EnemyRed" || m_ActorType == "EnemyBoss"))
	{
		float TextWidth{ m_pBulletTexture->GetWidth() }, TextHeight{ m_pBulletTexture->GetHeight() };
		m_pBulletTexture->Draw(Point2f{ m_Pos.x, m_Pos.y }, Rectf(0, TextHeight, TextWidth, TextHeight));
	}

}
void Bullet::DrawAvatarBullet()
{
	if (!m_DestroyBullet && m_ActorType == "Avatar")
	{
		float TextWidth{ m_pBulletTexture->GetWidth() }, TextHeight{ m_pBulletTexture->GetHeight() };
		glPushMatrix();
		glTranslatef((m_Pos.x+TextWidth), (m_Pos.y+TextHeight), 0);
		glRotatef(m_BulletRotateAngle, 0, 0, 1);
		glTranslatef(-TextWidth/2, -TextHeight/2, 0);
		m_pBulletTexture->Draw(Point2f{}, Rectf(0, TextHeight, TextWidth, TextHeight));
		glPopMatrix();
	}

} 

void Bullet::UpdateAvatarBullet(float elapsedSec)
{
	const float EnemyBulletSpeedVertical{ 70.f };
	const float EnemyBulletSpeedHorizontal = 70.f;

	const float EnemyBossBulletSpeedVertical{ 200.f };
	const float EnemyBossBulletSpeedHorizontal = 500.f;

	const float AvatarBulletSpeedVertical{ 400.f };
	const float AvatarBulletSpeedHorizontal = 500.f;
	if (m_ActorType == "EnemyBoss")
	{
		m_HorSpeed = EnemyBossBulletSpeedHorizontal;
		m_verticalSpeed = EnemyBossBulletSpeedVertical;
	}
	
	else if (m_ActorType == "EnemyRed" )
	{
		m_HorSpeed = EnemyBulletSpeedHorizontal;
		m_verticalSpeed = EnemyBulletSpeedVertical;
	}
	else
	{
		m_verticalSpeed =AvatarBulletSpeedVertical;
		m_HorSpeed = AvatarBulletSpeedHorizontal;
	}
	 
	if (!m_DestroyBullet)
	{
		{
			m_Pos.x += m_BulletVelocity.x * elapsedSec * m_HorSpeed;
			m_Pos.y += m_BulletVelocity.y * elapsedSec * m_verticalSpeed;
		}
	}
}

void Bullet::SetDestroyBullet(const bool isBulletDestroyed)
{
	m_DestroyBullet = isBulletDestroyed;
}

bool Bullet::GetBulletDetroyed() const
{
	return m_DestroyBullet;
}