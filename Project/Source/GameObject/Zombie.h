#pragma once
#include "GameObject.h"
#include "../System/Animator.h"
#include <memory>

class Player;

class Zombie : public GameObject
{
public:
	Zombie();
	virtual ~Zombie() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	// プレイヤーのポインタを設定する(Initの前に実行する)
	void SetPlayer(const std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

private:
	Animator m_anim;
	float m_angle = 0.0f;

	std::shared_ptr<Player> m_pPlayer = nullptr;	// プレイヤーのポインタ
};

