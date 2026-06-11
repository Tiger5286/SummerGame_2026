#pragma once
#include "System/Animator.h"
#include "Utility/Vector3.h"

class SpinWing
{
public:
	SpinWing() = default;
	~SpinWing();

	void Init(const Vector3& pos);
	void Update();
	void Draw();

	float GetAngle() const { return m_angle; }
	void SetPos(const Vector3& pos) { m_pos = pos; }
private:
	int m_frame = 0;
	int m_handle = -1;
	Animator m_anim;
	Vector3 m_pos;
	float m_angle = 0.0f;
	float m_angleVel = 0.0f;
	float m_scale = 0.0f;
};

