#pragma once
#include "ColliderBase.h"
class SphereCollider :
    public ColliderBase
{
public:
    SphereCollider(float radius);
    virtual ~SphereCollider();

    void Draw() override;

    MV1_COLL_RESULT_POLY_DIM CheckCollModel(int mapHandle) override;

	float GetRadius() const { return m_radius; }

private:
    float m_radius = 0.0f;
};

