#pragma once
#include "../../CharacterStateBase.h"
#include <memory>

class General;

class GeneralStateBase : public CharacterStateBase
{
public:
	GeneralStateBase() = default;
	virtual ~GeneralStateBase() = default;

	virtual void OnEnter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual void Draw() {}

protected:
	std::weak_ptr<General> m_pGeneral;
};

