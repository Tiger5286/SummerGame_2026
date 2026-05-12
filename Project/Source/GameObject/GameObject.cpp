#include "GameObject.h"

namespace
{
	constexpr float kResistancePower = 1.0f;
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Resistance()
{
	// 速度と逆向きのベクトルを生成
	auto resistVec = -m_vel;
	resistVec.y = 0.0f;
	// 長さを固定
	resistVec.Normalize();
	resistVec *= kResistancePower;
	// 速度に足す
	m_vel += resistVec;
	// 速度が一定以下になったら止まる
	auto velXZ = Vector3(m_vel.x, 0.0f, m_vel.z);
	if (velXZ.SquaredLength() < kResistancePower)
	{
		m_vel.x = 0.0f;
		m_vel.z = 0.0f;
	}
}

void GameObject::Gravity(float power)
{
	m_vel.y -= power;
	if (m_pos.y <= 0.0f)
	{
		m_vel.y = 0.0f;
		m_pos.y = 0.0f;
	}
}
