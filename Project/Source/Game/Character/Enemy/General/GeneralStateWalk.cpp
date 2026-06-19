#include "GeneralStateWalk.h"
#include "General.h"
#include "../../Player/Player.h"
#include "Utility/Matrix4x4.h"
#include "Utility/MyLib.h"

namespace
{
	const std::wstring kAnimName = L"General|Walk";

	constexpr float kMoveSpeed = 4.0f;
}

void GeneralStateWalk::Enter(std::weak_ptr<General> pGeneral)
{
	m_pGeneral = pGeneral;
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName);
}

void GeneralStateWalk::Update()
{
	auto general = m_pGeneral.lock();
	auto player = general->m_pPlayer;

	Vector3 toPlayer = player->GetPos() - general->m_pos;
	toPlayer.y = 0.0f;
	toPlayer.Normalize();
	Vector3 moveVec = toPlayer * Matrix4x4::GetRotY(DX_PI_F / 2);
	moveVec *= kMoveSpeed;
	general->m_vel = moveVec;

	general->m_angle = MyLib::GetAngleVec(toPlayer.z, toPlayer.x);
}

void GeneralStateWalk::Exit()
{

}
