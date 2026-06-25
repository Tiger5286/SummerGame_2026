#pragma once
#include <memory>

class Player;
class Camera;
class EnemyManager;
class EnemyBase;

class TargetManager
{
public:
	TargetManager() = default;
	~TargetManager() = default;

	void Init(std::shared_ptr<Player> pPlayer,std::shared_ptr<Camera> pCamera,std::shared_ptr<EnemyManager> pEnemyManager);
	void Update();
	void Draw();

private:
	std::shared_ptr<EnemyBase> SearchTarget();

private:
	std::shared_ptr<Player> m_pPlayer = nullptr;
	std::shared_ptr<Camera> m_pCamera = nullptr;
	std::shared_ptr<EnemyManager> m_pEnemyManager = nullptr;

	std::shared_ptr<EnemyBase> m_pTarget = nullptr;
};

