#pragma once
#include "GeneralStateBase.h"
class GeneralStateAppear :
    public GeneralStateBase
{
public:
    GeneralStateAppear() = default;
    ~GeneralStateAppear() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;

private:
    int m_frame = 0;
};

