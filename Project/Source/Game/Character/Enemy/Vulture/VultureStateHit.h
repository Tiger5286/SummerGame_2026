#pragma once
#include "VultureStateBase.h"
class VultureStateHit :
    public VultureStateBase
{
public:
    VultureStateHit() = default;
    ~VultureStateHit() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
private:

};

