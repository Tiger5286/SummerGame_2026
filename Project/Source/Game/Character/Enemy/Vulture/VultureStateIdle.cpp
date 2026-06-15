#include "VultureStateIdle.h"
#include "Vulture.h"

#include "VultureStateAttack.h"

namespace
{
	const std::wstring kIdleAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_Flying";
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
	if (CheckHitKey(KEY_INPUT_C))
	{
		ChangeState(std::make_shared<VultureStateAttack>());
		return;
	}
}

void VultureStateIdle::Exit()
{

}