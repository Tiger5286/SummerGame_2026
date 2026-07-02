#include "GeneralStateDeath.h"
#include "General.h"

void GeneralStateDeath::Enter(std::weak_ptr<Character> pOwner)
{
	auto general = std::dynamic_pointer_cast<General>(pOwner.lock());
	m_pGeneral = general;
	general->m_anim.ChangeAnim(L"General|Death", 0.5f, false);
	general->m_isDying = true;
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
