#include "PlayerStateBase.h"
#include "Player.h"

void PlayerStateBase::AddSpecialCharge(int value)
{
	m_pPlayer.lock()->m_specialCharge += value;
}
