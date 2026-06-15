#include "VultureStateAttack.h"
#include "Vulture.h"
#include "../../Player/Player.h"

#include "VultureStateIdle.h"

namespace
{
	// 降下にかかるフレーム数
	constexpr int kDescentFrame = 60;
	// 突進し始めるフレーム数
	constexpr int kStartChargeFrame = kDescentFrame + 60;
	// 突進し終わるフレーム数
	constexpr int kEndChargeFrame = kStartChargeFrame + 45;

	// 突進のスピード
	constexpr float kChargeSpeed = 10.0f;
}

void VultureStateAttack::Enter(std::weak_ptr<Vulture> pVulture)
{
	m_pVulture = pVulture;
	auto vulture = m_pVulture.lock();

	m_toPlayerHeightDif = vulture->m_pos.y - vulture->m_pPlayer->GetPos().y;
}

void VultureStateAttack::Update()
{
	m_frame++;
	auto vulture = m_pVulture.lock();

	// 下降
	if (m_frame < kDescentFrame)
	{
		vulture->m_flyHeight = vulture->m_pPlayer->GetPos().y;
		vulture->m_vel.y = abs(m_toPlayerHeightDif) / kDescentFrame;
	}
	// 突進
	if (m_frame == kStartChargeFrame)
	{	// 突進開始時にプレイヤーへの方向を覚える
		m_toPlayerDir = (vulture->m_pPlayer->GetPos() - vulture->m_pos).Normalized();
	}
	if (m_frame > kStartChargeFrame && m_frame < kEndChargeFrame)
	{	// プレイヤーの方向に突進
		vulture->m_vel = m_toPlayerDir * kChargeSpeed;
	}

	// 突進が終わったらidle
	if (m_frame > kEndChargeFrame)
	{
		ChangeState(std::make_shared<VultureStateIdle>());
		return;
	}
}

void VultureStateAttack::Exit()
{
	m_pVulture.lock()->m_flyHeight = 300.0f;
}