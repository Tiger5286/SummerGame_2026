#include "VultureStateDown.h"
#include "Vulture.h"

#include "VultureStateIdle.h"

namespace
{
	const std::wstring kAnimName = L"VultureCinereous_Skelmesh|AA_VultureCinereous_Idle";

	constexpr int kReturnFrame = 120;
}

void VultureStateDown::OnEnter()
{
	m_pVulture = std::dynamic_pointer_cast<Vulture>(m_pOwner.lock());
	auto vulture = m_pVulture.lock();

	vulture->m_anim.ChangeAnim(kAnimName);
	vulture->m_isFlying = false;
}

void VultureStateDown::Update()
{
	auto vulture = m_pVulture.lock();
	// 接地してからの時間をカウント
	if (vulture->m_isGround)
	{
		m_groundFrame++;
	}
	// 一定時間たったら復帰する
	if (m_groundFrame > kReturnFrame)
	{
		ChangeState(std::make_shared<VultureStateIdle>());
		return;
	}
}

void VultureStateDown::Exit()
{

}
