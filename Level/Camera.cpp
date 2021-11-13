#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(float width, float height)
	:m_Width{ width }
	, m_Height{ height }
	, m_LevelBoundaries{ Rectf{0.f,0.f,width,height} }
	, m_CamerLeftBoundary{}

{

}

void Camera::SetLevelBoundaries(const Rectf& levelboundaries)
{
	m_LevelBoundaries = levelboundaries;
}

void Camera::Transform(const Rectf& target)
{
	m_LeftBottom = Track(target);
	glTranslatef(-m_LeftBottom.x, -m_LeftBottom.y, 0.f);
}

Point2f Camera::Track(const Rectf& target)
{
    m_CamerLeftBoundary = m_LeftBottom.x;
	m_LeftBottom.x = (target.left + target.width / 2) - m_Width / 2;
	m_LeftBottom.y = (target.bottom + target.height / 2) - m_Height;
	Clamp(m_LeftBottom);
	return m_LeftBottom;
}

void Camera::Clamp(Point2f& bottomLeftPos)
{
	if (bottomLeftPos.x <= m_LevelBoundaries.left)
	{
		m_LeftBottom.x =0;
	}
	if (bottomLeftPos.x <= m_CamerLeftBoundary)
	{
		m_LeftBottom.x = m_CamerLeftBoundary;
	}
	
	if (bottomLeftPos.x + m_Width >= m_LevelBoundaries.width)
	{
		m_LeftBottom.x = m_LevelBoundaries.width - m_Width;
	}

	if (bottomLeftPos.y + m_Height >= m_LevelBoundaries.height)
	{
		m_LeftBottom.y = m_LevelBoundaries.height-m_Height;
	}
	if (bottomLeftPos.y <=0 )
	{
		m_LeftBottom.y = 0;
	}
}
Rectf Camera::GetCameraBoundaries() const
{
	Rectf cameraBoundry = Rectf(m_LeftBottom.x,m_LeftBottom.y, m_Width,m_Height);
	return cameraBoundry;
}