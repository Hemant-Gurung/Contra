#pragma once
#include "SoundEffect.h"
#include "SoundStream.h"
#include "Level.h"
#include "Hud.h"
#include"Score.h"
#include "Avatar.h"
#include "Bullet.h"
#include "PickUp.h"
#include "Enemy.h"
#include "Intro.h"
#include "EnemyTank.h"


class Camera;
class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();
	void Update( float elapsedSec );
	void Draw( ) const;
	static int m_CountScore;
	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;
	Level m_Level;
	Avatar m_Avatar;
	Rectf m_AvatarPos;
	bool m_ShootBullet{false};
	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
	void UpdateScore();
	void UpdateRestartGame();
	void UpdatGameEnd();
	//pointers
	SoundStream* m_pBackGroundSound;
	SoundStream* m_pGameOverSound;
	Camera* m_pCamera;
	Hud* m_pHud;
	Score* m_pScore;
	Intro* m_pIntro;
	std::string m_Score;
	bool m_StartGame;
	
};