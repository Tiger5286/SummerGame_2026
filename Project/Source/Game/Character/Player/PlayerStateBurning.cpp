#include "PlayerStateBurning.h"
#include "Player.h"
#include "../../Camera/Camera.h"
#include "../../Camera/CameraStateBurning.h"
#include "../../Camera/CameraStateFree.h"
#include "../Attack.h"
#include "Singleton/EffectManager.h"
#include "Wing/BurningWing.h"

#include "PlayerStateIdle.h"

namespace
{
	// プレイヤーの燃焼状態のアニメーション名
	const std::wstring kBurningAnimName = L"Player|Power";
	// 攻撃の開始フレームと終了フレーム
	constexpr int kStartAttackFrame = 120;
	constexpr int kEndAttackFrame = 300;
	// 1秒あたりの攻撃回数
	constexpr float kAttackPerSecond = 0.25f;
	// 攻撃の情報
	const MyLib::AttackData kAttackData = {
		1500.0f,
		60,
		MyLib::CharacterType::Enemy
	};
}

void PlayerStateBurning::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto player = m_pPlayer.lock();
	// アニメーションを切り替える
	player->m_anim.ChangeAnim(kBurningAnimName, 0.5f, false);
	// カメラを切り替える
	player->m_pCamera.lock()->ChangeState(std::make_shared<CameraStateBurning>());
	// ターゲットの方向を向く
	player->RotateToTarget(FLT_MAX);
	// エフェクトを再生する
	EffectManager::GetInstance().PlayEffect(L"Burning", player->m_pos + Vector3(0, 100, 0));

	// 翼を生成する
	m_pWing = std::make_shared<BurningWing>();
	m_pWing->Init(player->m_pos + Vector3(0, 100, 0), player->m_angle);

	player->m_specialCharge = 0;

	player->m_isCanHitAttack = false;
}

void PlayerStateBurning::Update()
{
	m_frame++;
	auto player = m_pPlayer.lock();
	// 翼を更新する
	m_pWing->Update();

	// 攻撃のフレームの間、一定のフレームごとに攻撃を生成する
	if (m_frame > kStartAttackFrame && m_frame < kEndAttackFrame)
	{	// 一定のフレームごとに攻撃を生成する
		if (m_frame % static_cast<int>(60 * kAttackPerSecond) == 0)
		{
			// 攻撃を生成する
			m_pAtk = std::make_shared<Attack>();
			m_pAtk->SetData(kAttackData, shared_from_this());
			m_pAtk->Init();
			m_pAtk->SetPos(player->m_pos);
		}
	}
	else
	{	// 攻撃のフレームの外なら攻撃を消す
		m_pAtk = nullptr;
	}
	// 攻撃を更新する
	if (m_pAtk != nullptr)
	{
		m_pAtk->Update();
	}
	// アニメーションが終わったら状態を切り替える
	if (player->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateBurning::Exit()
{	// このステートが終わる時にカメラを元に戻す
	m_pPlayer.lock()->m_pCamera.lock()->ChangeState(std::make_shared<CameraStateFree>());
	
	m_pPlayer.lock()->m_isCanHitAttack = true;
}

void PlayerStateBurning::Draw()
{
	// 翼を描画する
	m_pWing->Draw();
#ifdef _DEBUG
	// 攻撃を描画する
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}
