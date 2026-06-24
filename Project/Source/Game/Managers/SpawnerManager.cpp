#include "SpawnerManager.h"
#include "../EnemySpawner.h"

void SpawnerManager::Load()
{

}

void SpawnerManager::Update()
{
	for (auto& spawner : m_pEnemySpawners)
	{
		spawner->Update();
	}
}

void SpawnerManager::Draw()
{
	for (auto& spawner : m_pEnemySpawners)
	{
		spawner->Draw();
	}
}
