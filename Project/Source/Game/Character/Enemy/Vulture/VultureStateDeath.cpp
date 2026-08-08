#include "VultureStateDeath.h"
#include "Vulture.h"
#include "Singleton/SoundManager.h"

namespace
{
	const std::wstring kAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_Land";
}

void VultureStateDeath::OnEnter()
{
	m_pVulture = std::dynamic_pointer_cast<Vulture>(m_pOwner.lock());
	auto vulture = m_pVulture.lock();
	vulture->m_anim.ChangeAnim(kAnimName, 0.5f, false);
	vulture->m_isDying = true;
	vulture->m_isFlying = false;
	vulture->m_isCanHitAttack = false;
	SoundManager::GetInstance().PlaySoundGame(L"VultureDeath");
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
