#include "SphereCollider.h"
#include "Dxlib.h"

SphereCollider::SphereCollider(ColliderType type, float radius):
	ColliderBase(type),
	m_radius(radius)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Draw()
{
	DrawSphere3D(m_pos.ToDxLib(), m_radius, 8, 0xffff00, 0xffffff, false);
}
