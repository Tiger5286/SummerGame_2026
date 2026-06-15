#include "VultureStateIdle.h"
#include "Vulture.h"
#include "Utility/MyLib.h"

#include "VultureStateAttack.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_Flying";
	// 移動速度
	constexpr float kMoveSpeed = 5.0f;
	// 回転速度
	constexpr float kRotateSpeed = 0.03f;
}

void VultureStateIdle::Enter(std::weak_ptr<Vulture> pVulture)
{
	m_pVulture = pVulture;
	auto vulture = m_pVulture.lock();
	vulture->m_anim.ChangeAnim(kIdleAnimName);
	vulture->m_isFlying = true;
}

void VultureStateIdle::Update()
{
	auto vulture = m_pVulture.lock();
	
	vulture->m_angle += kRotateSpeed;
	Vector3 moveVec = MyLib::GetVecAngle(vulture->m_angle);
	moveVec *= kMoveSpeed;
	vulture->m_vel = moveVec;

	if (CheckHitKey(KEY_INPUT_C))
	{
		ChangeState(std::make_shared<VultureStateAttack>());
		return;
	}
}

void VultureStateIdle::Exit()
{

}