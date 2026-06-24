#pragma once
#include <memory>
#include <vector>
#include "Utility/MyLib.h"

class EnemySpawner;
class EnemyManager;
class Player;

class SpawnerManager
{
public:
	SpawnerManager() = default;
	~SpawnerManager() = default;

	void Load();

	void Init(std::shared_ptr<EnemyManager> pEnemyManager, std::shared_ptr<Player> pPlayer);
	void Update();
	void Draw();

private:
	std::vector<std::shared_ptr<EnemySpawner>> m_pEnemySpawners;
	std::shared_ptr<EnemyManager> m_pEnemyManager = nullptr;
	std::shared_ptr<Player> m_pPlayer = nullptr;
};

