#pragma once
#include <memory>

class EnemyBase;
class Camera;

class EnemyHpBar
{
public:
	EnemyHpBar() = default;
	~EnemyHpBar();

	void Init(std::shared_ptr<EnemyBase> pEnemy, int maxHp);
	void End();
	void Update();
	void Draw();

private:
	std::shared_ptr<EnemyBase> m_pEnemy = nullptr;
	int m_maxHp = 0;
	int m_currentHp = 0;
	int m_RTHandle = -1;
};

