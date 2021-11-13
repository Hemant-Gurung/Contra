#pragma once
#include "structs.h"
#include"Texture.h"
#include <vector>
#include "SoundEffect.h"

class Bridge final
{
public:
	Bridge(Point2f pos,std::string path);
	~Bridge();

	Bridge(const Bridge&) = delete;
	Bridge& operator=(const Bridge&) = delete;
	Bridge(Bridge&&) = delete;
	Bridge& operator=(Bridge&&) = delete;

	void Draw();
	void Update(float elapsedSec);
	void SetAvatarPos(const Rectf& avatarPos);
	Rectf GetBridgePos() const ;
	bool GetIsDestroyed() const ;
private:
	Point2f m_BridgePos;
	std::string m_filePath;
	Texture* m_pBridgeTexture;
	int m_BridgePieces{};
	Rectf m_AvaratPos;
	Texture* m_pExplodingTexture;
	Rectf m_DestRect;
	int m_CurrFrame;
	float m_AnimeTime;
	float m_AccuSec;

	float m_ClipWidth{};
	float m_ClipHeight{};

	bool m_IsDetroyed{ false };
	bool m_StartExplosion;
	SoundEffect* m_pBridgeExplodingSound;
};