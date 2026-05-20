#pragma once
#include "GameObject.h"
#include "../System/Animator.h"

class Zombie : public GameObject
{
public:
	Zombie();
	virtual ~Zombie() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	// プレイヤーの位置を設定する(毎フレームUpdateの前に呼ぶ)
	void SetPlayerPos(const Vector3& pos) { m_playerPos = pos; }

private:
	Animator m_anim;

	Vector3 m_playerPos;	// プレイヤーの位置
};

