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
	constexpr int kMoveFrame = 30;
	constexpr float kNoTargetMoveDist = 500.0f;

	constexpr float kInvisibleStartTimeRate = 0.3f;
	constexpr float kInvisibleEndTimeRate = 0.9f;

	// 敵との距離がこれになったら止まる
	constexpr float kStopTrackingDist = 120.0f;
}

void PlayerStateShift::Enter(std::weak_ptr<Character> pOwner)
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(pOwner.lock());
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kShiftAnimName, 0.5f, false);
	player->RotateToTarget(1500.0f);
	// プレイヤーからターゲットへのベクトルを計算しておく
	if (player->m_target != nullptr)
	{
		Vector3 target = player->m_target->GetPos() + player->m_target->GetVel() * 10;
		m_playerToTarget = target - player->m_pos;
		if (m_playerToTarget.SquaredLength() > kTrackingDist * kTrackingDist)
		{
			m_playerToTarget.Normalize();
			m_playerToTarget *= kTrackingDist;
		}
		auto len = m_playerToTarget.Length();
		if (m_moveDist < len)
		{
			m_moveDist = len;
		}
	}
	// 空中で発動可能フラグを消す
	player->m_isCanAirShift = false;
}

void PlayerStateShift::Update()
{
	auto player = m_pPlayer.lock();
	auto& input = Input::GetInstance();

	// 移動時間中なら移動する
	if (player->m_anim.GetAnimRate() > kMoveStartTimeRate && player->m_anim.GetAnimRate() < kMoveEndTimeRate)
	{
		// プレイヤーからターゲットへのベクトルを計算しておく
		if (player->m_target != nullptr)
		{
			Vector3 target = player->m_target->GetPos() + player->m_target->GetVel() * 10;
			m_playerToTarget = target - player->m_pos;
			if (m_playerToTarget.SquaredLength() > kTrackingDist * kTrackingDist)
			{
				m_playerToTarget.Normalize();
				m_playerToTarget *= kTrackingDist;
			}
			auto len = m_playerToTarget.Length();
			if (m_moveDist < len)
			{
				m_moveDist = len;
			}
		}

		// ターゲットがいればそっちに移動する
		if (player->m_target != nullptr)
		{	// ターゲットの方向へ移動するベクトル
			player->m_vel = m_playerToTarget.Normalized() * m_moveDist / kMoveFrame;
			// 距離が近かったら移動を止める
			float squareDist = (player->GetPos() - player->m_target->GetPos()).SquaredLength();
			if (squareDist < kStopTrackingDist * kStopTrackingDist)
			{
				player->m_vel = Vector3::Zero();
				// y軸だけは一致するまで移動する
				float yDif = abs(player->GetPos().y - player->m_target->GetPos().y);
				if (yDif > 1.0f)
				{
					player->m_vel.y = m_playerToTarget.y / kMoveFrame;
				}
			}
		}
		else	// ターゲットがいなかったら
		{
			player->m_vel = Vector3::Back() * kNoTargetMoveDist * Matrix4x4::GetRotY(player->m_angle) / kMoveFrame;
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
		// シフト開始のエフェクトを一回だけ再生
		if (!m_isPlayedShiftEffect)
		{
			auto effHandle = EffectManager::GetInstance().PlayEffect(L"Shift", player->m_pos);
			SetRotationPlayingEffekseer3DEffect(effHandle, 0.0f, player->m_angle, 0.0f);

			m_isPlayedShiftEffect = true;
		}
	}
	else
	{
		player->m_isInvisible = false;
	}

	if (player->m_anim.GetAnimRate() > kInvisibleEndTimeRate)
	{	// シフト終了のエフェクトを一回だけ再生
		if (!m_isPlayedShiftEndEffect)
		{
			auto effHandle = EffectManager::GetInstance().PlayEffect(L"ShiftEnd", player->m_pos);
			SetRotationPlayingEffekseer3DEffect(effHandle, 0.0f, player->m_angle, 0.0f);

			m_isPlayedShiftEndEffect = true;
		}
	}

	// アニメーションが終わったらステート遷移
	if (player->m_anim.IsEnd())
	{
		// 移動していたらmoveへ
		if (input.GetStickInput(MyLib::LR::Left).SquaredLength() > 0.0f)
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
