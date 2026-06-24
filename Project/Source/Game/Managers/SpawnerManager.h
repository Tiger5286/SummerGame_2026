#pragma once
#include <memory>
#include <vector>

class EnemySpawner;

class SpawnerManager
{
	SpawnerManager() = default;
	~SpawnerManager() = default;

	void Load();

	void Update();
	void Draw();
private:
	std::vector<std::shared_ptr<EnemySpawner>> m_pEnemySpawners;
};

