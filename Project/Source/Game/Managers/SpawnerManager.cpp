#include "SpawnerManager.h"
#include "../EnemySpawner.h"
#include <string>

namespace
{
	const std::wstring kfilePath = L"data/csv/SpawnerData.csv";
}

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
