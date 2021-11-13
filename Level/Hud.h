#pragma once
#include "structs.h"
#include "Texture.h"
class Hud final
{
public:
	Hud(const Point2f& topleft,int totalLives);
	~Hud();
	
	Hud(const Hud&) = delete;
	Hud& operator=(const Hud&) = delete;
	Hud(Hud&&) = delete;
	Hud& operator=(Hud&&) = delete;

	void Draw();
	void Update(const Point2f& topleft);
	void UpdateLives(const int lives);
	void ResetNumberOfLives(const int lives);
	int GetNumberOfRemainingLives() const;
private:
	int m_TotalLives;
	Point2f m_TopLeft;
	Texture* m_pLifeTexture;
	float m_ClipWidth{};
	float m_ClipHeight{};
	int m_CurrentLives;
	
};