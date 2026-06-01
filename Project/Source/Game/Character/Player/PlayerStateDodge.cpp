#include "PlayerStateDodge.h"
#include "Player.h"
#include "Utility/Matrix4x4.h"

#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateFall.h"

namespace
{
	// アニメーション名
	const std::wstring kRollingAnimName = L"Player|Rolling";
	// 回避の速度
	constexpr float kDodgeSpeed = 15.0f;
	// 回避のフレーム数
	constexpr int kDodgeFrame = 30;
}

void PlayerStateDodge::Enter(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->m_anim.ChangeAnim(kRollingAnimName, 0.5f, false);
	m_pPlayer->RotateInputDir();
}

void PlayerStateDodge::Update()
{
		// 回避中のフレームをカウント
		m_dodgeFrame++;
		// 一定時間経過したら回避終了
		if (m_dodgeFrame > kDodgeFrame)
		{
			// 接地していなかったらfall
			if (!(m_pPlayer->m_isGround))
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
		moveVec *= Matrix4x4::GetRotY(m_pPlayer->m_angle);
		m_pPlayer->m_vel.x = moveVec.x;
		m_pPlayer->m_vel.z = moveVec.z;
}

void PlayerStateDodge::Exit()
{

}
