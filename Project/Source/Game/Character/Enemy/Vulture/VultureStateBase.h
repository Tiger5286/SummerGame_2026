#pragma once
#include "../../CharacterStateBase.h"
#include <memory>

class Vulture;

class VultureStateBase : public CharacterStateBase
{
public:
	VultureStateBase() = default;
	virtual ~VultureStateBase() = default;

	virtual void Enter(std::weak_ptr<Character> pOwner) = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual void Draw() {}

protected:
	std::weak_ptr<Vulture> m_pVulture;
};

