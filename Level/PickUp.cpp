#include "pch.h"
#include "PickUp.h"
#include "utils.h"
#include "Bullet.h"
#include <iostream>
PickUp::PickUp(Point2f pos,std::string path)
	:m_Pos{pos},
	m_pPickUpBox{new Texture(path)}
	,m_pPickUpTexture{new Texture("Resources/PickUps/Pickups.png")}
	,m_SrcRect{}
	,m_FilePath{path}
	,m_IsActorInRange{false}
	,m_IsHit{false}
	,m_PickUpPosition{250.f}
	,m_IsTouched{false}
	,m_pPowerUpSound{new SoundEffect("Resources/Sounds/1-up.mp3")}
	,m_pPowerBoxSound{ new SoundEffect("Resources/Sounds/enemy-hit.mp3") }
{
		m_ClipWidth = m_pPickUpBox->GetWidth();
		m_ClipHeight = m_pPickUpBox->GetHeight();
}
PickUp::~PickUp()
{
	delete m_pPickUpBox;
	m_pPickUpBox = nullptr;

	delete m_pPickUpTexture ;
	m_pPickUpTexture = nullptr;

	delete m_pPowerUpSound;
	m_pPowerUpSound = nullptr;

	delete m_pPowerBoxSound;
	m_pPowerBoxSound = nullptr;
}

void PickUp::Draw() const
{
	if(!m_IsHit)m_pPickUpBox->Draw(m_Pos, m_SrcRect);
	else
	{
		if(!m_IsTouched)m_pPickUpTexture->Draw(Rectf(m_Pos.x,m_Pos.y,40.f,40.f), m_SrcRect);
	}
}

void PickUp::Update(float elapsedSec)
{
	UpdateSrcRect(elapsedSec);
}


Point2f PickUp::GetPos()
{
	return m_Pos;
}
void PickUp::SetActorPos(Rectf actorPos)
{
		if (actorPos.left - m_Pos.x <= 100)
		{
			m_IsActorInRange = true;
		}
		else
		{
			m_IsActorInRange = false;
		}
}

bool PickUp::IsHit(Rectf bulletPos)
{
	
	 if (utils::IsOverlapping(Rectf(m_Pos.x,m_Pos.y,m_ClipWidth,m_ClipHeight),bulletPos))
	  {
		 m_IsHit = true;
		 m_pPowerBoxSound->Play(0);
		 m_ClipWidth = m_pPickUpTexture->GetWidth() / 10;
		 m_ClipHeight = m_pPickUpTexture->GetHeight();
		 return true;
	  }
	return false;
}

bool PickUp::IsTouched(Rectf actorPos)
{
	if (m_IsHit && !m_IsTouched)
	{
		if (utils::IsOverlapping(Rectf(m_Pos.x, m_Pos.y, m_ClipWidth, m_ClipHeight),actorPos))
		{
			std::cout << "increase POWER..\n";
			m_IsTouched = true;
			Bullet::m_AvatarPower++;
			std::cout << Bullet::m_AvatarPower << '\n';
			m_pPowerUpSound->Play(0);
		}
		else
		{
			m_IsTouched = false;
		}
	}
	return m_IsTouched;
}

void PickUp::UpdateSrcRect(float elapsedSec)
{
	if (m_IsHit)
	{
		if (m_Pos.y <= m_PickUpPosition)
		{
			m_Pos.y++;
		}
		m_SrcRect.left = 1;
		m_SrcRect.bottom = m_ClipHeight;
	}
	else
	{
		m_SrcRect.left = 0;
		m_SrcRect.bottom = m_ClipHeight;

	}
	m_SrcRect.width = m_ClipWidth;
	m_SrcRect.height = m_ClipHeight;
}