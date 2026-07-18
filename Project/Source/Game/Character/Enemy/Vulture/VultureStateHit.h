#pragma once
#include "VultureStateBase.h"
class VultureStateHit :
    public VultureStateBase
{
public:
    VultureStateHit() = default;
    ~VultureStateHit() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:

};

