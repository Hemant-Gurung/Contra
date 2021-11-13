#include "pch.h"
#include "Intro.h"

Intro::Intro()
:m_TogglePlayButton{false}
, m_pSoundStream{ new SoundStream("Resources/INTRO/title.mp3") }
,m_WindowWidth{530.f}
{
	m_pIntroTexrture.push_back(new Texture("Resources/INTRO/Intro1.png"));
	m_pIntroTexrture.push_back(new Texture("Resources/INTRO/Intro2.png"));
	
	
	m_DestRect.left = m_WindowWidth;
	m_DestRect.bottom = 0.f;;
	m_DestRect.width = m_pIntroTexrture[0]->GetWidth();
	m_DestRect.height = m_pIntroTexrture[0]->GetHeight();
	//m_pSoundStream->Play(true);
}

Intro::~Intro()
{
	for (size_t i = 0; i < m_pIntroTexrture.size(); i++)
	{
		delete m_pIntroTexrture[i];
		m_pIntroTexrture[i] = nullptr;
	}
	delete m_pSoundStream;

}

void Intro::StartPlaying()
{
	
	if (m_DestRect.left <= 0)
	{
		if (m_TogglePlayButton)
		{
			m_pIntroTexrture[0]->Draw(Rectf{});
			m_TogglePlayButton = false;
			Sleep(300);
		}
		else
		{
			m_pIntroTexrture[1]->Draw(Rectf{});
			m_TogglePlayButton = true;
			Sleep(300);
		}
	}
	else
	{
		m_pIntroTexrture[0]->Draw(m_DestRect);

	}
}

void Intro::Update(float elapsedSec)
{
	if (m_DestRect.left <= 0.f)
	{
		
	}
	else m_DestRect.left --;
}

void Intro::StopIntroMusic()
{
	m_pSoundStream->Stop();
}
