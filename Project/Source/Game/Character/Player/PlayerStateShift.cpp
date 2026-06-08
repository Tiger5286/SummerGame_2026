#include "PlayerStateShift.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Singleton/EffectManager.h"
#include "Utility/Matrix4x4.h"
#include <EffekseerForDXLib.h>

#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"

namespace
{
	const std::wstring kShiftAnimName = L"Player|Shift";

	constexpr float kTrackingDist = 1000.0f;

	constexpr float kMoveStartTimeRate = 0.3f;
	constexpr float kMoveEndTimeRate = 0.8f;
	constexpr float kMoveSpeed = 20.0f;

	constexpr float kInvisibleStartTimeRate = 0.3f;
	constexpr float kInvisibleEndTimeRate = 0.9f;

	// 攻撃の前進をやめる距離
	constexpr float kStopTrackingDist = 120.0f;
}

void PlayerStateShift::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kShiftAnimName, 0.5f, false);
	player->RotateToTarget(kTrackingDist);
}

void PlayerStateShift::Update()
{
	auto player = m_pPlayer.lock();
	auto& input = Input::GetInstance();

	// 移動する
	if (player->m_anim.GetAnimRate() > kMoveStartTimeRate && player->m_anim.GetAnimRate() < kMoveEndTimeRate)
	{
		player->m_vel = Vector3::Back() * kMoveSpeed * Matrix4x4::GetRotY(player->m_angle);
		// 敵との距離が近かったら速度を0にする
		if (player->m_target != nullptr)
		{
			float squareDist = (player->GetPos() - player->m_target->GetPos()).SquaredLength();
			if (squareDist < kStopTrackingDist * kStopTrackingDist)
			{
				player->m_vel = Vector3::Zero();
			}
		}
	}
	else
	{
		player->m_vel = Vector3::Zero();
	}

	// 透明化
	if (player->m_anim.GetAnimRate() > kInvisibleStartTimeRate && player->m_anim.GetAnimRate() < kInvisibleEndTimeRate)
	{
		player->m_isInvisible = true;
		if (!m_isPlayedEffect)
		{
			auto effHandle = EffectManager::GetInstance().PlayEffect(L"Shift", player->m_pos);
			SetRotationPlayingEffekseer3DEffect(effHandle, 0.0f, player->m_angle, 0.0f);
			m_isPlayedEffect = true;
		}
	}
	else
	{
		player->m_isInvisible = false;
	}

	// アニメーションが終わったらステート遷移
	if (player->m_anim.IsEnd())
	{
		// 移動していたらmoveへ
		if (input.GetStickInput(LR::Left).SquaredLength() > 0.0f)
		{
			ChangeState(std::make_shared<PlayerStateMove>());
			return;
		}
		// そうでないならidleへ
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateShift::Exit()
{
	m_pPlayer.lock()->m_isInvisible = false;
}
