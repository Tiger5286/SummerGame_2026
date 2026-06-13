#include "VultureStateIdle.h"
#include "Vulture.h"

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

}

void VultureStateIdle::Exit()
{

}