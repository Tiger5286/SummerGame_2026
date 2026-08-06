#include "PlayerStateDodge.h"
#include "Player.h"
#include "Utility/Matrix4x4.h"
#include "Singleton/SoundManager.h"

#include "PlayerStateMove.h"
#include "PlayerStateFall.h"

namespace
{
	// アニメーション名
	const std::wstring kRollingAnimName = L"Player|Rolling";
	// 回避の速度
	constexpr float kDodgeSpeed = 10.0f;
	// 回避のフレーム数
	constexpr int kDodgeFrame = 30;
}

void PlayerStateDodge::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	m_pPlayer.lock()->m_anim.ChangeAnim(kRollingAnimName, 0.5f, false);
	m_pPlayer.lock()->RotateInputDir();
	m_pPlayer.lock()->m_isCanHitAttack = false;
	SoundManager::GetInstance().PlaySoundGame(L"Dodge");
}

void PlayerStateDodge::Update()
{
		// 回避中のフレームをカウント
		m_dodgeFrame++;
		// 一定時間経過したら回避終了
		if (m_dodgeFrame > kDodgeFrame)
		{
			// 接地していなかったらfall
			if (!(m_pPlayer.lock()->m_isGround))
			{
				ChangeState(std::make_shared<PlayerStateFall>());
				return;
			}
			// それ以外ならmove
			ChangeState(std::make_shared<PlayerStateMove>());
			return;
		}

		// 向いている方向に進む
		Vector3 moveVec = Vector3(0.0f, 0.0f, -kDodgeSpeed);
		moveVec *= Matrix4x4::GetRotY(m_pPlayer.lock()->m_angle);
		m_pPlayer.lock()->m_vel.x = moveVec.x;
		m_pPlayer.lock()->m_vel.z = moveVec.z;
}

void PlayerStateDodge::Exit()
{
	m_pPlayer.lock()->m_isCanHitAttack = true;
}
