#include "VultureStateIdle.h"

void VultureStateIdle::Enter(std::weak_ptr<Vulture> pVulture)
{
	m_pVulture = pVulture;
}

void VultureStateIdle::Update()
{

}

void VultureStateIdle::Exit()
{

}