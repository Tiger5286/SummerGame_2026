#include "SphereCollider.h"
#include "Dxlib.h"

SphereCollider::SphereCollider(float radius):
	ColliderBase(ColliderType::Sphere),
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
