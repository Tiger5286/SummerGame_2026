#include "PlayerStateSpin.h"
#include "Player.h"
#include "../Attack.h"
#include "Wing/SpinWing.h"
#include "Utility/Matrix4x4.h"
#include "Singleton/EffectManager.h"
#include "Singleton/SoundManager.h"

#include "PlayerStateIdle.h"

namespace
{
	// アニメーション名
	const std::wstring kAnimName = L"Player|SpinAttack";
	// 攻撃を生成するフレーム
	constexpr int kAttackFrame[3] = { 40,55,70 };
	// 攻撃を消すフレーム
	constexpr int kEndAttackFrame = 100;
	// 攻撃の情報
	const MyLib::AttackData kAttackData = {
		.colliderRadius = 100.0f,
		.damage = 70,
		.hitCharacterType = MyLib::CharacterType::Enemy,
		.isKnockDown = true,
		.isIgnoreInvincible = false,
		.specialCharge = 25
	};
	// 攻撃のプレイヤーからのオフセット
	const Vector3 kAttackOffsetL = Vector3(-130, 100, 0);
	const Vector3 kAttackOffsetR = Vector3(130, 100, 0);
	// 翼の位置のプレイヤーからのオフセット
	const Vector3 kWingOffset = Vector3(0, 100, 0);
}

void PlayerStateSpin::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kAnimName, 0.5f, false);

	m_pWing = std::make_shared<SpinWing>();
	m_pWing->Init(player->m_pos + kWingOffset);

	m_effHandle = EffectManager::GetInstance().PlayEffect(L"Spin", player->m_pos + kWingOffset);

	player->m_skillCooltime = 0;

	if (!player->m_isGround)
	{
		player->m_anim.SetFloatAnimLowerBody(true);
	}

	SoundManager::GetInstance().PlaySoundGame(L"Shift");
}

void PlayerStateSpin::Update()
{
	m_frame++;

	auto player = m_pPlayer.lock();
	// 翼の更新
	m_pWing->SetPos(player->m_pos + kWingOffset);
	m_pWing->Update();
	// プレイヤーのy速度を0にする(落下しないようにする)
	player->m_vel.y = 0.0f;

	// 攻撃の生成	// 指定のフレームになったら攻撃を生成する
	if (m_frame == kAttackFrame[0] || m_frame == kAttackFrame[1] || m_frame == kAttackFrame[2])
	{
		m_pAttackL = std::make_shared<Attack>();
		m_pAttackL->SetData(kAttackData,shared_from_this());
		m_pAttackL->Init();
		m_pAttackR = std::make_shared<Attack>();
		m_pAttackR->SetData(kAttackData, shared_from_this());
		m_pAttackR->Init();
		if (m_frame == kAttackFrame[0])
		{
			SoundManager::GetInstance().PlaySoundGame(L"Spin");
		}
	}
	// 攻撃の更新
	if (m_pAttackL != nullptr)
	{	// 翼の回転に合わせて攻撃も回転させる
		Vector3 offset = kAttackOffsetL * Matrix4x4::GetRotY(m_pWing->GetAngle());
		m_pAttackL->SetPos(player->m_pos + offset);
		m_pAttackL->Update();
	}
	if (m_pAttackR != nullptr)
	{	// 翼の回転に合わせて攻撃も回転させる
		Vector3 offset = kAttackOffsetR * Matrix4x4::GetRotY(m_pWing->GetAngle());
		m_pAttackR->SetPos(player->m_pos + offset);
		m_pAttackR->Update();
	}
	// 攻撃の削除
	if (m_frame > kEndAttackFrame)
	{
		m_pAttackL = nullptr;
		m_pAttackR = nullptr;
	}

	// アニメーションが終わったらIdle
	if (player->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateSpin::Exit()
{
	EffectManager::GetInstance().StopEffect(m_effHandle);
	auto player = m_pPlayer.lock();
	player->m_anim.SetFloatAnimLowerBody(false);
}

void PlayerStateSpin::Draw()
{
	// 翼の描画
	m_pWing->Draw();
#ifdef _DEBUG
	// 攻撃の描画(デバッグ用)
	if (m_pAttackL != nullptr)
	{
		m_pAttackL->Draw();
	}
	if (m_pAttackR != nullptr)
	{
		m_pAttackR->Draw();
	}
#endif
}
