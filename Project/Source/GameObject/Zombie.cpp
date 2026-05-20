#include "Zombie.h"
#include <string>

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Player|Idle";
	const std::wstring kWalkAnimName = L"Player|Walk";
	const std::wstring kAttackAnimName = L"Player|Attack";

	// ゾンビの移動速度
	constexpr float kZombieMoveSpeed = 3.0f;
	// プレイヤーを見つける距離
	constexpr float kPlayerFindDist = 200.0f;
}

Zombie::Zombie()
{}

Zombie::~Zombie()
{}

void Zombie::Init()
{
	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kIdleAnimName);
}

void Zombie::End()
{}

void Zombie::Update()
{
	

	// アニメーションの更新
	m_anim.Update();
}

void Zombie::Draw()
{
	// モデルの描画
	MV1DrawModel(m_modelHandle);
}
