//Gurung Hemant 1DAE09
#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include <iostream>
#include "utils.h"
int Game::m_CountScore{ 0 };
Game::Game( const Window& window ) 
	:m_Window{ window }
	,m_pCamera{ new Camera{window.width,window.height} }
	,m_pBackGroundSound{new SoundStream("Resources/sounds/stage_1.mp3")}
	,m_pGameOverSound{new SoundStream("Resources/sounds/game-over.mp3")}
	,m_StartGame{ false }
    ,m_pScore{nullptr}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	const int HorBoundary{ 20};
	const int VerBoundary{ 100};
	std::cout << "Press i to see Controls: \n";
	m_pIntro = new Intro();
	m_pScore = new Score();
	m_pHud = new Hud(Point2f(m_pCamera->GetCameraBoundaries().left + HorBoundary,
			(m_pCamera->GetCameraBoundaries().bottom + m_pCamera->GetCameraBoundaries().height - VerBoundary)), 3);
	
}

void Game::Cleanup( )
{
	delete m_pCamera;
	m_pCamera = nullptr;
	
	delete m_pBackGroundSound; 

	delete m_pHud;
	m_pHud = nullptr;

	delete m_pIntro;
	m_pIntro = nullptr;

	delete m_pScore;
	m_pScore = nullptr;
	
	delete m_pGameOverSound;
	m_pGameOverSound = nullptr;
}

void Game::Update( float elapsedSec )
{
	
	if (m_StartGame)
	{
		m_AvatarPos = m_Avatar.GetShape();
		m_Avatar.Update(elapsedSec, m_Level);
		m_Level.Update(elapsedSec, m_AvatarPos, m_Avatar.GetBulletPos());
		m_Level.SetCameraBoundary(m_pCamera->GetCameraBoundaries());
		m_Avatar.UpdateIfEnemyIsDestroyed(m_Level.GetEnemyList());
		m_Avatar.SetEnemyPos(m_Level.GetEnemyPos());
		m_pCamera->SetLevelBoundaries(Rectf(0.f, 0.f, m_Level.GetBoundaries().width, m_Level.GetBoundaries().height));
		m_Avatar.SetCameraBoundary(m_pCamera->GetCameraBoundaries());
		m_Avatar.SetIsShot(m_Level.UpdateIfPlyaerIsShot(m_AvatarPos));
		m_pHud->Update(Point2f(m_pCamera->GetCameraBoundaries().left + 20.f,
		m_pCamera->GetCameraBoundaries().bottom + m_pCamera->GetCameraBoundaries().height - 50.f));
		m_pHud->UpdateLives(m_Avatar.GetCurrentLives());

		UpdateScore();
		UpdateRestartGame();
		UpdatGameEnd();
	
		
	}
	else
	{
		m_pIntro->Update(elapsedSec);
	}
}

void Game::Draw( ) const
{
	if (m_StartGame)
	{
		ClearBackground();
		glPushMatrix();
		m_pCamera->Transform(m_AvatarPos);
		m_Level.Draw();
		m_Avatar.Draw();
		m_pHud->Draw();
		glPopMatrix();
	}
	else
	{
		m_pIntro->StartPlaying();
		if (m_pScore!= nullptr)
		{
			m_pScore->Draw();
		} 
	}
}

void Game::UpdateScore()
{
	if (int(m_AvatarPos.left + m_AvatarPos.width) % 1000 == 0)
	{
		m_CountScore += 1000;
	}


}
void Game::UpdateRestartGame()
{
	if (m_pHud->GetNumberOfRemainingLives() <= 0)
	{
		m_pScore->Update(m_CountScore);
		m_StartGame = false;
		m_pGameOverSound->Play(0);
		m_Avatar.UpdateNumberOfLives(0);
		m_pHud->ResetNumberOfLives(3);
		Bullet::m_AvatarPower = 0;
		m_CountScore = 0;
	}

}

void Game::UpdatGameEnd()
{
	if ((m_AvatarPos.left + m_AvatarPos.width / 2) >= m_Level.GetBoundaries().width)
	{
		m_StartGame = false;
		m_pBackGroundSound->Stop();
		m_pGameOverSound->Play(0);
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_c:
	
		m_Avatar.ProcessKeyUpEvent(e);

		break;

	case SDLK_i:
		std::cout << "s : TO START..\n";
		std::cout << "x : TO JUMP..\n";
		std::cout << "c :TO SHOOT..\n";
		std::cout << "Right left arrow to shoot up,left right,and combination\n";
		break;
	case SDLK_s:
		m_StartGame = true;
		//m_pIntro->StopIntroMusic();
	//	m_pBackGroundSound->Play(true);
		
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{

}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}




