#include "GeneralStateAppear.h"
#include "General.h"
#include "Singleton/FadeManager.h"

#include "GeneralStateWalk.h"

void GeneralStateAppear::OnEnter()
{
	m_pGeneral = std::dynamic_pointer_cast<General>(m_pOwner.lock());
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(L"General|Idle");
}

void GeneralStateAppear::Update()
{
	auto general = m_pGeneral.lock();

	m_frame++;

	if (m_frame == 120)
	{
		general->m_anim.ChangeAnim(L"General|Power", 0.3f, false);
	}
	if (general->m_anim == L"General|Power" && general->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<GeneralStateWalk>());
		return;
	}
}

void GeneralStateAppear::Exit()
{
}
