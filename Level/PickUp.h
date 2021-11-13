#pragma once
#include "structs.h"
#include "Texture.h"
#include "SoundEffect.h"
class PickUp
{
	public:
		PickUp(Point2f pos,std::string path);
		~PickUp();
		PickUp(const PickUp&) = delete;
		PickUp& operator=(const PickUp&) = delete;
		PickUp(PickUp&&) = delete;
		PickUp& operator=(PickUp&&) = delete;
		void Draw() const;
		void Update(float elapsedSec);
		Point2f GetPos();
		void SetActorPos(Rectf actorPos);
		bool IsHit(Rectf bulletPos);
		bool IsTouched(Rectf actorPos);
		bool m_IsHit;
	private:
		void UpdateSrcRect(float elapsedSec);
		Point2f m_Pos;
		Texture* m_pPickUpBox;
		Texture* m_pPickUpTexture;
		Rectf m_SrcRect;
		float m_ClipWidth;
		float m_ClipHeight;
		std::string m_FilePath;
		bool m_IsActorInRange{};
		float m_PickUpPosition;
		bool m_IsTouched;
		SoundEffect* m_pPowerUpSound;
		SoundEffect* m_pPowerBoxSound;
};