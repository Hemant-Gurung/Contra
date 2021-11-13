#pragma once
#include "Texture.h"
#include <vector>
#include "structs.h"
#include "SoundStream.h"
class Intro final
{
public:
	Intro();
	~Intro();
	
	Intro(const Intro&) = delete;
	Intro& operator=(const Intro&) = delete;
	Intro(Intro&&) = delete;
	Intro& operator=(Intro&&) = delete;

	void StartPlaying();
	void Update(float elapsedSec);
	void StopIntroMusic();
private:
	std::vector<Texture*> m_pIntroTexrture;
	bool m_TogglePlayButton;
	SoundStream* m_pSoundStream;
	Rectf m_DestRect;
	const float m_WindowWidth{};
};