#pragma once
#include "ColliderBase.h"

class CapsuleCollider :
    public ColliderBase
{
public:
	CapsuleCollider(float radius, float height);
	virtual ~CapsuleCollider();

	void Draw() override;

	MV1_COLL_RESULT_POLY_DIM CheckCollModel(int mapHandle) override;

	float GetRadius() const { return m_radius; }
	float GetHeight() const { return m_height; }


private:
	float m_radius = 0.0f;	// 半径
	float m_height = 0.0f;	// 高さ
};