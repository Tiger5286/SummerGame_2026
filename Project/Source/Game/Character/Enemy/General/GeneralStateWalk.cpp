#include "GeneralStateWalk.h"
#include "General.h"
#include "../../Player/Player.h"
#include "Utility/Matrix4x4.h"
#include "Utility/MyLib.h"

#include "GeneralStateIdle.h"

namespace
{
	const std::wstring kAnimName = L"General|Walk";

	constexpr float kMoveSpeed = 4.0f;

	constexpr int kChangeStateFrame = 180;

	constexpr float kIdleDist = 500.0f;
}

void GeneralStateWalk::Enter(std::weak_ptr<Character> pOwner)
{
	m_pGeneral = std::dynamic_pointer_cast<General>(pOwner.lock());
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName);
}

void GeneralStateWalk::Update()
{
	auto general = m_pGeneral.lock();
	auto player = general->m_pPlayer;

	// クールタイムを減らす
	general->m_attackCooltime--;

	// 移動処理
	// プレイヤーへの方向を取得
	Vector3 toPlayer = player->GetPos() - general->m_pos;
	toPlayer.y = 0.0f;
	toPlayer.Normalize();
	// プレイヤーから見て左に移動する
	Vector3 moveVec = toPlayer * Matrix4x4::GetRotY(DX_PI_F / 2);
	moveVec *= kMoveSpeed;
	moveVec.y = general->m_vel.y;
	general->m_vel = moveVec;
	// プレイヤーの方を向く
	general->m_angle = MyLib::GetAngleVec(toPlayer.z, toPlayer.x);

	// クールタイムが終わっていたら攻撃
	if (general->m_attackCooltime < 0)
	{
		general->AttackRandom();
		return;
	}
}

void GeneralStateWalk::Exit()
{

}