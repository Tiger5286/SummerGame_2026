#pragma once
#include "Utility/Vector3.h"
#include "System/Animator.h"

class BurningWing
{
public:
	BurningWing() = default;
	~BurningWing();

	void Init(const Vector3& pos,float angle);
	void Update();
	void Draw();
private:
	int m_frame = 0;
	Animator m_anim;
	int m_modelHandle = -1;
	float m_scale = 0.0f;
	Vector3 m_pos;
	float m_angle = 0.0f;
};

