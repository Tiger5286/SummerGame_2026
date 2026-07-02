#include "VultureStateAttack.h"
#include "Vulture.h"
#include "../../Player/Player.h"
#include "Utility/MyLib.h"
#include "../../Attack.h"

#include "VultureStateIdle.h"

namespace
{
	// アニメーション名
	const std::wstring kAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_Gliding";

	// 降下にかかるフレーム数
	constexpr int kDescentFrame = 60;
	// 突進し始めるフレーム数
	constexpr int kStartChargeFrame = kDescentFrame + 60;
	// 突進し終わるフレーム数
	constexpr int kEndChargeFrame = kStartChargeFrame + 45;

	// 突進のスピード
	constexpr float kChargeSpeed = 20.0f;

	// 攻撃の情報
	const MyLib::AttackData kAttackData = {
		.colliderRadius = 80.0f,
		.damage = 20,
		.hitCharacterType = MyLib::CharacterType::Player,
		.isKnockDown = false
	};
}

void VultureStateAttack::Enter(std::weak_ptr<Character> pOwner)
{
	m_pVulture = std::dynamic_pointer_cast<Vulture>(pOwner.lock());
	auto vulture = m_pVulture.lock();
	// プレイヤーとの高さの差を記録
	m_toPlayerHeightDif = vulture->m_pos.y - (vulture->m_pPlayer->GetPos().y + 50.0f);
}

void VultureStateAttack::Update()
{
	m_frame++;
	auto vulture = m_pVulture.lock();

	// 下降
	if (m_frame < kDescentFrame)
	{
		vulture->m_flyHeight = 50.0f;
		vulture->m_pos.y -= abs(m_toPlayerHeightDif) / kDescentFrame;
	}
	// 突進待機
	if (m_frame < kStartChargeFrame)
	{
		// 突進するまでプレイヤーの方向を向く
		m_toPlayerDir = (vulture->m_pPlayer->GetPos() - vulture->m_pos).Normalized();
		// プレイヤーへの方向を向く
		vulture->m_angle = atan2f(m_toPlayerDir.z, -m_toPlayerDir.x) + DX_PI_F / 2;
	}
	// 突進
	if (m_frame == kStartChargeFrame)
	{
		// 攻撃を生成
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData);
		m_pAtk->Init();
		m_pAtk->SetPos(vulture->m_pos + Vector3(0,50,0));
		// アニメーションを変更
		vulture->m_anim.ChangeAnim(kAnimName);
	}
	// 突進中
	if (m_frame > kStartChargeFrame && m_frame < kEndChargeFrame)
	{	// プレイヤーの方向に突進
		vulture->m_vel = m_toPlayerDir * kChargeSpeed;
		// 攻撃の位置を設定
		m_pAtk->SetPos(vulture->m_pos + Vector3(0, 50, 0));
	}
	// 攻撃の更新
	if (m_pAtk != nullptr)
	{
		m_pAtk->Update();
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

void VultureStateAttack::Draw()
{
#ifdef _DEBUG
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}
