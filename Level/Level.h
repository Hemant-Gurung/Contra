#pragma once
#include "structs.h"
#include <vector>
#include "Texture.h"
#include "Vector2f.h"
#include "SVGParser.h"
#include "PickUp.h"
#include "Enemy.h"
#include "EnemyRunning.h"
#include "EnemyTank.h"
#include "Bridge.h"
#include "EnemyBoss.h"
class Level final
{
public:
	Level();
	~Level();

	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;
	Level(Level&&) = delete;
	Level& operator=(Level&&) = delete;

	void Draw() const;
	void SetCameraBoundary(Rectf camBoundary);
	void Update(float elapsedSec,Rectf actorPos,Rectf bulletPos);
	void HandleCollision(Rectf& actorShape,Vector2f& actorVelocity) const;
	void HandleCollisionOnBridge(Rectf& actorShape, Vector2f& actorVelocity) const;
	std::vector<Enemy*>GetEnemyList();
	Point2f Getshape() const;
	Point2f GetLowestVertex();
	
	Rectf GetBoundaries() const;
	Rectf GetEnemyPos()const;
	//void SetEnemyIsHit(bool isHit);
	
	bool IsOnBridge(const Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity) const;
	bool UpdateIfEnemyIsHit();
	bool UpdateIfPlyaerIsShot(Rectf actorpos);
	
private:
	std::vector<std::vector<Point2f>> m_Vertices;
	Texture* m_pBackgroundTextureOne;
	Texture* m_pBackgroundTextureTwo;

	std::vector<Bridge*> m_pBridge;
	EnemyBoss* m_pEnemyBoss;

	SVGParser m_pSvgParser;
	Rectf m_Boundaries;
	std::vector<Point2f> m_Smallest;
	std::vector<PickUp*> m_pPickUps;
	std::vector <Enemy*> m_pEnemy;
	Rectf m_ActorShape;
	Rectf m_EnemyPos;
	Rectf m_CameraBoundary{};
	int m_BridgePieces{};
	bool m_ToggleBackground{};
	float m_AccuSec;
	float m_TimeToWait{};

	void Initialize();
	void InitializeEnemies();
	void UpdatePickUps(float elapsedSec);
	void DrawPickUps() const;
	void DrawEnemy() const;
};