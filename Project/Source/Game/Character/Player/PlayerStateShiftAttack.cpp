#include "PlayerStateShiftAttack.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Utility/MyLib.h"
#include "../Attack.h"
#include "Singleton/SoundManager.h"

#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"

namespace
{
	constexpr int kCanTransStateFrame = 28;

	constexpr int kStartColFrame = 8;
	constexpr int kEndColFrame = 24;

	MyLib::AttackData kAttackData = {
		.colliderRadius = 100.0f,
		.damage = 50,
		.hitCharacterType = MyLib::CharacterType::Enemy,
		.isKnockDown = false,
		.isIgnoreInvincible = false,
		.specialCharge = 30
	};
	const Vector3 kColliderOffset = Vector3(0, 100, -100);
}

void PlayerStateShiftAttack::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(L"Player|ShiftAttack", MyLib::kDefaultAnimSpeed, false);
	if (!player->m_isGround)
	{
		player->m_anim.SetFloatAnimLowerBody(true);
	}
}

void PlayerStateShiftAttack::Update()
{
	m_frame++;
	auto player = m_pPlayer.lock();
	auto& input = Input::GetInstance();

	// 落下しないようにする
	player->m_vel.y = 0.0f;

	// 当たり判定開始
	if (m_frame == kStartColFrame)
	{
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData, shared_from_this());
		m_pAtk->Init();
		SoundManager::GetInstance().PlaySoundGame(L"HeavySlash");
	}
	// 当たり判定終了
	if (m_frame >= kEndColFrame)
	{
		m_pAtk = nullptr;
	}
	// 当たり判定の移動
	if (m_pAtk != nullptr)
	{
		Vector3 colPos = player->m_pos + (kColliderOffset * Matrix4x4::GetRotY(player->m_angle));
		m_pAtk->SetPos(colPos);
		m_pAtk->Update();
	}

	// ほかのステートに移行可能になったときに移行する行動をとれば移行する
	if (m_frame > kCanTransStateFrame)
	{
		// 攻撃ボタンを押していたらAttack
		if (input.IsPressed(player->kAttack))
		{
			ChangeState(std::make_shared<PlayerStateAttack>());
			return;
		}

		// 移動していたらmove
		if (input.GetStickInput(MyLib::LR::Left).SquaredLength() > 0.0f)
		{
			ChangeState(std::make_shared<PlayerStateMove>());
			return;
		}
	}

	// アニメーションが終わったらIdle
	if (player->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateShiftAttack::Exit()
{
	auto player = m_pPlayer.lock();
	player->m_anim.SetFloatAnimLowerBody(false);
}

void PlayerStateShiftAttack::Draw()
{
#ifdef _DEBUG
	// 攻撃の当たり判定のデバッグ表示
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}
