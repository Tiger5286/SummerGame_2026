#include "VultureStateDown.h"
#include "Vulture.h"

namespace
{
	const std::wstring kAnimName = L"VultureCinereous_Skelmesh|AA_VultureCinereous_Idle";
}

void VultureStateDown::Enter(std::weak_ptr<Vulture> pVulture)
{
	m_pVulture = pVulture;
	auto vulture = m_pVulture.lock();

	vulture->m_anim.ChangeAnim(kAnimName);
	vulture->m_isFlying = false;
}

void VultureStateDown::Update()
{

}

void VultureStateDown::Exit()
{

}
