#pragma once
#include "Texture.h"
class Score final
{
public:
	Score();
	~Score();

	Score(const Score&) = delete;
	Score& operator=(const Score&) = delete;
	Score(Score&&) = delete;
	Score& operator=(Score&&) = delete;

	void Draw();
	void Update(int score);

private:
	int m_Score;
	Texture* m_pScore;
};