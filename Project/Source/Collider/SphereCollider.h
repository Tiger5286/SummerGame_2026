#pragma once
#include "ColliderBase.h"
class SphereCollider :
    public ColliderBase
{
public:
    SphereCollider(ColliderType type,float radius);
    virtual ~SphereCollider();

    void Draw() override;

private:
    float m_radius = 0.0f;
};

