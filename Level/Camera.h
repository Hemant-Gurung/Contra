#pragma once
#include "structs.h"
class Camera final
{
public:
	Camera(float width, float height);

	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(Camera&&) = delete;

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target);
	Rectf GetCameraBoundaries() const;
private:
	float m_Width{};
	float m_Height{};
	Rectf m_LevelBoundaries{};
	Point2f Track(const Rectf& target);
	void Clamp(Point2f& bottomLeftPos);
	Point2f m_LeftBottom{};
	float m_CamerLeftBoundary;
};