#include "pch.h"
#include "Level.h"
#include "utils.h"
#include <iostream>
Level::Level()
:m_pBackgroundTextureOne{new Texture("Resources/Background/Backgroundnew.png") }
,m_Vertices{}
,m_ActorShape{}
,m_BridgePieces{3}
,m_EnemyPos{}
,m_CameraBoundary{}
,m_ToggleBackground{ false }
,m_AccuSec{}
, m_pBackgroundTextureTwo{new Texture("Resources/Background/BackgroundnewTwo.png") }
,m_TimeToWait{1.f}
{
	Initialize();
}
 
void Level::Initialize()
{
	float BridgePosX{ 1600.f };
	const float BridgePosY{ 200.f };
	for (int i = 0; i <= m_BridgePieces; i++)
	{
		m_pBridge.push_back(new Bridge(Point2f(BridgePosX,BridgePosY),"Resources/Bridge/BridgePiece" + std::to_string(i + 1) + ".png"));
		BridgePosX += 67.f;
	}
	 BridgePosX= 2199.f ;
	 for (int i = 0; i <= m_BridgePieces; i++)
	 {
		 m_pBridge.push_back(new Bridge(Point2f(BridgePosX, BridgePosY), "Resources/Bridge/BridgePiece" + std::to_string(i + 1) + ".png"));
		 BridgePosX += 67.f;
	 }
	bool getVertices = m_pSvgParser.GetVerticesFromSvgFile("Resources/NEW_Background.svg", m_Vertices);
	m_Smallest = m_Vertices.at(0);
	m_Boundaries = Rectf{ 0.f,0.f,m_pBackgroundTextureOne->GetWidth(),m_pBackgroundTextureOne->GetHeight() };
	
	InitializeEnemies();
	//PickUps
	m_pPickUps.push_back(new PickUp(Point2f(650.f, 150.f), "Resources/PickUps/PickUpBox.png"));
	m_pPickUps.push_back(new PickUp(Point2f(2742.f, 100.f), "Resources/PickUps/PickUpBox.png"));
}
void Level::InitializeEnemies()
{
	m_pEnemy.reserve(32);
	//Enemies Initialized
	m_pEnemy.push_back(new Enemy(Point2f(642.f, 48.f), "Resources/Enemy/Enemy.png"));
	m_pEnemy.push_back(new Enemy(Point2f(1276.f, 48.f), "Resources/Enemy/Enemy.png"));
	m_pEnemy.push_back(new Enemy(Point2f(2751.f, 251.f), "Resources/Enemy/Enemy.png"));
	m_pEnemy.push_back(new Enemy(Point2f(2955.f, 317.f), "Resources/Enemy/Enemy.png"));
	m_pEnemy.push_back(new Enemy(Point2f(5079.f, 117.f), "Resources/Enemy/Enemy.png"));

	m_pEnemy.push_back(new EnemyTank(Point2f(2500.f, 165.f), "Resources/Enemy/MachinaEnemysprites.png"));
	m_pEnemy.push_back(new EnemyTank(Point2f(1023.f, 165.f), "Resources/Enemy/MachinaEnemysprites.png"));
	m_pEnemy.push_back(new EnemyTank(Point2f(3054.f, 50.f), "Resources/Enemy/MachinaEnemysprites.png"));
	m_pEnemy.push_back(new EnemyTank(Point2f(3365.f, 245.f), "Resources/Enemy/MachinaEnemysprites.png"));
	m_pEnemy.push_back(new EnemyTank(Point2f(3722.f, 245.f), "Resources/Enemy/MachinaEnemysprites.png"));
	m_pEnemy.push_back(new EnemyTank(Point2f(5688.f, 150.f), "Resources/Enemy/MachinaEnemysprites.png"));
	
	//Running
	m_pEnemy.push_back(new Enemy(Point2f(5407.f, 250.f), "Resources/Enemy/Enemy.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(600.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(800.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(670.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(700.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(1471.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(1475.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(2070.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(2073.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(2065.f, 250.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(3319.f, 317.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(3335.f, 317.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(3339.f, 317.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(3350.f, 317.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(4086.f, 247.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(4095.f, 247.f), "Resources/Enemy/EnemyRunning.png"));
	m_pEnemy.push_back(new EnemyRunning(Point2f(4501.f, 317.f), "Resources/Enemy/EnemyRunning.png"));

	//Boss
	m_pEnemy.push_back(new EnemyBoss(Point2f(6717.f, 0), "Resources/Enemy/EnemyBoss.png"));
}
Level::~Level()
{
	delete m_pBackgroundTextureOne;
	m_pBackgroundTextureOne = nullptr;
	delete m_pBackgroundTextureTwo;
	m_pBackgroundTextureTwo = nullptr;
	
	for (size_t i = 0; i < m_pBridge.size(); i++)
	{
		delete m_pBridge[i];
		m_pBridge[i] = nullptr;
	}
	
	
	for (size_t i = 0; i < m_pPickUps.size(); i++)
	{
		delete m_pPickUps[i];
	}
	m_pPickUps.clear();

	for (size_t i = 0; i < m_pEnemy.size(); i++)
	{
		delete m_pEnemy[i];
	}
	m_pEnemy.clear();
}

void Level::Draw() const
{
	if (m_ToggleBackground)
	{
		m_pBackgroundTextureOne->Draw(Point2f(0.f, 0.f));
	}
	else
	{ 
		m_pBackgroundTextureTwo->Draw(Point2f(0.f, 0.f));
	}
	DrawEnemy();
	DrawPickUps();
	for (size_t i = 0; i < m_pBridge.size(); i++)
	{
		m_pBridge[i]->Draw();
	}
	
}

bool Level::UpdateIfPlyaerIsShot(Rectf actorpos)
{
	for (size_t i = 0; i < m_pEnemy.size(); i++)
	{
		if (m_pEnemy[i]->UpdateIfPlayerIsShot(actorpos))
		{
			return true;
		}
	}
	return false;

}

bool Level::UpdateIfEnemyIsHit()
{
	for (size_t i = 0; i < m_pEnemy.size(); i++)
	{
		return m_pEnemy[i]->GetEnemyIsHit();
	}
	return false;
}
Rectf Level::GetEnemyPos() const
{
	return m_EnemyPos;
}


void Level::Update(float elapsedSec, Rectf actorPos, Rectf actorBulletPos)
{
	for (size_t i = 0; i < m_pBridge.size(); i++)
	{
		m_pBridge[i]->Update(elapsedSec);
		m_pBridge[i]->SetAvatarPos(actorPos);
	}

	UpdateIfPlyaerIsShot(actorPos);
	for (size_t i = 0; i < m_pEnemy.size(); i++)
	{
		if (m_pEnemy[i]->GetActorIsInRange(actorPos))
		{
			m_pEnemy[i]->SetCameraBoundary(m_CameraBoundary);
			m_pEnemy[i]->Update(elapsedSec);
			UpdateIfEnemyIsHit();
			m_pEnemy[i]->FindActorLocation(actorPos);
			m_EnemyPos = m_pEnemy[i]->GetPos();

			Vector2f velocity = m_pEnemy[i]->GetVelocity();
			if (velocity != Vector2f{ 0,0 })
			{
				HandleCollision(m_EnemyPos, velocity);
				if (IsOnGround(m_EnemyPos, velocity))
				{
					m_pEnemy[i]->IsOnGround(true);
				}
				else
				{
					m_pEnemy[i]->IsOnGround(false);
				}
			}
		}

	}

	for (size_t i = 0; i < m_pPickUps.size(); i++)
	{
		m_pPickUps[i]->Update(elapsedSec);
		m_pPickUps[i]->IsHit(actorBulletPos);
		m_pPickUps[i]->IsTouched(actorPos);
	}

	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_TimeToWait)
	{
		if(m_AccuSec>m_TimeToWait*2) m_AccuSec = 0;
		m_ToggleBackground = true;
	}
	else
	{
		m_ToggleBackground = false;
	}
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	Point2f rayStart{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
	Point2f rayEnd{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom };

	utils::HitInfo h;

	if (actorVelocity.y <= 0)
	{
		for (size_t i = 0; i < m_Vertices.size(); i++)
		{

			if (utils::Raycast(m_Vertices.at(i), rayStart, rayEnd, h))
			{
				actorShape.bottom = h.intersectPoint.y;
				actorVelocity.y = 0.0f;
			}
		}
	}

}

void Level::HandleCollisionOnBridge(Rectf& actorShape, Vector2f& actorVelocity) const
{
	for (size_t i = 0; i < m_pBridge.size(); i++)
	{
		if (!m_pBridge[i]->GetIsDestroyed())
		{
			Rectf BridgePos = m_pBridge[i]->GetBridgePos();
			if(utils::IsOverlapping(actorShape, BridgePos))
			{
				actorVelocity.y = 0.0f;
			}
		}

	}
}

bool Level::IsOnBridge(const Rectf& actorShape, Vector2f& actorVelocity) const
{
	Rectf BridgePos = m_pBridge[0]->GetBridgePos();
	if (BridgePos.left - actorShape.left < 50)
	{
		for (size_t i = 0; i < m_pBridge.size(); i++)
		{
			if (!m_pBridge[i]->GetIsDestroyed())
			{
				Rectf BridgePos = m_pBridge[i]->GetBridgePos();
				if (utils::IsOverlapping(actorShape, BridgePos))
				{
					return true;
				}
			}

		}
	}
	return false;
}

Point2f Level::GetLowestVertex()
{
	
	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		if (m_Smallest.at(0).y > m_Vertices.at(i).at(0).y)
		{
			m_Smallest.at(0).y = m_Vertices.at(i).at(0).y;
		}
		if (m_Smallest.at(1).y > m_Vertices.at(i).at(1).y)
		{
			m_Smallest.at(1).y = m_Vertices.at(i).at(1).y;
		}
		
	}
	return Point2f{ m_Smallest.at(0).y,m_Smallest.at(1).y };
}

bool Level::IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity) const
{
		
		Point2f rayStart{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height };
		Point2f rayEnd{ actorShape.left + (actorShape.width / 2.0f), actorShape.bottom - 1 };
		
		utils::HitInfo h;
		if (actorVelocity.y <= 0)
		{
			for (size_t i = 0; i < m_Vertices.size(); i++)
			{
				if (utils::Raycast(m_Vertices.at(i), rayStart, rayEnd, h))
				{
					return true;
				}
			}
		}
		return false;
		
}

Point2f Level::Getshape() const
{
	return Point2f{ m_pBackgroundTextureOne->GetWidth(),m_pBackgroundTextureOne->GetHeight() };
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

void Level::UpdatePickUps(float elapsedSec)
{
	//update if powerup box is hit
	for (size_t i = 0; i < m_pPickUps.size(); i++)
	{
		m_pPickUps[i]->Update(elapsedSec);
	
	}
	
}

void Level::SetCameraBoundary(Rectf camBoundary)
{
	m_CameraBoundary = camBoundary;
}

void Level::DrawPickUps() const
{
	for (size_t i = 0; i < m_pPickUps.size(); i++)
	{
		m_pPickUps[i]->Draw();
	}
}

void Level::DrawEnemy() const
{
	for (size_t i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Draw();
	}
}

std::vector<Enemy*> Level::GetEnemyList()
{
	return m_pEnemy;
}
