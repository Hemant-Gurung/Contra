#include "pch.h"
#include "Score.h"
#include "structs.h"
Score::Score()
	:m_pScore{}
{
	
}

Score::~Score()
{
	delete m_pScore;
	m_pScore = nullptr;
}

void Score::Draw()
{
	if (m_pScore != nullptr)
	{
		m_pScore->Draw(Point2f(20.f, 20.f));
	}
}

void Score::Update(int score)
{
	if (m_pScore != nullptr)
	{
		delete m_pScore;
	}
	m_Score = score;
	m_pScore = new Texture("Score: " + std::to_string(m_Score), "Resources/AtariClassicChunky-PxXP.ttf", 20, Color4f(1, 1, 1, 0.5f));
}
