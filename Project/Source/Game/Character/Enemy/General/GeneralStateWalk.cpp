#include "GeneralStateWalk.h"
#include "General.h"
#include "../../Player/Player.h"
#include "Utility/Matrix4x4.h"
#include "Utility/MyLib.h"

#include "GeneralStateHeavySlash.h"
#include "GeneralStateThrust.h"
#include "GeneralStateProjectile.h"

namespace
{
	const std::wstring kAnimName = L"General|Walk";

	constexpr float kMoveSpeed = 4.0f;

	constexpr int kChangeStateFrame = 180;
}

void GeneralStateWalk::Enter(std::weak_ptr<General> pGeneral)
{
	m_pGeneral = pGeneral;
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName);
}

void GeneralStateWalk::Update()
{
	m_frame++;
	auto general = m_pGeneral.lock();
	auto player = general->m_pPlayer;

	// 移動処理
	// プレイヤーへの方向を取得
	Vector3 toPlayer = player->GetPos() - general->m_pos;
	toPlayer.y = 0.0f;
	toPlayer.Normalize();
	// プレイヤーから見て左に移動する
	Vector3 moveVec = toPlayer * Matrix4x4::GetRotY(DX_PI_F / 2);
	moveVec *= kMoveSpeed;
	general->m_vel = moveVec;
	// プレイヤーの方を向く
	general->m_angle = MyLib::GetAngleVec(toPlayer.z, toPlayer.x);
}

void GeneralStateWalk::Exit()
{

}