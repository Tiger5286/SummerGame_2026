#pragma once
#include "VultureStateBase.h"
class VultureStateDown :
    public VultureStateBase
{
public:
    VultureStateDown() = default;
    ~VultureStateDown() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:
    int m_groundFrame = 0;
};

