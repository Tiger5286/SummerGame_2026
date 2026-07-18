#include "GeneralStateDeath.h"
#include "General.h"

void GeneralStateDeath::OnEnter()
{
	auto general = std::dynamic_pointer_cast<General>(m_pOwner.lock());
	m_pGeneral = general;
	general->m_anim.ChangeAnim(L"General|Death", 0.5f, false);
	general->m_isDying = true;
	general->m_isCanHitAttack = false;
}

void GeneralStateDeath::Update()
{
	auto general = m_pGeneral.lock();
	if (general->m_anim.IsEnd())
	{
		general->m_isDead = true;
	}
}

void GeneralStateDeath::Exit()
{

}
