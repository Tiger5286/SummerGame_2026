#include "VultureStateDeath.h"
#include "Vulture.h"

namespace
{
	const std::wstring kAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_Land";
}

void VultureStateDeath::Enter(std::weak_ptr<Vulture> pVulture)
{
	m_pVulture = pVulture;
	auto vulture = m_pVulture.lock();
	vulture->m_anim.ChangeAnim(kAnimName, 0.5f, false);
}

void VultureStateDeath::Update()
{
	auto vulture = m_pVulture.lock();
	
	if (vulture->m_anim.IsEnd())
	{
		vulture->m_isDead = true;
	}
}

void VultureStateDeath::Exit()
{

}
